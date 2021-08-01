// DCreateQuery.cpp: implementation of the DCreateQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "DCreateQuery.h"
#include "DQueryFactory.h"
#include "DSession.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DCREATEQuery::DCREATEQuery(DSession *ssn, char * sql) : DDDLQuery(ssn, sql)
{

}

DCREATEQuery::~DCREATEQuery()
{

}

int DCREATEQuery::Parse()
{
	return TRUE;
}

int DCREATEQuery::Execute()
{
	if (EQUAL(m_stl[1], "TABLE"))
	{
		return ExecuteCreateTable();
	}
	else if (EQUAL(m_stl[1], "INDEX"))
	{
		return ExecuteCreateIndex();
	}
	else if (EQUAL(m_stl[1], "VIEW"))
	{
		return FALSE;
	}

	return FALSE;
}

int DCREATEQuery::ExecuteCreateIndex()
{
	char idxname[TABLE_NAME_WIDTH], tname[TABLE_NAME_WIDTH], cat_sql[MAX_CHAR_LENGTH];
	unsigned short numf = 0;
	int i, retcd = false, res = true;
	tagDBField * ff;
	DQuery * qry;
	DTable * tbl;
	//	DBIndex * idx;
	int tn = 3, brac = 0, fi = 0;

	strcpy(idxname, m_stl[2]);

	if (!EQUAL(m_stl[3], "ON")) HDB_RETURN(ERR_BADSQL);

	strcpy(tname, m_stl[4]);

	for (tn = 5;; tn++)
	{
		if (EQUAL(m_stl[tn], "(")) brac++;
		if (EQUAL(m_stl[tn], ")")) brac--;
		if (brac == 1 && EQUAL(m_stl[tn], ",")) numf++;
		if (brac == 0 || EQUAL(m_stl[tn], "") || EQUAL(m_stl[tn], ";")){ numf++; break; }
		//if(brac==1 && EQUAL(qry->m_stl[tn], "CONSTRAINT")) break;
	}

	ff = (tagDBField *)malloc(numf*sizeof(tagDBField));

	tbl = new DTable(m_ssn, tname);
	tbl->Open();

	brac = 0;
	fi = 0;

	char ** pFieldNames = (char**)malloc(numf * sizeof(char*));
	for (i = 0; i < numf; i++)
	{
		char * fldName = m_stl[6 + i * 2];
		int len = strlen(fldName);
		pFieldNames[i] = (char*)malloc(len + 1);
		strcpy(pFieldNames[i], fldName);
	}

	if (true)
	{
		strcpy(cat_sql, "Select * From Cat Where ONAME='");
		strcat(cat_sql, idxname);
		strcat(cat_sql, "';");

		qry = DQueryFactory::CreateQuery(m_ssn, cat_sql);

		if (qry->Parse())
			qry->Execute();

		if (qry->GetResult()->m_nRow>0)
		{
			delete qry;
			HDB_RETURN(ERR_ALREADYEXIST);
		}

		delete qry;
	}

	res = tbl->CreateIndex(pFieldNames, numf, idxname);
	if (res == true)
	{
		strcpy(cat_sql, "insert into cat values('");
		strcat(cat_sql, idxname);
		strcat(cat_sql, "','INDEX');");

		qry = DQueryFactory::CreateQuery(m_ssn, cat_sql);

		if (qry->Parse())
			qry->Execute();

		delete qry;
	}
	retcd = true;
	strcpy(m_result->m_errmsg, "Index Created");

CLEANUP:
	if (ff != NULL) free(ff);
	tbl->Close();
	delete tbl;
	m_result->m_errcd = retcd;
	return TRUE;
}

