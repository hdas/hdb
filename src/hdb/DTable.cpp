// DTable.cpp: implementation of the DTable class.
//
//////////////////////////////////////////////////////////////////////

#include <direct.h>
#include "DTable.h"
#include "DSession.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DTable::DTable(DSession * pSsn, char * TableName)
{
	m_tablename[0] = 0x00;
	m_username[0] = 0x00;
	m_fp = NULL;
	m_pHeader = NULL;
	m_pSsn = NULL;
	m_Cached = 0;
	m_nFields = 0;
	m_pTagFields = NULL;
	m_abspos = 0;
	m_Locked = FALSE; // bool; determine if locked or unlocked; and lockid must greater than 0 (zero)
	m_LockId = 0; // depends on locked; has some id (>0) which is also the lockid of Session by which this table is locked 
	m_fp_buf = NULL;

	strcpy(m_tablename, TableName);
	strcpy(m_username, pSsn->GetUserName());
}

DTable::~DTable()
{

}

int DTable::Open()
{
	char fname[FILE_NAME_WIDTH] = "", fl_extn[6];
	int nfld = 0;

	m_Cached = true;
	m_Locked = false;
	m_LockId = 0;

	strcpy(fl_extn, ".tab");
	sprintf(fname, "%s%s%s%s%s%s", global_opt.DatabasePath, DIR_SEP, m_username, DIR_SEP, m_tablename, fl_extn);
	m_fp = fopen(fname, "rb+");
	if (m_fp == NULL)
	{
		return FAILURE;
	}

	m_fp_buf = (char*)malloc(1);
	//setvbuf(fp, fp_buf, _IOFBF, 65536);

	m_pHeader = (struct tagDBHeader*)malloc(sizeof(struct tagDBHeader));
	fread(m_pHeader, sizeof(struct tagDBHeader), 1, m_fp);

	nfld = m_pHeader->m_nFields;

	m_nFields = nfld;

	m_pTagFields = (struct tagDBField*)malloc(nfld*sizeof(struct tagDBField));
	//m_pFieldList = new [nfld] m_pTagFields();
	fread(m_pTagFields, sizeof(struct tagDBField), nfld, m_fp);
	m_abspos = sizeof(struct tagDBHeader) + nfld * sizeof(struct tagDBField) + 1; // last one for XBASE field list identity
	return SUCCESS;
}

int DTable::Close()
{
	time_t ltime;
	struct tm * ttm;

	time(&ltime);
	ttm = localtime(&ltime);

	if (m_pHeader)
	{
		m_pHeader->m_last_update[0] = (char)ttm->tm_year - 100;
		m_pHeader->m_last_update[1] = (char)ttm->tm_mon + 1;
		m_pHeader->m_last_update[2] = (char)ttm->tm_mday + 1;

		if (m_fp)
		{
			fseek(m_fp, 0, SEEK_SET);
			fwrite(m_pHeader, sizeof(struct tagDBHeader), 1, m_fp);
			fclose(m_fp);
			free(m_fp_buf);
		}
		free(m_pHeader);
	}

	if (m_pTagFields)
		free(m_pTagFields);

	return true;
}

int DTable::GetFieldCount()
{
	return m_nFields;
}

tagDBField * DTable::GetTagField(char * fieldName)
{
	tagDBField * tbFld = NULL;
	int fldcd = this->FieldCode(fieldName);
	if (fldcd >= 0 && fldcd < m_nFields)
	{
		tbFld = &m_pTagFields[fldcd];
	}
	return tbFld;
}

tagDBField * DTable::GetTagField(int fldCode)
{
	tagDBField * tbFld = NULL;
	if (fldCode >= 0 && fldCode < m_nFields)
	{
		tbFld = &m_pTagFields[fldCode];
	}
	return tbFld;
}

int DTable::FieldCode(char *FieldName)
{
	int i;
	for (i = 0; i < m_nFields; i++)
	{
		if (strcmp(FieldName, (m_pTagFields + i)->m_field_name) == 0) return i;
	}
	return ERR_BADFIELD;
}

int DTable::FieldSize(int FieldCode)
{
	if (FieldCode >= m_nFields) return ERR_BADFIELD;
	return (m_pTagFields + FieldCode)->m_field_length;
}

