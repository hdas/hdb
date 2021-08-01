# Microsoft Developer Studio Project File - Name="db" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=db - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "db.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "db.mak" CFG="db - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "db - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "db - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "db - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /Op /D "WIN32" /D "NDEBUG" /D "_MBCS" /YX /FD /c
# SUBTRACT CPP /u
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /machine:I386 /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib libc.lib" /out:"Release/hdb.exe"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "db - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /MT /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# SUBTRACT CPP /u
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib advapi32.lib /nologo /subsystem:console /pdb:none /debug /machine:I386 /out:"Debug/hdb.exe"
# SUBTRACT LINK32 /map /nodefaultlib

!ENDIF 

# Begin Target

# Name "db - Win32 Release"
# Name "db - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DAggr.cpp
# End Source File
# Begin Source File

SOURCE=.\DALTERQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DArray.cpp
# End Source File
# Begin Source File

SOURCE=.\db.cpp
# End Source File
# Begin Source File

SOURCE=.\DConditionalQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DCreateQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DData.cpp
# End Source File
# Begin Source File

SOURCE=.\DDataArray.cpp
# End Source File
# Begin Source File

SOURCE=.\DDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DDDLQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DDeleteQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DDESCQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DDMLQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DDropQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DExpr.cpp
# End Source File
# Begin Source File

SOURCE=.\DGrouperStreamDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DIndex.cpp
# End Source File
# Begin Source File

SOURCE=.\DInsertQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DMemDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DQueryFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\DReadableDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DResult.cpp
# End Source File
# Begin Source File

SOURCE=.\DRow.cpp
# End Source File
# Begin Source File

SOURCE=.\DSchema.cpp
# End Source File
# Begin Source File

SOURCE=.\DSchemaArray.cpp
# End Source File
# Begin Source File

SOURCE=.\DSelectQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DSession.cpp
# End Source File
# Begin Source File

SOURCE=.\DSorterStreamDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DStack.cpp
# End Source File
# Begin Source File

SOURCE=.\DStreamDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DTable.cpp
# End Source File
# Begin Source File

SOURCE=.\DTableDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DUpdateQuery.cpp
# End Source File
# Begin Source File

SOURCE=.\DVar.cpp
# End Source File
# Begin Source File

SOURCE=.\DVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\DView.cpp
# End Source File
# Begin Source File

SOURCE=.\DViewDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DWriteableDatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\hdbio.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\service.cpp
# End Source File
# Begin Source File

SOURCE=.\sql.cpp
# End Source File
# Begin Source File

SOURCE=.\token.cpp
# End Source File
# Begin Source File

SOURCE=.\where.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DAggr.h
# End Source File
# Begin Source File

SOURCE=.\DALTERQuery.h
# End Source File
# Begin Source File

SOURCE=.\DArray.h
# End Source File
# Begin Source File

SOURCE=.\db.h
# End Source File
# Begin Source File

SOURCE=.\DConditionalQuery.h
# End Source File
# Begin Source File

SOURCE=.\DCreateQuery.h
# End Source File
# Begin Source File

SOURCE=.\DData.h
# End Source File
# Begin Source File

SOURCE=.\DDataArray.h
# End Source File
# Begin Source File

SOURCE=.\DDatrix.h
# End Source File
# Begin Source File

SOURCE=.\DDDLQuery.h
# End Source File
# Begin Source File

SOURCE=.\DDeleteQuery.h
# End Source File
# Begin Source File

SOURCE=.\DDESCQuery.h
# End Source File
# Begin Source File

SOURCE=.\DDmlQuery.h
# End Source File
# Begin Source File

SOURCE=.\DDropQuery.h
# End Source File
# Begin Source File

SOURCE=.\DExpr.h
# End Source File
# Begin Source File

SOURCE=.\DGrouperStreamDatrix.h
# End Source File
# Begin Source File

SOURCE=.\DIndex.h
# End Source File
# Begin Source File

SOURCE=.\DInsertQuery.h
# End Source File
# Begin Source File

SOURCE=.\DMemDatrix.h
# End Source File
# Begin Source File

SOURCE=.\DQuery.h
# End Source File
# Begin Source File

SOURCE=.\DQueryFactory.h
# End Source File
# Begin Source File

SOURCE=.\DReadableDatrix.h
# End Source File
# Begin Source File

SOURCE=.\DResult.h
# End Source File
# Begin Source File

SOURCE=.\DRow.h
# End Source File
# Begin Source File

SOURCE=.\DSchema.h
# End Source File
# Begin Source File

SOURCE=.\DSchemaArray.h
# End Source File
# Begin Source File

SOURCE=.\DSelectQuery.h
# End Source File
# Begin Source File

SOURCE=.\DSession.h
# End Source File
# Begin Source File

SOURCE=.\DSorterStreamDatrix.h
# End Source File
# Begin Source File

SOURCE=.\DStack.h
# End Source File
# Begin Source File

SOURCE=.\DStreamDatrix.h
# End Source File
# Begin Source File

SOURCE=.\DTable.h
# End Source File
# Begin Source File

SOURCE=.\DTableDatrix.h
# End Source File
# Begin Source File

SOURCE=.\DUpdateQuery.h
# End Source File
# Begin Source File

SOURCE=.\DVar.h
# End Source File
# Begin Source File

SOURCE=.\DVariable.h
# End Source File
# Begin Source File

SOURCE=.\DView.h
# End Source File
# Begin Source File

SOURCE=.\DViewDatrix.h
# End Source File
# Begin Source File

SOURCE=.\DWriteableDatrix.h
# End Source File
# Begin Source File

SOURCE=.\hdbio.h
# End Source File
# Begin Source File

SOURCE=.\perser.h
# End Source File
# Begin Source File

SOURCE=.\service.H
# End Source File
# Begin Source File

SOURCE=.\sort.h
# End Source File
# Begin Source File

SOURCE=.\sql.h
# End Source File
# Begin Source File

SOURCE=.\token.h
# End Source File
# Begin Source File

SOURCE=.\where.h
# End Source File
# End Group
# End Target
# End Project
