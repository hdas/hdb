# Microsoft Developer Studio Generated NMAKE File, Based on db.dsp
!IF "$(CFG)" == ""
CFG=db - Win32 Debug
!MESSAGE No configuration specified. Defaulting to db - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "db - Win32 Release" && "$(CFG)" != "db - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "db - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\hdb.exe"


CLEAN :
	-@erase "$(INTDIR)\aggr.obj"
	-@erase "$(INTDIR)\db.obj"
	-@erase "$(INTDIR)\expr.obj"
	-@erase "$(INTDIR)\hdbio.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\perser.obj"
	-@erase "$(INTDIR)\select.obj"
	-@erase "$(INTDIR)\service.obj"
	-@erase "$(INTDIR)\sort.obj"
	-@erase "$(INTDIR)\sql.obj"
	-@erase "$(INTDIR)\token.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\where.obj"
	-@erase "$(OUTDIR)\hdb.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /MT /W3 /GX /O2 /Op /D "WIN32" /D "NDEBUG" /D "_MBCS" /Fp"$(INTDIR)\db.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\db.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\hdb.pdb" /machine:I386 /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib libc.lib" /out:"$(OUTDIR)\hdb.exe" 
LINK32_OBJS= \
	"$(INTDIR)\aggr.obj" \
	"$(INTDIR)\db.obj" \
	"$(INTDIR)\expr.obj" \
	"$(INTDIR)\hdbio.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\perser.obj" \
	"$(INTDIR)\select.obj" \
	"$(INTDIR)\service.obj" \
	"$(INTDIR)\sort.obj" \
	"$(INTDIR)\sql.obj" \
	"$(INTDIR)\token.obj" \
	"$(INTDIR)\where.obj"

"$(OUTDIR)\hdb.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "db - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\hdb.exe"


CLEAN :
	-@erase "$(INTDIR)\aggr.obj"
	-@erase "$(INTDIR)\db.obj"
	-@erase "$(INTDIR)\expr.obj"
	-@erase "$(INTDIR)\hdbio.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\perser.obj"
	-@erase "$(INTDIR)\select.obj"
	-@erase "$(INTDIR)\service.obj"
	-@erase "$(INTDIR)\sort.obj"
	-@erase "$(INTDIR)\sql.obj"
	-@erase "$(INTDIR)\token.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\where.obj"
	-@erase "$(OUTDIR)\hdb.exe"
	-@erase "$(OUTDIR)\hdb.ilk"
	-@erase "$(OUTDIR)\hdb.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /G5 /MT /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\db.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\db.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\hdb.pdb" /debug /machine:I386 /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /out:"$(OUTDIR)\hdb.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\aggr.obj" \
	"$(INTDIR)\db.obj" \
	"$(INTDIR)\expr.obj" \
	"$(INTDIR)\hdbio.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\perser.obj" \
	"$(INTDIR)\select.obj" \
	"$(INTDIR)\service.obj" \
	"$(INTDIR)\sort.obj" \
	"$(INTDIR)\sql.obj" \
	"$(INTDIR)\token.obj" \
	"$(INTDIR)\where.obj"

"$(OUTDIR)\hdb.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("db.dep")
!INCLUDE "db.dep"
!ELSE 
!MESSAGE Warning: cannot find "db.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "db - Win32 Release" || "$(CFG)" == "db - Win32 Debug"
SOURCE=.\aggr.c

"$(INTDIR)\aggr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\db.c

"$(INTDIR)\db.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\expr.c

"$(INTDIR)\expr.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hdbio.c

"$(INTDIR)\hdbio.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\main.c

"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\perser.c

"$(INTDIR)\perser.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\select.c

"$(INTDIR)\select.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\service.C

"$(INTDIR)\service.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sort.c

"$(INTDIR)\sort.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sql.c

"$(INTDIR)\sql.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\token.c

"$(INTDIR)\token.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\where.c

"$(INTDIR)\where.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

