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
#include "precompiled_svx.hxx"

#include <svx/svdovirt.hxx>
#include <svx/xpool.hxx>
#include <svx/svdtrans.hxx>
#include <svx/svdetc.hxx>
#include <svx/svdhdl.hxx>
#include <svx/sdr/contact/viewcontactofvirtobj.hxx>
#include <basegfx/matrix/b2dhommatrix.hxx>
#include <svx/svdograf.hxx>
#include <svx/svddrgv.hxx>
#include <basegfx/matrix/b2dhommatrixtools.hxx>

////////////////////////////////////////////////////////////////////////////////////////////////////

sdr::properties::BaseProperties& SdrVirtObj::GetProperties() const
{
	return rRefObj.GetProperties();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// AW, OD 2004-05-03 #i27224#
sdr::contact::ViewContact* SdrVirtObj::CreateObjectSpecificViewContact()
{
	return new sdr::contact::ViewContactOfVirtObj(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TYPEINIT1(SdrVirtObj,SdrObject);

SdrVirtObj::SdrVirtObj(SdrObject& rNewObj):
	rRefObj(rNewObj)
{
	bVirtObj=sal_True; // Ja, ich bin ein virtuelles Objekt
	rRefObj.AddReference(*this);
	bClosedObj=rRefObj.IsClosedObj();
}

SdrVirtObj::SdrVirtObj(SdrObject& rNewObj, const Point& rAnchorPos):
	rRefObj(rNewObj)
{
	aAnchor=rAnchorPos;
	bVirtObj=sal_True; // Ja, ich bin ein virtuelles Objekt
	rRefObj.AddReference(*this);
	bClosedObj=rRefObj.IsClosedObj();
}

SdrVirtObj::~SdrVirtObj()
{
	rRefObj.DelReference(*this);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const SdrObject& SdrVirtObj::GetReferencedObj() const
{
	return rRefObj;
}

SdrObject& SdrVirtObj::ReferencedObj()
{
	return rRefObj;
}

void __EXPORT SdrVirtObj::Notify(SfxBroadcaster& /*rBC*/, const SfxHint& /*rHint*/)
{
	bClosedObj=rRefObj.IsClosedObj();
	SetRectsDirty(); // hier noch Optimieren.

	// Only a repaint here, rRefObj may have changed and broadcasts
	ActionChanged();
	// BroadcastObjectChange();
}

void SdrVirtObj::NbcSetAnchorPos(const Point& rAnchorPos)
{
	aAnchor=rAnchorPos;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void SdrVirtObj::SetModel(SdrModel* pNewModel)
{
	SdrObject::SetModel(pNewModel);
	rRefObj.SetModel(pNewModel);
}

void SdrVirtObj::TakeObjInfo(SdrObjTransformInfoRec& rInfo) const
{
	rRefObj.TakeObjInfo(rInfo);
}

sal_uInt32 SdrVirtObj::GetObjInventor() const
{
	return rRefObj.GetObjInventor();
}

sal_uInt16 SdrVirtObj::GetObjIdentifier() const
{
	return rRefObj.GetObjIdentifier();
}

SdrObjList* SdrVirtObj::GetSubList() const
{
	return rRefObj.GetSubList();
}

const Rectangle& SdrVirtObj::GetCurrentBoundRect() const
{
	((SdrVirtObj*)this)->aOutRect=rRefObj.GetCurrentBoundRect(); // Hier noch optimieren
	((SdrVirtObj*)this)->aOutRect+=aAnchor;
	return aOutRect;
}

const Rectangle& SdrVirtObj::GetLastBoundRect() const
{
	((SdrVirtObj*)this)->aOutRect=rRefObj.GetLastBoundRect(); // Hier noch optimieren
	((SdrVirtObj*)this)->aOutRect+=aAnchor;
	return aOutRect;
}

void SdrVirtObj::RecalcBoundRect()
{
	aOutRect=rRefObj.GetCurrentBoundRect();
	aOutRect+=aAnchor;
}

void SdrVirtObj::SetChanged()
{
	SdrObject::SetChanged();
}

SdrObject* SdrVirtObj::Clone() const
{
	SdrObject* pObj=new SdrVirtObj(((SdrVirtObj*)this)->rRefObj); // Nur eine weitere Referenz
	return pObj;
}

void SdrVirtObj::operator=(const SdrObject& rObj)
{   // ???anderes Objekt referenzieren???
	SdrObject::operator=(rObj);
	aAnchor=((SdrVirtObj&)rObj).aAnchor;
}

void SdrVirtObj::TakeObjNameSingul(XubString& rName) const
{
	rRefObj.TakeObjNameSingul(rName);
	rName.Insert(sal_Unicode('['), 0);
	rName += sal_Unicode(']');

	String aName( GetName() );
	if(aName.Len())
	{
		rName += sal_Unicode(' ');
		rName += sal_Unicode('\'');
		rName += aName;
		rName += sal_Unicode('\'');
	}
}

void SdrVirtObj::TakeObjNamePlural(XubString& rName) const
{
	rRefObj.TakeObjNamePlural(rName);
	rName.Insert(sal_Unicode('['), 0);
	rName += sal_Unicode(']');
}

void operator +=(PolyPolygon& rPoly, const Point& rOfs)
{
	if (rOfs.X()!=0 || rOfs.Y()!=0) {
		sal_uInt16 i,j;
		for (j=0; j<rPoly.Count(); j++) {
			Polygon aP1(rPoly.GetObject(j));
			for (i=0; i<aP1.GetSize(); i++) {
				 aP1[i]+=rOfs;
			}
			rPoly.Replace(aP1,j);
		}
	}
}

basegfx::B2DPolyPolygon SdrVirtObj::TakeXorPoly() const
{
	basegfx::B2DPolyPolygon aPolyPolygon(rRefObj.TakeXorPoly());

	if(aAnchor.X() || aAnchor.Y())
	{
        aPolyPolygon.transform(basegfx::tools::createTranslateB2DHomMatrix(aAnchor.X(), aAnchor.Y()));
	}

	return aPolyPolygon;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

sal_uInt32 SdrVirtObj::GetHdlCount() const
{
	return rRefObj.GetHdlCount();
}

SdrHdl* SdrVirtObj::GetHdl(sal_uInt32 nHdlNum) const
{
	SdrHdl* pHdl=rRefObj.GetHdl(nHdlNum);

	// #i73248#
	// GetHdl() at SdrObject is not guaranteed to return an object
	if(pHdl)
	{
		Point aP(pHdl->GetPos()+aAnchor);
		pHdl->SetPos(aP);
	}

	return pHdl;
}

sal_uInt32 SdrVirtObj::GetPlusHdlCount(const SdrHdl& rHdl) const
{
	return rRefObj.GetPlusHdlCount(rHdl);
}

SdrHdl* SdrVirtObj::GetPlusHdl(const SdrHdl& rHdl, sal_uInt32 nPlNum) const
{
	SdrHdl* pHdl=rRefObj.GetPlusHdl(rHdl,nPlNum);
	pHdl->SetPos(pHdl->GetPos() + aAnchor);
	return pHdl;
}

void SdrVirtObj::AddToHdlList(SdrHdlList& rHdlList) const
{
	// #i73248#
	// SdrObject::AddToHdlList(rHdlList) is not a good thing to call
	// since at SdrPathObj, only AddToHdlList may be used and the call
	// will instead use the standard implementation which uses GetHdlCount()
	// and GetHdl instead. This is not wrong, but may be much less effective
	// and may not be prepared to GetHdl returning NULL

	// get handles using AddToHdlList from ref object
	SdrHdlList aLocalList(0);
	rRefObj.AddToHdlList(aLocalList);
	const sal_uInt32 nHdlCount(aLocalList.GetHdlCount());

	if(nHdlCount)
	{
		// translate handles and add them to dest list. They are temporarily part of
		// two lists then
		const Point aOffset(GetOffset());

		for(sal_uInt32 a(0L); a < nHdlCount; a++)
		{
			SdrHdl* pCandidate = aLocalList.GetHdl(a);
			pCandidate->SetPos(pCandidate->GetPos() + aOffset);
			rHdlList.AddHdl(pCandidate);
		}

		// remove them from source list, else they will be deleted when
		// source list is deleted
		while(aLocalList.GetHdlCount())
		{
			aLocalList.RemoveHdl(aLocalList.GetHdlCount() - 1L);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool SdrVirtObj::hasSpecialDrag() const
{
	return rRefObj.hasSpecialDrag();
}

bool SdrVirtObj::supportsFullDrag() const
{
    return false;
}

SdrObject* SdrVirtObj::getFullDragClone() const
{
    static bool bSpecialHandling(false);
    SdrObject* pRetval = 0;

    if(bSpecialHandling)
    {
	    // special handling for VirtObj. Do not create another
	    // reference to rRefObj, this would allow to change that
	    // one on drag. Instead, create a SdrGrafObj for drag containing
	    // the graphical representation
	    pRetval = new SdrGrafObj(SdrDragView::GetObjGraphic(GetModel(), this), GetLogicRect());
    }
    else
    {
        SdrObject& rReferencedObject = ((SdrVirtObj*)this)->ReferencedObj();
	    pRetval = new SdrGrafObj(SdrDragView::GetObjGraphic(GetModel(), &rReferencedObject), GetLogicRect());
    }

	return pRetval;
}

bool SdrVirtObj::beginSpecialDrag(SdrDragStat& rDrag) const
{
	return rRefObj.beginSpecialDrag(rDrag);
}

bool SdrVirtObj::applySpecialDrag(SdrDragStat& rDrag)
{
	return rRefObj.applySpecialDrag(rDrag);
}

basegfx::B2DPolyPolygon SdrVirtObj::getSpecialDragPoly(const SdrDragStat& rDrag) const
{
	return rRefObj.getSpecialDragPoly(rDrag);
	// Offset handlen !!!!!! fehlt noch !!!!!!!
}

String SdrVirtObj::getSpecialDragComment(const SdrDragStat& rDrag) const
{
	return rRefObj.getSpecialDragComment(rDrag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

FASTBOOL SdrVirtObj::BegCreate(SdrDragStat& rStat)
{
	return rRefObj.BegCreate(rStat);
}

FASTBOOL SdrVirtObj::MovCreate(SdrDragStat& rStat)
{
	return rRefObj.MovCreate(rStat);
}

FASTBOOL SdrVirtObj::EndCreate(SdrDragStat& rStat, SdrCreateCmd eCmd)
{
	return rRefObj.EndCreate(rStat,eCmd);
}

FASTBOOL SdrVirtObj::BckCreate(SdrDragStat& rStat)
{
	return rRefObj.BckCreate(rStat);
}

void SdrVirtObj::BrkCreate(SdrDragStat& rStat)
{
	rRefObj.BrkCreate(rStat);
}

basegfx::B2DPolyPolygon SdrVirtObj::TakeCreatePoly(const SdrDragStat& rDrag) const
{
	return rRefObj.TakeCreatePoly(rDrag);
	// Offset handlen !!!!!! fehlt noch !!!!!!!
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void SdrVirtObj::NbcMove(const Size& rSiz)
{
	MovePoint(aAnchor,rSiz);
	SetRectsDirty();
}

void SdrVirtObj::NbcResize(const Point& rRef, const Fraction& xFact, const Fraction& yFact)
{
	rRefObj.NbcResize(rRef-aAnchor,xFact,yFact);
	SetRectsDirty();
}

void SdrVirtObj::NbcRotate(const Point& rRef, long nWink, double sn, double cs)
{
	rRefObj.NbcRotate(rRef-aAnchor,nWink,sn,cs);
	SetRectsDirty();
}

void SdrVirtObj::NbcMirror(const Point& rRef1, const Point& rRef2)
{
	rRefObj.NbcMirror(rRef1-aAnchor,rRef2-aAnchor);
	SetRectsDirty();
}

void SdrVirtObj::NbcShear(const Point& rRef, long nWink, double tn, FASTBOOL bVShear)
{
	rRefObj.NbcShear(rRef-aAnchor,nWink,tn,bVShear);
	SetRectsDirty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void SdrVirtObj::Move(const Size& rSiz)
{
	if (rSiz.Width()!=0 || rSiz.Height()!=0) {
		Rectangle aBoundRect0; if (pUserCall!=NULL) aBoundRect0=GetLastBoundRect();
		// #110094#-14 SendRepaintBroadcast();
		NbcMove(rSiz);
		SetChanged();
		BroadcastObjectChange();
		SendUserCall(SDRUSERCALL_MOVEONLY,aBoundRect0);
	}
}

void SdrVirtObj::Resize(const Point& rRef, const Fraction& xFact, const Fraction& yFact)
{
	if (xFact.GetNumerator()!=xFact.GetDenominator() || yFact.GetNumerator()!=yFact.GetDenominator()) {
		Rectangle aBoundRect0; if (pUserCall!=NULL) aBoundRect0=GetLastBoundRect();
		rRefObj.Resize(rRef-aAnchor,xFact,yFact);
		SetRectsDirty();
		SendUserCall(SDRUSERCALL_RESIZE,aBoundRect0);
	}
}

void SdrVirtObj::Rotate(const Point& rRef, long nWink, double sn, double cs)
{
	if (nWink!=0) {
		Rectangle aBoundRect0; if (pUserCall!=NULL) aBoundRect0=GetLastBoundRect();
		rRefObj.Rotate(rRef-aAnchor,nWink,sn,cs);
		SetRectsDirty();
		SendUserCall(SDRUSERCALL_RESIZE,aBoundRect0);
	}
}

void SdrVirtObj::Mirror(const Point& rRef1, const Point& rRef2)
{
	Rectangle aBoundRect0; if (pUserCall!=NULL) aBoundRect0=GetLastBoundRect();
	rRefObj.Mirror(rRef1-aAnchor,rRef2-aAnchor);
	SetRectsDirty();
	SendUserCall(SDRUSERCALL_RESIZE,aBoundRect0);
}

void SdrVirtObj::Shear(const Point& rRef, long nWink, double tn, FASTBOOL bVShear)
{
	if (nWink!=0) {
		Rectangle aBoundRect0; if (pUserCall!=NULL) aBoundRect0=GetLastBoundRect();
		rRefObj.Shear(rRef-aAnchor,nWink,tn,bVShear);
		SetRectsDirty();
		SendUserCall(SDRUSERCALL_RESIZE,aBoundRect0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void SdrVirtObj::RecalcSnapRect()
{
	aSnapRect=rRefObj.GetSnapRect();
	aSnapRect+=aAnchor;
}

const Rectangle& SdrVirtObj::GetSnapRect() const
{
	((SdrVirtObj*)this)->aSnapRect=rRefObj.GetSnapRect();
	((SdrVirtObj*)this)->aSnapRect+=aAnchor;
	return aSnapRect;
}

void SdrVirtObj::SetSnapRect(const Rectangle& rRect)
{
	{
		Rectangle aBoundRect0; if (pUserCall!=NULL) aBoundRect0=GetLastBoundRect();
		Rectangle aR(rRect);
		aR-=aAnchor;
		rRefObj.SetSnapRect(aR);
		SetRectsDirty();
		SendUserCall(SDRUSERCALL_RESIZE,aBoundRect0);
	}
}

void SdrVirtObj::NbcSetSnapRect(const Rectangle& rRect)
{
	Rectangle aR(rRect);
	aR-=aAnchor;
	SetRectsDirty();
	rRefObj.NbcSetSnapRect(aR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Rectangle& SdrVirtObj::GetLogicRect() const
{
	((SdrVirtObj*)this)->aSnapRect=rRefObj.GetLogicRect();  // !!! Missbrauch von aSnapRect !!!
	((SdrVirtObj*)this)->aSnapRect+=aAnchor;                // Wenns mal Aerger gibt, muss ein weiteres Rectangle Member her (oder Heap)
	return aSnapRect;
}

void SdrVirtObj::SetLogicRect(const Rectangle& rRect)
{
	Rectangle aBoundRect0; if (pUserCall!=NULL) aBoundRect0=GetLastBoundRect();
	Rectangle aR(rRect);
	aR-=aAnchor;
	rRefObj.SetLogicRect(aR);
	SetRectsDirty();
	SendUserCall(SDRUSERCALL_RESIZE,aBoundRect0);
}

void SdrVirtObj::NbcSetLogicRect(const Rectangle& rRect)
{
	Rectangle aR(rRect);
	aR-=aAnchor;
	SetRectsDirty();
	rRefObj.NbcSetLogicRect(aR);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

long SdrVirtObj::GetRotateAngle() const
{
	return rRefObj.GetRotateAngle();
}

long SdrVirtObj::GetShearAngle(FASTBOOL bVertical) const
{
	return rRefObj.GetShearAngle(bVertical);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

sal_uInt32 SdrVirtObj::GetSnapPointCount() const
{
	return rRefObj.GetSnapPointCount();
}

Point SdrVirtObj::GetSnapPoint(sal_uInt32 i) const
{
	Point aP(rRefObj.GetSnapPoint(i));
	aP+=aAnchor;
	return aP;
}

sal_Bool SdrVirtObj::IsPolyObj() const
{
	return rRefObj.IsPolyObj();
}

sal_uInt32 SdrVirtObj::GetPointCount() const
{
	return rRefObj.GetPointCount();
}

Point SdrVirtObj::GetPoint(sal_uInt32 i) const
{
	return Point(rRefObj.GetPoint(i) + aAnchor);
}

void SdrVirtObj::NbcSetPoint(const Point& rPnt, sal_uInt32 i)
{
	Point aP(rPnt);
	aP-=aAnchor;
	rRefObj.SetPoint(aP,i);
	SetRectsDirty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

SdrObjGeoData* SdrVirtObj::NewGeoData() const
{
	return rRefObj.NewGeoData();
}

void SdrVirtObj::SaveGeoData(SdrObjGeoData& rGeo) const
{
	rRefObj.SaveGeoData(rGeo);
}

void SdrVirtObj::RestGeoData(const SdrObjGeoData& rGeo)
{
	rRefObj.RestGeoData(rGeo);
	SetRectsDirty();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

SdrObjGeoData* SdrVirtObj::GetGeoData() const
{
	return rRefObj.GetGeoData();
}

void SdrVirtObj::SetGeoData(const SdrObjGeoData& rGeo)
{
	Rectangle aBoundRect0; if (pUserCall!=NULL) aBoundRect0=GetLastBoundRect();
	rRefObj.SetGeoData(rGeo);
	SetRectsDirty();
	SendUserCall(SDRUSERCALL_RESIZE,aBoundRect0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void SdrVirtObj::NbcReformatText()
{
	rRefObj.NbcReformatText();
}

void SdrVirtObj::ReformatText()
{
	rRefObj.ReformatText();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

FASTBOOL SdrVirtObj::HasMacro() const
{
	return rRefObj.HasMacro();
}

SdrObject* SdrVirtObj::CheckMacroHit(const SdrObjMacroHitRec& rRec) const
{
	return rRefObj.CheckMacroHit(rRec); // Todo: Positionsversatz
}

Pointer SdrVirtObj::GetMacroPointer(const SdrObjMacroHitRec& rRec) const
{
	return rRefObj.GetMacroPointer(rRec); // Todo: Positionsversatz
}

void SdrVirtObj::PaintMacro(OutputDevice& rOut, const Rectangle& rDirtyRect, const SdrObjMacroHitRec& rRec) const
{
	rRefObj.PaintMacro(rOut,rDirtyRect,rRec); // Todo: Positionsversatz
}

FASTBOOL SdrVirtObj::DoMacro(const SdrObjMacroHitRec& rRec)
{
	return rRefObj.DoMacro(rRec); // Todo: Positionsversatz
}

XubString SdrVirtObj::GetMacroPopupComment(const SdrObjMacroHitRec& rRec) const
{
	return rRefObj.GetMacroPopupComment(rRec); // Todo: Positionsversatz
}

const Point SdrVirtObj::GetOffset() const
{
	// #i73248# default offset of SdrVirtObj is aAnchor
    return aAnchor;
}

// eof
