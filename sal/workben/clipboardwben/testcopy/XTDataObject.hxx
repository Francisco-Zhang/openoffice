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




#ifndef _XTDATAOBJECT_HXX_
#define _XTDATAOBJECT_HXX_


//------------------------------------------------------------------------
// includes
//------------------------------------------------------------------------

/*
#include <com/sun/star/datatransfer/XTransferable.hpp>
#include <com/sun/star/datatransfer/clipboard/XClipboardOwner.hpp>
#include "WinClipboard.hxx"
*/

#include <windows.h>
#include <ole2.h>
#include <objidl.h>


//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------

class EnumFormatEtc;

class CXTDataObject : public IDataObject
{
public:
	CXTDataObject( LONG nRefCntInitVal = 0);
    ~CXTDataObject( );

	//-----------------------------------------------------------------
	// ole interface implementation
	//-----------------------------------------------------------------

    //IUnknown 
    STDMETHODIMP           QueryInterface(REFIID iid, LPVOID* ppvObject);
    STDMETHODIMP_( ULONG ) AddRef( );
    STDMETHODIMP_( ULONG ) Release( );

    //IDataObject 
    STDMETHODIMP GetData( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium );
    STDMETHODIMP GetDataHere( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium );
    STDMETHODIMP QueryGetData( LPFORMATETC pFormatetc );
    STDMETHODIMP GetCanonicalFormatEtc( LPFORMATETC pFormatectIn, LPFORMATETC pFormatetcOut );
    STDMETHODIMP SetData( LPFORMATETC pFormatetc, LPSTGMEDIUM pmedium, BOOL fRelease );
    STDMETHODIMP EnumFormatEtc( DWORD dwDirection, IEnumFORMATETC** ppenumFormatetc );
    STDMETHODIMP DAdvise( LPFORMATETC pFormatetc, DWORD advf, LPADVISESINK pAdvSink, DWORD* pdwConnection );
    STDMETHODIMP DUnadvise( DWORD dwConnection );
    STDMETHODIMP EnumDAdvise( LPENUMSTATDATA* ppenumAdvise );

	operator IDataObject*( );

	// notification handler
	//void SAL_CALL LostOwnership( );

	//sal_Int64 SAL_CALL QueryDataSize( );

	// retrieve the data from the source
	// necessary so that 
	//void SAL_CALL GetAllDataFromSource( );

private:
	LONG m_nRefCnt;
	//CWinClipboard& m_rCWinClipboard;
	//const const ::com::sun::star::uno::Reference< ::com::sun::star::datatransfer::clipboard::XClipboardOwner >&  m_rXClipboardOwner;
	//const const ::com::sun::star::uno::Reference< ::com::sun::star::datatransfer::XTransferable >&               m_rXTDataSource;

	//friend class CWinClipboard;
	friend class CEnumFormatEtc;
};

//------------------------------------------------------------------------
// 
//------------------------------------------------------------------------

class CEnumFormatEtc : public IEnumFORMATETC
{
public:
	CEnumFormatEtc( LPUNKNOWN pUnkDataObj );
    ~CEnumFormatEtc( );

    // IUnknown 
    STDMETHODIMP           QueryInterface( REFIID iid, LPVOID* ppvObject );
    STDMETHODIMP_( ULONG ) AddRef( );
    STDMETHODIMP_( ULONG ) Release( );

    //IEnumFORMATETC 
    STDMETHODIMP Next( ULONG celt, LPFORMATETC rgelt, ULONG* pceltFetched );
    STDMETHODIMP Skip( ULONG celt );
    STDMETHODIMP Reset( );
    STDMETHODIMP Clone( IEnumFORMATETC** ppenum );

private:
	LONG		m_nRefCnt;
	LPUNKNOWN	m_pUnkDataObj;
    ULONG		m_nCurrentPos;
	CLIPFORMAT  m_cfFormats[2];
};


typedef CEnumFormatEtc *PCEnumFormatEtc;

#endif