char DTable::FieldType(int FieldCode)
{
	if (FieldCode >= m_nFields || FieldCode < 0) return (char)ERR_BADFIELD;
	return m_pTagFields[FieldCode].m_field_type;
}

int DTable::FirstRecord()
{
	char flag;
	m_abspos = sizeof(tagDBHeader) + m_nFields * sizeof(tagDBField) + 1;
	if (fseek(m_fp, m_abspos, SEEK_SET) != 0) return ERR_POSNOTSET;
	fread(&flag, 1, 1, m_fp);

	if (feof(m_fp) || flag == FLAG_EOF)
		return ERR_EOF;

	if (flag == FLAG_DELETED_RECORD)
		return NextRecord();

	return true;
}

int DTable::NextRecord()
{
	char flag = FLAG_DELETED_RECORD;
	while (flag == FLAG_DELETED_RECORD)
	{
		m_abspos += m_pHeader->m_record_size;
		if (fseek(m_fp, m_abspos, SEEK_SET) != 0) return ERR_POSNOTSET;
		fread(&flag, 1, 1, m_fp);
		if (feof(m_fp)) return ERR_EOF;
	}
	return true;
}

char * DTable::FetchField(int FieldCode, int * retcd)
{
	//int i; //,offset=1; // initial value of offset is 1 because of flag byte
	char * Value, *vv;

	Value = (char*)malloc((m_pTagFields + FieldCode)->m_field_length + 1);
	//for(i=0;i<FieldCode;i++) offset += (m_pTagFields +i)->m_field_length;
	if (fseek(m_fp, m_abspos + m_pTagFields[FieldCode].m_pos_offset, SEEK_SET) != 0)
	{
		free(Value);
		*retcd = ERR_POSNOTSET;
		return NULL;
	}
	
	if (fread(Value, 1, m_pTagFields[FieldCode].m_field_length, m_fp) != m_pTagFields[FieldCode].m_field_length)
	{
		free(Value);
		*retcd = ERR_NOREAD;
		return NULL;
	}
	
	Value[m_pTagFields[FieldCode].m_field_length] = 0; // terminating character
	vv = (char*)malloc(strlen(Value) + 1);
	strcpy(vv, Value);
	free(Value);
	*retcd = true;
	return vv;
}

int DTable::RestoreField(int FieldCode, char * Value)
{
	//int i,offset=1; // initial value of offset is 1 because of flag byte
	char * tbuf = (char*)malloc(m_pTagFields[FieldCode].m_field_length);

	if (strlen(Value) > m_pTagFields[FieldCode].m_field_length)
	{
		free(tbuf);
		return ERR_LARGE_VALUE;
	}

	memset(tbuf, 0x00, m_pTagFields[FieldCode].m_field_length);
	strncpy(tbuf, Value, m_pTagFields[FieldCode].m_field_length);

	if (fseek(m_fp, m_abspos + m_pTagFields[FieldCode].m_pos_offset, SEEK_SET) != 0)
	{
		free(tbuf);
		return ERR_POSNOTSET;
	}

	if (fwrite(tbuf, 1, m_pTagFields[FieldCode].m_field_length, m_fp) != m_pTagFields[FieldCode].m_field_length)
	{
		free(tbuf);
		return ERR_NOWRITE;
	}
	free(tbuf);

	return true;
}

int DTable::AppendRecord()
{
	char * buf = (char *)malloc(m_pHeader->m_record_size);

	memset(buf, 0x00, m_pHeader->m_record_size);

	buf[0] = FLAG_VALID_RECORD;

	m_abspos = m_pHeader->m_header_size + m_pHeader->m_record_size * m_pHeader->m_no_of_record;
	fseek(m_fp, m_abspos, SEEK_SET);
	/*fseek(fp, 0, SEEK_END);
	if(abspos != ftell(fp))
	{
	free(buf);
	return ERR_POSNOTSET;
	}*/

	if (fwrite(buf, 1, m_pHeader->m_record_size, m_fp) != m_pHeader->m_record_size)
	{
		free(buf);
		return ERR_NOWRITE;
	}
	free(buf);

	m_pHeader->m_no_of_record++;
	return true;
}

int DTable::DeleteRecord()
{
	char flag = FLAG_DELETED_RECORD; // 0x00 means recored is deleted

	if (fseek(m_fp, m_abspos, SEEK_SET) != 0) return ERR_POSNOTSET;
	if (fwrite(&flag, 1, 1, m_fp) != 1) return ERR_NOWRITE;

	return true;
}

