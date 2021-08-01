# Microsoft Developer Studio Generated NMAKE File, Based on hdbm.dsp
!IF "$(CFG)" == ""
CFG=hdbm - Win32 Debug
!MESSAGE No configuration specified. Defaulting to hdbm - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "hdbm - Win32 Release" && "$(CFG)" != "hdbm - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "hdbm.mak" CFG="hdbm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "hdbm - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "hdbm - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "hdbm - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\hdbm.exe"


CLEAN :
	-@erase "$(INTDIR)\db.obj"
	-@erase "$(INTDIR)\dbclnt.obj"
	-@erase "$(INTDIR)\hdbm.obj"
	-@erase "$(INTDIR)\token.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\hdbm.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /Op /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\hdbm.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hdbm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\hdbm.pdb" /machine:I386 /out:"$(OUTDIR)\hdbm.exe" 
LINK32_OBJS= \
	"$(INTDIR)\db.obj" \
	"$(INTDIR)\dbclnt.obj" \
	"$(INTDIR)\hdbm.obj" \
	"$(INTDIR)\token.obj"

"$(OUTDIR)\hdbm.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "hdbm - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\hdbm.exe"


CLEAN :
	-@erase "$(INTDIR)\db.obj"
	-@erase "$(INTDIR)\dbclnt.obj"
	-@erase "$(INTDIR)\hdbm.obj"
	-@erase "$(INTDIR)\token.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\hdbm.exe"
	-@erase "$(OUTDIR)\hdbm.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\hdbm.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\hdbm.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\hdbm.pdb" /machine:I386 /out:"$(OUTDIR)\hdbm.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\db.obj" \
	"$(INTDIR)\dbclnt.obj" \
	"$(INTDIR)\hdbm.obj" \
	"$(INTDIR)\token.obj"

"$(OUTDIR)\hdbm.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("hdbm.dep")
!INCLUDE "hdbm.dep"
!ELSE 
!MESSAGE Warning: cannot find "hdbm.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "hdbm - Win32 Release" || "$(CFG)" == "hdbm - Win32 Debug"
SOURCE=.\db.c

"$(INTDIR)\db.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\dbclnt.c

"$(INTDIR)\dbclnt.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\hdbm.c

"$(INTDIR)\hdbm.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\token.c

"$(INTDIR)\token.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