int DCREATEQuery::ExecuteCreateTable(BOOL bCheckCat)
{
	char tname[TABLE_NAME_WIDTH], cat_sql[MAX_CHAR_LENGTH];
	unsigned short numf = 0, cur_pos_offset = 0;
	int retcd = false, res = true;
	tagDBField * ff = NULL;
	//DResult * tmp_result=NULL;
	DQuery * qry = NULL;
	int tn = 3, brac = 0, fi = 0;


	// result must be created first; beacause it store at list the return code

	// the number of token must be 9 or more
	if (m_nToken < 9) HDB_RETURN(ERR_BADSQL);

	if (strlen(m_stl[2]) >= TABLE_NAME_WIDTH) HDB_RETURN(ERR_LARGE_NAME);

	if (dGetReserveWordCode(m_stl[2])) HDB_RETURN(ERR_RW_USED);

	strcpy(tname, m_stl[2]);

	if (bCheckCat)
	{
		// Now checking if the table/view/synonym already exist or not
		strcpy(cat_sql, "Select OTYPE From CAT Where ONAME='");
		strcat(cat_sql, tname); strcat(cat_sql, "';");

		qry = DQueryFactory::CreateQuery(m_ssn, cat_sql);

		if (qry->Parse() > 0)
		{
			qry->Execute();
		}
		else
		{
			DCREATEQuery * cqry = (DCREATEQuery*)DQueryFactory::CreateQuery(m_ssn, "create table cat(oname varchar(64), otype varchar(64));");
			if (cqry->Parse() > 0)
			{
				cqry->ExecuteCreateTable(FALSE);
			}
			delete cqry;
		}

		if (qry->GetResult()->m_nRow > 0)
		{
			delete qry;
			HDB_RETURN(ERR_ALREADYEXIST);
		}

		delete qry;
		qry = NULL;
	}

	for (tn = 3;; tn++)
	{
		if (EQUAL(m_stl[tn], "(")) brac++;
		if (EQUAL(m_stl[tn], ")")) brac--;
		if (brac == 1 && EQUAL(m_stl[tn], ",")) numf++;
		if (brac == 0 || EQUAL(m_stl[tn], "") || EQUAL(m_stl[tn], ";")){ numf++; break; }
		if (brac == 1 && EQUAL(m_stl[tn], "CONSTRAINT")) break;
	}

	ff = (tagDBField *)malloc(numf*sizeof(tagDBField));

	brac = 0;
	fi = 0;
	cur_pos_offset = 1;

	for (tn = 4;;)
	{
		ff[fi].m_field_decimal = 0;
		ff[fi].m_is_null = 'Y';
		ff[fi].m_field_length = 8; // for date

		if (tn >= m_nToken) { res = ERR_BADSQL; break; }
		if (strlen(m_stl[tn]) >= FIELD_NAME_WIDTH) { res = ERR_LARGE_NAME; break; }
		if (dGetReserveWordCode(m_stl[tn])) { res = ERR_RW_USED; break; }
		strncpy(ff[fi].m_field_name, m_stl[tn], FIELD_NAME_WIDTH - 1);  ff[fi].m_field_name[FIELD_NAME_WIDTH - 1] = 0;
		tn++;
		if (tn >= m_nToken) { res = ERR_BADSQL; break; }
		ff[fi].m_field_type = dGetDataTypeCode(m_stl[tn]);
		if (!ff[fi].m_field_type) { res = ERR_BADDATATYPE; break; } // beacause it is boolean
		if (ff[fi].m_field_type == DT_DATETIME) { ff[fi].m_field_length = 8; ff[fi].m_field_decimal = 0; }
		if (ff[fi].m_field_type == DT_CHAR) { ff[fi].m_field_length = 1; ff[fi].m_field_decimal = 0; }
		if (ff[fi].m_field_type == DT_NUMBER) { ff[fi].m_field_length = 9; ff[fi].m_field_decimal = 0; }

		tn++;
		if (tn >= m_nToken) { res = ERR_BADSQL; break; }
		if (EQUAL(m_stl[tn], "("))
		{
			brac++;
			tn++;
			if (tn >= m_nToken) { res = ERR_BADSQL; break; }
			ff[fi].m_field_length = atoi(m_stl[tn++]);
			if (tn >= m_nToken) { res = ERR_BADSQL; break; }
			if (EQUAL(m_stl[tn], ","))
			{
				tn++;
				if (tn >= m_nToken) { res = ERR_BADSQL; break; }
				ff[fi].m_field_decimal = atoi(m_stl[tn++]);
			}
			if (tn >= m_nToken) { res = ERR_BADSQL; break; }
			if (EQUAL(m_stl[tn], ")"))
			{
				tn++;
				brac--;
			}
		}

		// field length calculation done
		// now setting the pos_offset
		ff[fi].m_pos_offset = cur_pos_offset;
		cur_pos_offset += ff[fi].m_field_length;

		// Not Null Checking
		if (EQUAL(m_stl[tn], "NOT") && EQUAL(m_stl[tn + 1], "NULL"))
		{
			ff[fi].m_is_null = 'N';
		}
		if (tn >= m_nToken) { res = ERR_BADSQL; break; }
		if ((fi == numf - 1) || EQUAL(m_stl[tn], "")) break;
		while (fi < numf - 1) // loop until ','
		{
			if (EQUAL(m_stl[tn], ","))
			{
				fi++;
				tn++;
				break;
			}
			tn++;
			if (tn >= m_nToken) { res = ERR_BADSQL; break; }
		}
		if (EQUAL(m_stl[tn], ")"))
		{
			if (brac == 0) fi++;
			tn++;
		}
		if (tn >= m_nToken) { res = ERR_BADSQL; break; }
		if ((fi == numf) || EQUAL(m_stl[tn], "")) break;
	}

	if (res != true) HDB_RETURN(res);

	DTable * tbl = new DTable(m_ssn, tname);

	res = tbl->CreateTable(ff, numf);
	if (res == true)
	{
		strcpy(cat_sql, "insert into cat values('");
		strcat(cat_sql, tname);
		strcat(cat_sql, "','TABLE');");
		qry = DQueryFactory::CreateQuery(m_ssn, cat_sql);
		if (!(qry->Parse() && qry->Execute()))
		{
			strcpy(m_result->m_errmsg, qry->GetResult()->m_errmsg);
			HDB_RETURN(qry->GetResult()->m_errcd);
		}

		delete qry;
	}
	retcd = true;

	delete tbl;

	strcpy(m_result->m_errmsg, "Table Created");

CLEANUP:
	if (ff != NULL)
		free(ff);

	m_errcd = m_result->m_errcd = retcd;

	if (m_errcd == SUCCESS)
		return TRUE;
	else
		return FALSE;
}
