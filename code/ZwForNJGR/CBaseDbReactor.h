//-----------------------------------------------------------------------------
//----- CBaseDbReactor.h : Declaration of the CBaseDbReactor
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
#include "dbmain.h"

//-----------------------------------------------------------------------------
//----- Note: Uncomment the DLLIMPEXP symbol below if you wish exporting
//----- your class to other ARX/DBX modules
class /*DLLIMPEXP*/ CBaseDbReactor : public AcDbDatabaseReactor {

public:
	ACRX_DECLARE_MEMBERS(CBaseDbReactor) ;

protected:
	//----- Pointer to the document this reactor instance belongs to.
	AcDbDatabase *mpDatabase ;

public:
	CBaseDbReactor (AcDbDatabase *pDb =NULL) ;
	virtual ~CBaseDbReactor () ;

	virtual void Attach (AcDbDatabase *pDb) ;
	virtual void Detach () ;
	virtual AcDbDatabase *Subject () const ;
	virtual bool IsAttached () const ;

	// -----------------------------------------------------------------------------
	virtual void proxyResurrectionCompleted(const AcDbDatabase * dwg, const ACHAR * appname, AcDbObjectIdArray & objects);
	// -----------------------------------------------------------------------------
	virtual void goodbye(const AcDbDatabase * dwg);
	// -----------------------------------------------------------------------------
	virtual void headerSysVarWillChange(const AcDbDatabase * dwg, const ACHAR * name);
	// -----------------------------------------------------------------------------
	virtual void headerSysVarChanged(const AcDbDatabase * dwg, const ACHAR * name, Adesk::Boolean bSuccess);
	// -----------------------------------------------------------------------------
	virtual void objectAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj);
	// -----------------------------------------------------------------------------
	virtual void objectUnAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj);
	// -----------------------------------------------------------------------------
	virtual void objectReAppended(const AcDbDatabase * dwg, const AcDbObject * dbObj);
	// -----------------------------------------------------------------------------
	virtual void objectOpenedForModify(const AcDbDatabase * dwg, const AcDbObject * dbObj);
	// -----------------------------------------------------------------------------
	virtual void objectModified(const AcDbDatabase * dwg, const AcDbObject * dbObj);
	// -----------------------------------------------------------------------------
	virtual void objectErased(const AcDbDatabase * dwg, const AcDbObject * dbObj, Adesk::Boolean pErased);
	// -----------------------------------------------------------------------------
	static AcDbDatabaseReactor * cast(void);
} ;

#ifdef WRQ_REACTOR_MOUDLE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(CBaseDbReactor)
#endif
