//----- CBaseDocReactor.cpp : Implementation of CBaseDocReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CBaseDocReactor.h"
#include "BaseUtils.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(CBaseDocReactor, AcApDocManagerReactor, 1)
extern void attachDbReactorToAll();
extern void removeDbReactorFromAll();

//-----------------------------------------------------------------------------
CBaseDocReactor::CBaseDocReactor (const bool autoInitAndRelease) : AcApDocManagerReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acDocManager )
			acDocManager->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
CBaseDocReactor::~CBaseDocReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CBaseDocReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acDocManager ) {
			acDocManager->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void CBaseDocReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acDocManager ) {
			acDocManager->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcApDocManager *CBaseDocReactor::Subject () const {
	return (acDocManager) ;
}

bool CBaseDocReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}


void CBaseDocReactor::documentCreateStarted(AcApDocument* pDocCreating)
{

}

void CBaseDocReactor::documentCreated(AcApDocument* pDocCreating)
{
}

void CBaseDocReactor::documentToBeDestroyed(AcApDocument* pDocToDestroy)
{
	removeDbReactorFromAll();
}

void CBaseDocReactor::documentDestroyed(const ACHAR* fileName)
{
	
}

void CBaseDocReactor::documentCreateCanceled(AcApDocument* pDocCreateCancelled)
{

}

// -----------------------------------------------------------------------------
void CBaseDocReactor::documentLockModeWillChange(AcApDocument * param2, AcAp::DocLockMode myCurrentMode, AcAp::DocLockMode myNewMode, AcAp::DocLockMode currentMode, const ACHAR * pGlobalCmdName)
{
	AcApDocManagerReactor::documentLockModeWillChange (param2, myCurrentMode, myNewMode, currentMode, pGlobalCmdName) ;
}

// -----------------------------------------------------------------------------
void CBaseDocReactor::documentBecameCurrent(AcApDocument * param2)
{
	AcApDocManagerReactor::documentBecameCurrent (param2) ;
	//acDocManager->sendStringToExecute(param2, _T("test5 "));
}

// -----------------------------------------------------------------------------
void CBaseDocReactor::documentToBeActivated(AcApDocument * pActivatingDoc)
{
	//acedCommand(RTSTR, _T("RIBBONCLOSE"), RTNONE);
#ifndef ARX2014
#ifndef ZRXSDK
	CString strCur;
	TCHAR strRet[256];
	MyBaseUtils::GetVar(_T("WSCURRENT"), strRet);
	strCur = strRet;
	if (strCur.CompareNoCase(_T("AutoCAD ¾­µä")) != 0)
	{
		acedCommand(RTSTR, _T("WSCURRENT"), RTSTR, _T("AutoCAD ¾­µä"), RTNONE);
	}

	int nCur = 0;
	MyBaseUtils::GetVar(_T("RIBBONSTATE"), &nCur);	

	if (nCur != 0)
	{
		acedCommand(RTSTR, _T("RIBBONCLOSE"), RTNONE);
	}
#endif
#endif
	AcApDocManagerReactor::documentToBeActivated (pActivatingDoc) ;
}

// -----------------------------------------------------------------------------
void CBaseDocReactor::documentToBeDeactivated(AcApDocument * pDeActivatedDoc)
{
	//acedCommand(RTSTR, _T("RIBBONCLOSE"), RTNONE);

	AcApDocManagerReactor::documentToBeDeactivated (pDeActivatedDoc) ;
}

// -----------------------------------------------------------------------------
void CBaseDocReactor::documentActivationModified(bool bActivation)
{
	AcApDocManagerReactor::documentActivationModified (bActivation) ;
}

// -----------------------------------------------------------------------------
void CBaseDocReactor::documentActivated(AcApDocument * pActivatedDoc)
{
	CBaseUtils::setJxbFlag(false);
	attachDbReactorToAll();
}

// -----------------------------------------------------------------------------
Acad::ErrorStatus CBaseDocReactor::veto(void)
{
	Acad::ErrorStatus retCode =AcApDocManagerReactor::veto () ;
	return (retCode) ;
}
