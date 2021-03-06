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


#ifndef DBAUI_QUERYORDER_HXX
#define DBAUI_QUERYORDER_HXX

#ifndef _DIALOG_HXX //autogen
#include <vcl/dialog.hxx>
#endif

#ifndef _LSTBOX_HXX //autogen
#include <vcl/lstbox.hxx>
#endif

#ifndef _EDIT_HXX //autogen
#include <vcl/edit.hxx>
#endif

#ifndef _FIXED_HXX //autogen
#include <vcl/fixed.hxx>
#endif

#ifndef _BUTTON_HXX //autogen
#include <vcl/button.hxx>
#endif

#define DOG_ROWS	3

namespace rtl
{
	class OUString;
}
namespace com
{
	namespace sun
	{
		namespace star
		{
			namespace sdb
			{
				class XSingleSelectQueryComposer;
			}
			namespace sdbc
			{
				class XConnection;
			}
			namespace container
			{
				class XNameAccess;
			}
			namespace beans
			{
				struct PropertyValue;
				class XPropertySet;
			}
		}
	}
}

//==================================================================
// DlgOrderCrit
//==================================================================
namespace dbaui
{
	class DlgOrderCrit : public ModalDialog
	{
	protected:
		ListBox			aLB_ORDERFIELD1;
		ListBox			aLB_ORDERVALUE1;
		ListBox			aLB_ORDERFIELD2;
		ListBox			aLB_ORDERVALUE2;
		ListBox			aLB_ORDERFIELD3;
		ListBox			aLB_ORDERVALUE3;
		FixedText		aFT_ORDERFIELD;
		FixedText		aFT_ORDERAFTER1;
		FixedText		aFT_ORDERAFTER2;
		FixedText		aFT_ORDEROPER;
		FixedText		aFT_ORDERDIR;
		OKButton		aBT_OK;
		CancelButton	aBT_CANCEL;
		HelpButton		aBT_HELP;
        FixedLine       aFL_ORDER;
		String			aSTR_NOENTRY;
		::rtl::OUString	m_sOrgOrder;

		::com::sun::star::uno::Reference< ::com::sun::star::sdb::XSingleSelectQueryComposer> m_xQueryComposer;
		::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess>	m_xColumns;
		::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>		m_xConnection;


		ListBox*		m_aColumnList[DOG_ROWS];
		ListBox*		m_aValueList[DOG_ROWS];

		DECL_LINK( FieldListSelectHdl, ListBox * );
		void			EnableLines();

	public:
		DlgOrderCrit(	Window * pParent,
						const ::com::sun::star::uno::Reference< ::com::sun::star::sdbc::XConnection>& _rxConnection,
						const ::com::sun::star::uno::Reference< ::com::sun::star::sdb::XSingleSelectQueryComposer>& _rxComposer,
						const ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess>& _rxCols);

						~DlgOrderCrit();
		void			BuildOrderPart();

		::rtl::OUString	GetOrderList( ) const;
		::rtl::OUString	GetOrignalOrder() const { return m_sOrgOrder; }

    private:
        void            impl_initializeOrderList_nothrow();
	};
}
#endif // DBAUI_QUERYORDER_HXX


