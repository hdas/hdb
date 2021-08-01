// DQueryFactory.cpp: implementation of the DQueryFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "DQueryFactory.h"
#include "Token.h"

#include "DALTERQuery.h"
#include "DCREATEQuery.h"
#include "DDELETEQuery.h"
#include "DDESCQuery.h"
#include "DDROPQuery.h"
#include "DINSERTQuery.h"
#include "DSELECTQuery.h"
#include "DUPDATEQuery.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DQueryFactory::DQueryFactory()
{

}

DQueryFactory::~DQueryFactory()
{

}

DQuery * DQueryFactory::CreateQuery(DSession * ssn, char *sql)
{
	DQuery * qry = NULL;

	char * tt = CreateToken(sql, 0);

	if (EQUAL(tt, "DESC")) {
		qry = new DDESCQuery(ssn, sql);
	}
	else if (EQUAL(tt, "CREATE")) {
		qry = new DCREATEQuery(ssn, sql);
	}
	else if (EQUAL(tt, "DROP")) {
		qry = new DDROPQuery(ssn, sql);
	}
	else if (EQUAL(tt, "INSERT")) {
		qry = new DINSERTQuery(ssn, sql);
	}
	else if (EQUAL(tt, "DELETE")) {
		qry = new DDELETEQuery(ssn, sql);
	}
	else if (EQUAL(tt, "UPDATE")) {
		qry = new DUPDATEQuery(ssn, sql);
	}
	else if (EQUAL(tt, "SELECT")) {
		qry = new DSELECTQuery(ssn, sql);
	}

	return qry;
}
