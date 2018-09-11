//-----------------------------------------------------------------------------
//----- CBaseDocReactor.h : Declaration of the CBaseDocReactor
//-----------------------------------------------------------------------------
#pragma once

#ifdef ZWFORNJGR_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else
//----- Note: we don't use __declspec(dllimport) here, because of the
//----- "local vtable" problem with msvc. If you use __declspec(dllimport),
//----- then, when a client dll does a new on the class, the object's
//----- vtable pointer points to a vtable allocated in that client
//----- dll. If the client dll then passes the object to another dll,
//----- and the client dll is then unloaded, the vtable becomes invalid
//----- and any virtual calls on the object will access invalid memory.
//-----
//----- By not using __declspec(dllimport), we guarantee that the
//----- vtable is allocated in the server dll during the ctor and the
//----- client dll does not overwrite the vtable pointer after calling
//----- the ctor. And, since we expect the server dll to remain in
//----- memory indefinitely, there is no problem with vtables unexpectedly
//----- going away.
#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "acdocman.h"

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ CBaseDocReactor : public AcApDocManagerReactor {

public:
	ACRX_DECLARE_MEMBERS(CBaseDocReactor) ;

protected:
	//----- Auto initialization and release flag.
	bool mbAutoInitAndRelease ;

public:
	CBaseDocReactor (const bool autoInitAndRelease =true) ;
	virtual ~CBaseDocReactor () ;

	virtual void Attach () ;
	virtual void Detach () ;
	virtual AcApDocManager *Subject () const ;
	virtual bool IsAttached () const ;

	virtual void              documentCreateStarted(AcApDocument* pDocCreating);
	// Fired when a document has been successfully created replaces kLoadDwgMsg.
	virtual void              documentCreated(AcApDocument* pDocCreating);
	// Fired when a document is about to be destroyed replaces kUnloadDwgMsg
	virtual void              documentToBeDestroyed(AcApDocument* pDocToDestroy);
	// Fired when a document is completely destroyed
	virtual void              documentDestroyed(const ACHAR* fileName);
	// Fired when a user action has cancelled a documents creation.
	// Only fired in MDI mode and after a documentCreateStarted
	virtual void              documentCreateCanceled(AcApDocument* pDocCreateCancelled);
	// -----------------------------------------------------------------------------
	virtual void documentLockModeWillChange(AcApDocument * param2, AcAp::DocLockMode myCurrentMode, AcAp::DocLockMode myNewMode, AcAp::DocLockMode currentMode, const ACHAR * pGlobalCmdName);
	// -----------------------------------------------------------------------------
	virtual void documentBecameCurrent(AcApDocument * param2);
	// -----------------------------------------------------------------------------
	virtual void documentToBeActivated(AcApDocument * pActivatingDoc);
	// -----------------------------------------------------------------------------
	virtual void documentToBeDeactivated(AcApDocument * pDeActivatedDoc);
	// -----------------------------------------------------------------------------
	virtual void documentActivationModified(bool bActivation);
	// -----------------------------------------------------------------------------
	virtual void documentActivated(AcApDocument * pActivatedDoc);
protected:
	// -----------------------------------------------------------------------------
	Acad::ErrorStatus veto(void);
} ;

#ifdef ZWFORNJGR_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CBaseDocReactor)
#endif