int DTable::Lock()
{
	while ((m_Locked == true) || (m_LockId > 0))
	{
		Sleep(20);
	}
	m_Locked = true;
	m_LockId = m_pSsn->Lock(this);
	return true;
}

int DTable::Unlock()
{
	if (m_pSsn->Unlock(this) == SUCCESS)
	{
		m_Locked = false;
		m_LockId = 0;
	}
	return true;
}

int DTable::CreateIndex(char ** pFieldNameList, unsigned short nFields, char *IndexName)
{
	FILE * fp;
	char fname[FILE_NAME_WIDTH], fl_extn[6];
	int i;
	char flg = 0x0D;
	tagDBIdxHeader hdr;

	hdr.m_record_size = 1; // initialize ; 1 for a flag byte

	tagDBField * pFieldArray = (tagDBField*)malloc(nFields * sizeof(tagDBField));

	for (i = 0; i < nFields; i++)
	{
		int fldcd = this->FieldCode(pFieldNameList[i]);
		hdr.m_record_size += this->m_pTagFields[fldcd].m_field_length;

		memcpy((pFieldArray + i), &m_pTagFields[fldcd], sizeof(tagDBField));
	}

	hdr.m_nFields = nFields;
	hdr.m_header_size = (unsigned short)(sizeof(tagDBIdxHeader) + nFields * sizeof(tagDBField) + 1);
	strcpy(hdr.m_tname, m_tablename);
	strcpy(hdr.m_idxname, IndexName);

	strcpy(fl_extn, ".ndx");
	sprintf(fname, "%s%s%s%s%s%s", global_opt.DatabasePath, DIR_SEP, m_pSsn->GetUserName(), DIR_SEP, IndexName, fl_extn);
	fp = fopen(fname, "wb");
	if (fp == NULL) return ERR_BADINDEX;

	fwrite(&hdr, sizeof(tagDBIdxHeader), 1, fp);
	fwrite(pFieldArray, sizeof(tagDBField), nFields, fp);

	fwrite(&flg, 1, 1, fp); // XBASE Compatible
	fclose(fp);
	return true;
}

int DTable::CreateTable(tagDBField *FieldList, unsigned short nFields)
{
	FILE * fp;
	char fname[FILE_NAME_WIDTH], fl_extn[6];
	tagDBHeader hdr;
	int i;
	char flg = 0x0D;
	time_t ltime;
	struct tm * ttm;

	memset((void*)&hdr, 0x00, sizeof(tagDBHeader));

	time(&ltime);
	ttm = localtime(&ltime);
	hdr.m_last_update[0] = (char)ttm->tm_year - 100;
	hdr.m_last_update[1] = (char)ttm->tm_mon + 1;
	hdr.m_last_update[2] = (char)ttm->tm_mday + 1;


	hdr.m_identity = 0x03;
	hdr.m_no_of_record = 0;
	hdr.m_language_driver_id = 0x03;

	hdr.m_record_size = 1; // initialize ; 1 for a flag byte
	for (i = 0; i < nFields; i++) hdr.m_record_size += FieldList[i].m_field_length;

	hdr.m_nFields = nFields;
	hdr.m_header_size = (unsigned short)(sizeof(tagDBHeader) + nFields * sizeof(tagDBField) + 1);

	strcpy(fl_extn, ".tab");

	char dirname[MAX_PATH];
	sprintf(dirname, "%s%s%s", global_opt.DatabasePath, DIR_SEP, m_username);
	sprintf(fname, "%s%s%s%s", dirname, DIR_SEP, m_tablename, fl_extn);

	fp = fopen(fname, "wb");
	if (fp == NULL) {
		int lerr = ::GetLastError();
		if (lerr == ERROR_PATH_NOT_FOUND) {
			if (_mkdir(dirname) == 0) {
				fp = fopen(fname, "wb");
			}
		}
		if (fp == NULL) {
			return ERR_BADTABLE;
		}
	}

	fwrite(&hdr, sizeof(tagDBHeader), 1, fp);
	fwrite(FieldList, sizeof(tagDBField), nFields, fp);

	fwrite(&flg, 1, 1, fp);
	fclose(fp);
	return true;
}
