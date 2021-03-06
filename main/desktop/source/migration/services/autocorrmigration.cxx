/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_desktop.hxx"
#include "autocorrmigration.hxx"
#include <i18npool/mslangid.hxx>
#include <tools/urlobj.hxx>
#include <unotools/bootstrap.hxx>


using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;


//.........................................................................
namespace migration
{
//.........................................................................


    static ::rtl::OUString sSourceSubDir = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "/user/autocorr" ) );
    static ::rtl::OUString sTargetSubDir = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "/user/autocorr" ) );
    static ::rtl::OUString sBaseName = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "/acor" ) );
    static ::rtl::OUString sSuffix = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( ".dat" ) );


    // =============================================================================
    // component operations
    // =============================================================================

    ::rtl::OUString AutocorrectionMigration_getImplementationName()
    {
        static ::rtl::OUString* pImplName = 0;
	    if ( !pImplName )
	    {
            ::osl::MutexGuard aGuard( ::osl::Mutex::getGlobalMutex() );
            if ( !pImplName )
		    {
                static ::rtl::OUString aImplName( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.comp.desktop.migration.Autocorrection" ) );
			    pImplName = &aImplName;
		    }
	    }
	    return *pImplName;
    }

    // -----------------------------------------------------------------------------

    Sequence< ::rtl::OUString > AutocorrectionMigration_getSupportedServiceNames()
    {
        static Sequence< ::rtl::OUString >* pNames = 0;
	    if ( !pNames )
	    {
            ::osl::MutexGuard aGuard( ::osl::Mutex::getGlobalMutex() );
		    if ( !pNames )
		    {
                static Sequence< ::rtl::OUString > aNames(1);
                aNames.getArray()[0] = ::rtl::OUString( RTL_CONSTASCII_USTRINGPARAM( "com.sun.star.migration.Autocorrection" ) );
                pNames = &aNames;
		    }
	    }
	    return *pNames;
    }

    // =============================================================================
    // AutocorrectionMigration
    // =============================================================================

    AutocorrectionMigration::AutocorrectionMigration()
    {
    }

    // -----------------------------------------------------------------------------

    AutocorrectionMigration::~AutocorrectionMigration()
    {
    }

    // -----------------------------------------------------------------------------

    TStringVectorPtr AutocorrectionMigration::getFiles( const ::rtl::OUString& rBaseURL ) const
    {
        TStringVectorPtr aResult( new TStringVector );
        ::osl::Directory aDir( rBaseURL);

        if ( aDir.open() == ::osl::FileBase::E_None )
        {
            // iterate over directory content
            TStringVector aSubDirs;
            ::osl::DirectoryItem aItem;
            while ( aDir.getNextItem( aItem ) == ::osl::FileBase::E_None )
            {
                ::osl::FileStatus aFileStatus( FileStatusMask_Type | FileStatusMask_FileURL );
                if ( aItem.getFileStatus( aFileStatus ) == ::osl::FileBase::E_None )
                {
                    if ( aFileStatus.getFileType() == ::osl::FileStatus::Directory )
                        aSubDirs.push_back( aFileStatus.getFileURL() );
                    else
                        aResult->push_back( aFileStatus.getFileURL() );
                }
            }

            // iterate recursive over subfolders
            TStringVector::const_iterator aI = aSubDirs.begin();
            while ( aI != aSubDirs.end() )
            {
                TStringVectorPtr aSubResult = getFiles( *aI );
                aResult->insert( aResult->end(), aSubResult->begin(), aSubResult->end() );
                ++aI;
            }
        }

        return aResult;
    }

    // -----------------------------------------------------------------------------

    ::osl::FileBase::RC AutocorrectionMigration::checkAndCreateDirectory( INetURLObject& rDirURL )
    {
        ::osl::FileBase::RC aResult = ::osl::Directory::create( rDirURL.GetMainURL( INetURLObject::DECODE_TO_IURI ) );
        if ( aResult == ::osl::FileBase::E_NOENT )
        {
            INetURLObject aBaseURL( rDirURL );
            aBaseURL.removeSegment();
            checkAndCreateDirectory( aBaseURL );
            return ::osl::Directory::create( rDirURL.GetMainURL( INetURLObject::DECODE_TO_IURI ) );
        }
        else
        {
            return aResult;
        }
    }       

    // -----------------------------------------------------------------------------

    void AutocorrectionMigration::copyFiles()
    {
        ::rtl::OUString sTargetDir;
        ::utl::Bootstrap::PathStatus aStatus = ::utl::Bootstrap::locateUserInstallation( sTargetDir );
        if ( aStatus == ::utl::Bootstrap::PATH_EXISTS )
        {
            sTargetDir += sTargetSubDir;
            TStringVectorPtr aFileList = getFiles( m_sSourceDir );
            TStringVector::const_iterator aI = aFileList->begin();
            while ( aI != aFileList->end() )
            {                
                ::rtl::OUString sSourceLocalName = aI->copy( m_sSourceDir.getLength() );
                sal_Int32 nStart = sBaseName.getLength();
                sal_Int32 nEnd = sSourceLocalName.lastIndexOf ( sSuffix );
                ::rtl::OUString sLanguageType = sSourceLocalName.copy( nStart, nEnd - nStart );
                ::rtl::OUString sIsoName = MsLangId::convertLanguageToIsoString( (LanguageType) sLanguageType.toInt32() );
                ::rtl::OUString sTargetLocalName = sBaseName;
                sTargetLocalName += ::rtl::OUString::createFromAscii( "_" );
                sTargetLocalName += sIsoName;
                sTargetLocalName += sSuffix;
                ::rtl::OUString sTargetName = sTargetDir + sTargetLocalName;
                INetURLObject aURL( sTargetName );
                aURL.removeSegment();
                checkAndCreateDirectory( aURL );            
                ::osl::FileBase::RC aResult = ::osl::File::copy( *aI, sTargetName );
                if ( aResult != ::osl::FileBase::E_None )
                {
                    ::rtl::OString aMsg( "AutocorrectionMigration::copyFiles: cannot copy " );
                    aMsg += ::rtl::OUStringToOString( *aI, RTL_TEXTENCODING_UTF8 ) + " to "
                         +  ::rtl::OUStringToOString( sTargetName, RTL_TEXTENCODING_UTF8 );
                    OSL_ENSURE( sal_False, aMsg.getStr() );
                }
                ++aI;
            }
        } 
        else
        {
            OSL_ENSURE( sal_False, "AutocorrectionMigration::copyFiles: no user installation!" );
        }
    }

    // -----------------------------------------------------------------------------
    // XServiceInfo
    // -----------------------------------------------------------------------------

    ::rtl::OUString AutocorrectionMigration::getImplementationName() throw (RuntimeException)
    {
        return AutocorrectionMigration_getImplementationName();
    }

    // -----------------------------------------------------------------------------

    sal_Bool AutocorrectionMigration::supportsService( const ::rtl::OUString& rServiceName ) throw (RuntimeException)
    {
	    Sequence< ::rtl::OUString > aNames( getSupportedServiceNames() );
	    const ::rtl::OUString* pNames = aNames.getConstArray();
	    const ::rtl::OUString* pEnd = pNames + aNames.getLength();
	    for ( ; pNames != pEnd && !pNames->equals( rServiceName ); ++pNames )
		    ;

	    return pNames != pEnd;
    }

    // -----------------------------------------------------------------------------

    Sequence< ::rtl::OUString > AutocorrectionMigration::getSupportedServiceNames() throw (RuntimeException)
    {
        return AutocorrectionMigration_getSupportedServiceNames();
    }

    // -----------------------------------------------------------------------------
    // XInitialization
    // -----------------------------------------------------------------------------

    void AutocorrectionMigration::initialize( const Sequence< Any >& aArguments ) throw (Exception, RuntimeException)
    {
        ::osl::MutexGuard aGuard( m_aMutex );

        const Any* pIter = aArguments.getConstArray();
        const Any* pEnd = pIter + aArguments.getLength();
        for ( ; pIter != pEnd ; ++pIter )
        {
            beans::NamedValue aValue;
            *pIter >>= aValue;
            if ( aValue.Name.equalsAscii( "UserData" ) )
            {
                if ( !(aValue.Value >>= m_sSourceDir) )
                {
                    OSL_ENSURE( false, "AutocorrectionMigration::initialize: argument UserData has wrong type!" );
                }
                m_sSourceDir += sSourceSubDir;
                break;
            }
        }
    }

    // -----------------------------------------------------------------------------
    // XJob
    // -----------------------------------------------------------------------------

    Any AutocorrectionMigration::execute( const Sequence< beans::NamedValue >& )
        throw (lang::IllegalArgumentException, Exception, RuntimeException)
    {
        ::osl::MutexGuard aGuard( m_aMutex );

        copyFiles();

        return Any();
    }

    // =============================================================================
    // component operations
    // =============================================================================

    Reference< XInterface > SAL_CALL AutocorrectionMigration_create(
        Reference< XComponentContext > const & )
        SAL_THROW( () )
    {
        return static_cast< lang::XTypeProvider * >( new AutocorrectionMigration() );
    }

    // -----------------------------------------------------------------------------

//.........................................................................
}	// namespace migration
//.........................................................................
