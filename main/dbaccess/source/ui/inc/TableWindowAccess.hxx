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


#ifndef DBACCESS_TABLEWINDOWACCESS_HXX
#define DBACCESS_TABLEWINDOWACCESS_HXX

#ifndef _COM_SUN_STAR_ACCESSIBILITY_XACCESSIBLERELATIONSET_HPP_
#include <com/sun/star/accessibility/XAccessibleRelationSet.hpp>
#endif
#ifndef _CPPUHELPER_IMPLBASE2_HXX_
#include <cppuhelper/implbase2.hxx>
#endif
#ifndef _TOOLKIT_AWT_VCLXACCESSIBLECOMPONENT_HXX_
#include <toolkit/awt/vclxaccessiblecomponent.hxx>
#endif

namespace dbaui
{
	typedef ::cppu::ImplHelper2< ::com::sun::star::accessibility::XAccessibleRelationSet,
 								 ::com::sun::star::accessibility::XAccessible
											> OTableWindowAccess_BASE;
	class OTableWindow;
	/** the class OTableWindowAccess represents the accessible object for table windows
		like they are used in the QueryDesign and the RelationDesign
	*/
	class OTableWindowAccess	:	public VCLXAccessibleComponent
								,	public OTableWindowAccess_BASE
	{
		OTableWindow*	m_pTable; // the window which I should give accessibility to

		::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > getParentChild(sal_Int32 _nIndex);
	protected:
		/** this function is called upon disposing the component
		*/
		virtual void SAL_CALL disposing();

		/** isEditable returns the current editable state
			@return	true if it is editable otherwise false				
		*/
		virtual sal_Bool isEditable() const;

        virtual void ProcessWindowEvent( const VclWindowEvent& rVclWindowEvent );
	public:
		OTableWindowAccess(	OTableWindow* _pTable);

		// XInterface
		virtual ::com::sun::star::uno::Any SAL_CALL queryInterface( const ::com::sun::star::uno::Type& aType ) throw (::com::sun::star::uno::RuntimeException);
		virtual void SAL_CALL acquire(  ) throw ()
		{ // here inline is allowed because we do not use this class outside this dll
			VCLXAccessibleComponent::acquire(  );
		}
		virtual void SAL_CALL release(  ) throw ()
		{ // here inline is allowed because we do not use this class outside this dll
			VCLXAccessibleComponent::release(  );
		}

		// XTypeProvider
		virtual ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Type > SAL_CALL getTypes(  ) throw (::com::sun::star::uno::RuntimeException);

		// XServiceInfo
		virtual ::rtl::OUString SAL_CALL getImplementationName() throw(com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::uno::Sequence< ::rtl::OUString > SAL_CALL getSupportedServiceNames() throw(com::sun::star::uno::RuntimeException);

		// XServiceInfo - static methods
		static com::sun::star::uno::Sequence< ::rtl::OUString > getSupportedServiceNames_Static(void) throw( com::sun::star::uno::RuntimeException );
		static ::rtl::OUString getImplementationName_Static(void) throw( com::sun::star::uno::RuntimeException );

		// XAccessible
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessibleContext > SAL_CALL getAccessibleContext(  ) throw (::com::sun::star::uno::RuntimeException);
		
		// XAccessibleContext
		virtual sal_Int32 SAL_CALL getAccessibleChildCount(  ) throw (::com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL getAccessibleChild( sal_Int32 i ) throw (::com::sun::star::lang::IndexOutOfBoundsException,::com::sun::star::uno::RuntimeException);
		virtual sal_Int32 SAL_CALL getAccessibleIndexInParent(  ) throw (::com::sun::star::uno::RuntimeException);
		virtual sal_Int16 SAL_CALL getAccessibleRole(  ) throw (::com::sun::star::uno::RuntimeException);
		virtual ::rtl::OUString SAL_CALL getAccessibleName(  ) throw (::com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessibleRelationSet > SAL_CALL getAccessibleRelationSet(  ) throw (::com::sun::star::uno::RuntimeException);

		// XAccessibleComponent
		virtual ::com::sun::star::uno::Reference< ::com::sun::star::accessibility::XAccessible > SAL_CALL getAccessibleAtPoint( const ::com::sun::star::awt::Point& aPoint ) throw (::com::sun::star::uno::RuntimeException);

		// XAccessibleExtendedComponent
		virtual ::rtl::OUString SAL_CALL getTitledBorderText(  ) throw (::com::sun::star::uno::RuntimeException);

		// XAccessibleRelationSet
		virtual sal_Int32 SAL_CALL getRelationCount(  ) throw (::com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::accessibility::AccessibleRelation SAL_CALL getRelation( sal_Int32 nIndex ) throw (::com::sun::star::lang::IndexOutOfBoundsException, ::com::sun::star::uno::RuntimeException);
		virtual sal_Bool SAL_CALL containsRelation( sal_Int16 aRelationType ) throw (::com::sun::star::uno::RuntimeException);
		virtual ::com::sun::star::accessibility::AccessibleRelation SAL_CALL getRelationByType( sal_Int16 aRelationType ) throw (::com::sun::star::uno::RuntimeException);

		void notifyAccessibleEvent(
					const sal_Int16 _nEventId,
					const ::com::sun::star::uno::Any& _rOldValue,
					const ::com::sun::star::uno::Any& _rNewValue
				)
		{
			NotifyAccessibleEvent(_nEventId,_rOldValue,_rNewValue);
		}
	};
}
#endif // DBACCESS_TABLEWINDOWACCESS_HXX
