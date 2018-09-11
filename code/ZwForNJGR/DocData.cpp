//-----------------------------------------------------------------------------
//----- DocData.cpp : Implementation file
//-----------------------------------------------------------------------------
#include "StdAfx.h"

//-----------------------------------------------------------------------------
//----- The one and only document manager object. You can use the DocVars object to retrieve
//----- document specific data throughout your application
AcApDataManager<CDocData> DocVars ;

//-----------------------------------------------------------------------------
//----- Implementation of the document data class.
CDocData::CDocData () {
}

//-----------------------------------------------------------------------------
CDocData::CDocData (const CDocData &data) {
}

//-----------------------------------------------------------------------------
CDocData::~CDocData () {
}
