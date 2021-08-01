// DResult.cpp: implementation of the DResult class.
//
//////////////////////////////////////////////////////////////////////

#include "DVariable.h"
#include "DResult.h"
#include "DRow.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DResult::DResult()
{
	this->Create();
}

DResult::~DResult()
{
	this->Close();
}

int DResult::Output()
{
	DRow * crow = NULL;
	int i, l;
	unsigned int ci;

	//HDB_ASSERT(result!=NULL);

	// Checking for error
	if (m_errcd != true)
	{
		printf("\nError : %d  ", m_errcd);
		printf("%s\n\n", m_errmsg);
		return true;
	}
	if (m_nCol > 0)
	{
		printf("\n");
		// displaying field captions
		for (ci = 0; ci < m_nCol; ci++)
		{
			//char * fcap = new char(m_FieldList[ci].m_field_length + 1); //(char*)malloc((m_FieldList[ci].m_field_length + 1)* sizeof(char));
			int lbb = m_FieldList[ci].m_field_length + 1;
			char * fcap = (char*)malloc(sizeof(char)*lbb);
			strncpy(fcap, m_FieldList[ci].m_field_name, m_FieldList[ci].m_field_length);
			fcap[m_FieldList[ci].m_field_length] = 0;
			rpad(fcap, m_FieldList[ci].m_field_length);
			printf(" %s", fcap);
			free(fcap);
		}
		printf("\n");
		for (ci = 0; ci < m_nCol; ci++)
		{
			char * border;
			int jj, lbb = m_FieldList[ci].m_field_length + 1;
			border = (char*)malloc(sizeof(char)*lbb);
			border[lbb - 1] = 0; // terminating char
			for (jj = 0; jj < (lbb - 1); jj++) border[jj] = '-';
			printf(" %s", border);
			free(border);
		}
		printf("\n");
	}

	crow = m_FirstRow->m_NextRow; // beacause first arValue is NULL
	// displaying result
	while (crow != NULL)
	{
		for (ci = 0; ci < m_nCol; ci++)
		{
			char * dummy = (char*)malloc((1 + m_FieldList[ci].m_field_length) * sizeof(char));
			l = strlen(crow->m_arValues[ci]);
			for (i = 0; i < ((int)(m_FieldList[ci].m_field_length) - l); i++) dummy[i] = 32; dummy[i] = 0;
			printf(" %s%s", crow->m_arValues[ci], dummy);
			free(dummy);
		}
		printf("\n");
		crow = crow->m_NextRow;
	}

	printf("\n");
	if (m_SqlType == SQL_DML) printf("%s : %ld Row(s).", m_errmsg, m_nRow);
	else //if(m_Sql_Type == SQL_DDL)
		printf("%s.", m_errmsg);

	printf("\n\n");


	return true;
}

int DResult::Close()
{
	DRow * crow = NULL, *tmprow = NULL;
	unsigned int ci = 0;

	//	if(result==NULL) return true;
	crow = m_FirstRow->m_NextRow; // beacause first arValue is NULL
	while (crow != NULL)
	{
		for (ci = 0; ci < m_nCol; ci++) if (crow->m_arValues[ci] != NULL) free(crow->m_arValues[ci]);
		free(crow->m_arValues);
		tmprow = crow;
		crow = crow->m_NextRow;
		free(tmprow);
	}
	free(m_FirstRow);
	free(m_FieldList);
	//	free(result);
	return true;
}

int DResult::Create()
{
	m_errcd = FALSE;
	strcpy(m_errmsg, "");
	m_FirstRow = new DRow(); // (DRow*)malloc(sizeof(DBRow));
	HDB_ASSERT(m_FirstRow != NULL);
	m_FirstRow->m_NextRow = NULL;
	m_FirstRow->m_arValues = NULL;
	m_FieldList = NULL;
	m_cRow = m_FirstRow;
	m_SqlType = SQL_DML;
	m_nCol = 0;
	m_nRow = 0;
	return SUCCESS;
}

DRow* DResult::AddRow()
{
	unsigned int ci = 0;
	m_cRow->m_NextRow = new DRow(); //(DBRow*)malloc(sizeof(DBRow));
	m_cRow = m_cRow->m_NextRow;
	m_cRow->m_NextRow = NULL;
	m_cRow->m_arValues = (char**)malloc(m_nCol * sizeof(char*));
	for (ci = 0; ci < m_nCol; ci++) m_cRow->m_arValues[ci] = NULL;
	m_nRow++;
	return m_cRow;
}

int DResult::SortResult(int *iSortOnArr, int nSortOnArr, int Direction)
{
	int son = 0, styp = 0, retcd, res;
	DVariable * vrst = NULL, *var1 = NULL, *var2 = NULL;
	DRow * row1 = NULL, *row2 = NULL;
	char * boolstr;

	//HDB_ASSERT(pResult != NULL);

	if (nSortOnArr != 1) HDB_RETURN(ERR_NOTIMPL);

	son = iSortOnArr[0];
	styp = m_FieldList[son].m_field_type;

	if (m_nRow == 1) return true;

	m_cRow = m_FirstRow->m_NextRow;

	row1 = m_FirstRow->m_NextRow;

	var1 = new DVariable();
	var1->Store(row1->m_arValues[son], styp);

	/*row1 =m_FirstRow;

	if(sCompareRow(row1->NextRow, row1->NextRow->NextRow) < 0)
	{
	tmprow = row1->NextRow;
	row1->NextRow = row1->NextRow->NextRow;

	}*/



	for (;;)
	{
		m_cRow = m_cRow->m_NextRow;
		if (m_cRow == NULL) break;

		var2 = new DVariable();
		var2->Store(m_cRow->m_arValues[son], styp);
		var2->ConvertTo(styp);

		vrst = DExpr::PerformOparation(var2, var1, OP_LT, &res);

		boolstr = vrst->GetStringValue();
		if (EQUAL(boolstr, "true"))
		{
			row1 = m_cRow;
			delete var1;
			var1 = new DVariable();
			var1->Store(row1->m_arValues[son], styp);
			var1->ConvertTo(styp);
		}
		free(boolstr);
		delete var2;
	}

CLEANUP:
	return true;
}
