#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

// Type Definations

class DBRowIdDef
{
 unsigned short int FileNo;
 unsigned short int PageNo;
 int RecordNo;
};
//typedef struct DBRowIDdef DBRowIdDef;

class DBPageDef
{
 unsigned short int PageNo;
 unsigned short int PageSize;
 FILE * FilePointer; 
};
//typedef struct DBPagedef DBPageDef;

 

class DBFileDef
{
 char FileName[32];
 unsigned char FileNo;
 unsigned short int PageSize;
 unsigned short int No_of_Pages;
};
//typedef struct DBFiledef DBFileDef;


class DBDatabaseDef
{
 char DatabaseName[32];
 char MasterFile[32];
};
//typedef struct DBDatabasedef DBDatabaseDef;


//  Function Declarations


int DBCreateDatabase(DBDatabaseDef * ddd)
{

}

int DBCreateFile(DBFileDef * dfd)
{

}


int DBCreatePage(DBPageDef * dpd)
{

}

