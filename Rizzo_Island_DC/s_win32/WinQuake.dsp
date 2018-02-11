# Microsoft Developer Studio Project File - Name="winquake" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=winquake - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WinQuake.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WinQuake.mak" CFG="winquake - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "winquake - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "winquake - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "winquake - Win32 GL Debug" (based on "Win32 (x86) Application")
!MESSAGE "winquake - Win32 GL Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /GX /O2 /I "..\common\include" /I "..\r_soft\include" /I ".\include" /I ".\scitech\include" /I ".\dxsdk\sdk\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 .\dxsdk\sdk\lib\dxguid.lib .\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:".\Release/Makaqu.exe"
# SUBTRACT LINK32 /profile /map /debug

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /ML /GX /ZI /Od /I ".\scitech\include" /I ".\dxsdk\sdk\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 .\dxsdk\sdk\lib\dxguid.lib .\scitech\lib\win32\vc\mgllt.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\winquake"
# PROP BASE Intermediate_Dir ".\winquake"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\debug_gl"
# PROP Intermediate_Dir ".\debug_gl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /ML /GX /Zi /Od /I ".\scitech\include" /I ".\dxsdk\sdk\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /ML /GX /ZI /Od /I ".\dxsdk\sdk\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "GLQUAKE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib .\scitech\lib\win32\vc\mgllt.lib /nologo /subsystem:windows /debug /machine:I386
# SUBTRACT BASE LINK32 /nodefaultlib
# ADD LINK32 .\dxsdk\sdk\lib\dxguid.lib comctl32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:".\debug_gl\glquake.exe"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\winquak0"
# PROP BASE Intermediate_Dir ".\winquak0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\release_gl"
# PROP Intermediate_Dir ".\release_gl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /GX /Ox /Ot /Ow /I ".\scitech\include" /I ".\dxsdk\sdk\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# SUBTRACT BASE CPP /Oa /Og
# ADD CPP /nologo /G5 /GX /Od /I "..\common\include" /I "..\r_gl\include" /I ".\include" /I ".\dxsdk\sdk\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "GLQUAKE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib .\scitech\lib\win32\vc\mgllt.lib /nologo /subsystem:windows /profile /machine:I386
# SUBTRACT BASE LINK32 /map /debug
# ADD LINK32 .\dxsdk\sdk\lib\dxguid.lib comctl32.lib winmm.lib wsock32.lib opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:".\release_gl\glMakaqu.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "winquake - Win32 Release"
# Name "winquake - Win32 Debug"
# Name "winquake - Win32 GL Debug"
# Name "winquake - Win32 GL Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Group "r_soft"

# PROP Default_Filter ""
# Begin Group "2D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\r_soft\src\d_draw.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\d_draw.s
InputName=d_draw

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\d_draw.s
InputName=d_draw

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_draw16.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\d_draw16.s
InputName=d_draw16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\d_draw16.s
InputName=d_draw16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_part.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_parta.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\d_parta.s
InputName=d_parta

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\d_parta.s
InputName=d_parta

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\draw.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\screen.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "MDL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\r_soft\src\d_polysa.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\d_polysa.s
InputName=d_polysa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\d_polysa.s
InputName=d_polysa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_polyse.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_aclip.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_aclipa.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\r_aclipa.s
InputName=r_aclipa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\r_aclipa.s
InputName=r_aclipa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_alias.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_aliasa.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\r_aliasa.s
InputName=r_aliasa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\r_aliasa.s
InputName=r_aliasa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "BSP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\r_soft\src\d_edge.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_scan.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_scana.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\d_scana.s
InputName=d_scana

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\d_scana.s
InputName=d_scana

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_sky.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_surf.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_bsp.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_draw.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_edge.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_edgea.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\r_edgea.s
InputName=r_edgea

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\r_edgea.s
InputName=r_edgea

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_sky.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_surf.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\surf16.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\surf16.s
InputName=surf16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\surf16.s
InputName=surf16

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\surf8.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\surf8.s
InputName=surf8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\surf8.s
InputName=surf8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "SPR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\r_soft\src\d_spr8.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\d_spr8.s
InputName=d_spr8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\d_spr8.s
InputName=d_spr8

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_sprite.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_sprite.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=..\r_soft\src\d_fill.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_init.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_modech.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_vars.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_varsa.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\d_varsa.s
InputName=d_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\d_varsa.s
InputName=d_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\d_zpoint.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\model.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_drawa.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\r_drawa.s
InputName=r_drawa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\r_drawa.s
InputName=r_drawa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_efrag.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_light.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_main.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_misc.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_vars.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_soft\src\r_varsa.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\r_soft\src\r_varsa.s
InputName=r_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\r_soft\src\r_varsa.s
InputName=r_varsa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "r_gl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\r_gl\src\gl_draw.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_mesh.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_model.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_refrag.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_rlight.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_rmain.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_rmisc.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_rsurf.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_screen.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_win32gl\src\gl_test.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_vidnt.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_win32gl\src\gl_vidnt.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\r_gl\src\gl_warp.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "s_win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cd_win.c
# End Source File
# Begin Source File

SOURCE=.\src\conproc.c
# End Source File
# Begin Source File

SOURCE=.\src\in_win.c
# End Source File
# Begin Source File

SOURCE=.\src\net_win.c
# End Source File
# Begin Source File

SOURCE=.\src\net_wins.c
# End Source File
# Begin Source File

SOURCE=.\src\net_wipx.c
# End Source File
# Begin Source File

SOURCE=.\src\snd_win.c
# End Source File
# Begin Source File

SOURCE=.\src\sys_win.c
# End Source File
# Begin Source File

SOURCE=.\src\sys_wina.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=.\src\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=.\src\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\debug_gl
InputPath=.\src\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP /DGLQUAKE > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# Begin Custom Build - mycoolbuild
OutDir=.\release_gl
InputPath=.\src\sys_wina.s
InputName=sys_wina

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP /DGLQUAKE > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\vid_win.c

!IF  "$(CFG)" == "winquake - Win32 Release"

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\src\chase.c
# End Source File
# Begin Source File

SOURCE=..\common\src\cl_demo.c
# End Source File
# Begin Source File

SOURCE=..\common\src\cl_input.c
# End Source File
# Begin Source File

SOURCE=..\common\src\cl_main.c
# End Source File
# Begin Source File

SOURCE=..\common\src\cl_parse.c
# End Source File
# Begin Source File

SOURCE=..\common\src\cl_tent.c
# End Source File
# Begin Source File

SOURCE=..\common\src\cmd.c
# End Source File
# Begin Source File

SOURCE=..\common\src\common.c
# End Source File
# Begin Source File

SOURCE=..\common\src\console.c
# End Source File
# Begin Source File

SOURCE=..\common\src\crc.c
# End Source File
# Begin Source File

SOURCE=..\common\src\cvar.c
# End Source File
# Begin Source File

SOURCE=..\common\src\host.c
# End Source File
# Begin Source File

SOURCE=..\common\src\host_cmd.c
# End Source File
# Begin Source File

SOURCE=..\common\src\keys.c
# End Source File
# Begin Source File

SOURCE=..\common\src\math.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\common\src\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\common\src\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\debug_gl
InputPath=..\common\src\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP /DGLQUAKE > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# Begin Custom Build - mycoolbuild
OutDir=.\release_gl
InputPath=..\common\src\math.s
InputName=math

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\src\mathlib.c
# End Source File
# Begin Source File

SOURCE=..\common\src\menu.c
# End Source File
# Begin Source File

SOURCE=..\common\src\net_dgrm.c
# End Source File
# Begin Source File

SOURCE=..\common\src\net_loop.c
# End Source File
# Begin Source File

SOURCE=..\common\src\net_main.c
# End Source File
# Begin Source File

SOURCE=..\common\src\net_vcr.c
# End Source File
# Begin Source File

SOURCE=..\common\src\pr_cmds.c
# End Source File
# Begin Source File

SOURCE=..\common\src\pr_edict.c
# End Source File
# Begin Source File

SOURCE=..\common\src\pr_exec.c
# End Source File
# Begin Source File

SOURCE=..\common\src\r_part.c
# End Source File
# Begin Source File

SOURCE=..\common\src\sbar.c
# End Source File
# Begin Source File

SOURCE=..\common\src\sv_main.c
# End Source File
# Begin Source File

SOURCE=..\common\src\sv_move.c
# End Source File
# Begin Source File

SOURCE=..\common\src\sv_phys.c
# End Source File
# Begin Source File

SOURCE=..\common\src\sv_user.c
# End Source File
# Begin Source File

SOURCE=..\common\src\view.c
# End Source File
# Begin Source File

SOURCE=..\common\src\wad.c
# End Source File
# Begin Source File

SOURCE=..\common\src\world.c
# End Source File
# Begin Source File

SOURCE=..\common\src\worlda.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\common\src\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\common\src\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\debug_gl
InputPath=..\common\src\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP /DGLQUAKE > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# Begin Custom Build - mycoolbuild
OutDir=.\release_gl
InputPath=..\common\src\worlda.s
InputName=worlda

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP /DGLQUAKE > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\src\zone.c
# End Source File
# End Group
# Begin Group "common_audio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\src\snd_dma.c
# End Source File
# Begin Source File

SOURCE=..\common\src\snd_mem.c
# End Source File
# Begin Source File

SOURCE=..\common\src\snd_mix.c
# End Source File
# Begin Source File

SOURCE=..\common\src\snd_mixa.s

!IF  "$(CFG)" == "winquake - Win32 Release"

# Begin Custom Build - mycoolbuild
OutDir=.\Release
InputPath=..\common\src\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\Debug
InputPath=..\common\src\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

# Begin Custom Build - mycoolbuild
OutDir=.\debug_gl
InputPath=..\common\src\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP /DGLQUAKE > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# Begin Custom Build - mycoolbuild
OutDir=.\release_gl
InputPath=..\common\src\snd_mixa.s
InputName=snd_mixa

"$(OUTDIR)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	cl /EP /DGLQUAKE > $(OUTDIR)\$(InputName).spp $(InputPath) 
	gas2masm\debug\gas2masm < $(OUTDIR)\$(InputName).spp >                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	ml /c /Cp /coff /Fo$(OUTDIR)\$(InputName).obj /Zm /Zi                                                                                                                                                                                                    $(OUTDIR)\$(InputName).asm 
	del $(OUTDIR)\$(InputName).spp 
	
# End Custom Build

!ENDIF 

# End Source File
# End Group
# Begin Group "s_dc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\s_dc\src\cd_dc.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\dc_menu.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\dc_vibe.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\dc_vmu.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\in_dc.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\Makefile

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\net_dc.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\snd_dc.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\snd_sdl.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\sys_dc.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\vid_dc.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\s_dc\src\vid_dcgl.c

!IF  "$(CFG)" == "winquake - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "winquake - Win32 Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Debug"

!ELSEIF  "$(CFG)" == "winquake - Win32 GL Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "r_common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\r_common\src\r_2d.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\winquake.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Group "r_gl_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\r_gl\include\gl_model.h
# End Source File
# Begin Source File

SOURCE=..\r_gl\include\gl_warp_sin.h
# End Source File
# Begin Source File

SOURCE=..\r_gl\include\glquake.h
# End Source File
# End Group
# Begin Group "s_win32_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\conproc.h
# End Source File
# Begin Source File

SOURCE=.\include\net_wins.h
# End Source File
# Begin Source File

SOURCE=.\include\net_wipx.h
# End Source File
# Begin Source File

SOURCE=.\include\winquake.h
# End Source File
# End Group
# Begin Group "common_h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\include\anorm_dots.h
# End Source File
# Begin Source File

SOURCE=..\common\include\anorms.h
# End Source File
# Begin Source File

SOURCE=..\common\include\bspfile.h
# End Source File
# Begin Source File

SOURCE=..\common\include\cdaudio.h
# End Source File
# Begin Source File

SOURCE=..\common\include\cmd.h
# End Source File
# Begin Source File

SOURCE=..\common\include\common.h
# End Source File
# Begin Source File

SOURCE=..\common\include\console.h
# End Source File
# Begin Source File

SOURCE=..\common\include\crc.h
# End Source File
# Begin Source File

SOURCE=..\common\include\cvar.h
# End Source File
# Begin Source File

SOURCE=..\common\include\input.h
# End Source File
# Begin Source File

SOURCE=..\common\include\keys.h
# End Source File
# Begin Source File

SOURCE=..\common\include\mathlib.h
# End Source File
# Begin Source File

SOURCE=..\common\include\menu.h
# End Source File
# Begin Source File

SOURCE=..\common\include\pr_comp.h
# End Source File
# Begin Source File

SOURCE=..\common\include\progdefs.h
# End Source File
# Begin Source File

SOURCE=..\common\include\progs.h
# End Source File
# Begin Source File

SOURCE=..\common\include\sbar.h
# End Source File
# Begin Source File

SOURCE=..\common\include\sound.h
# End Source File
# Begin Source File

SOURCE=..\common\include\sys.h
# End Source File
# Begin Source File

SOURCE=..\common\include\wad.h
# End Source File
# Begin Source File

SOURCE=..\common\include\world.h
# End Source File
# Begin Source File

SOURCE=..\common\include\zone.h
# End Source File
# End Group
# Begin Group "common_net"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\include\client.h
# End Source File
# Begin Source File

SOURCE=..\common\include\net.h
# End Source File
# Begin Source File

SOURCE=..\common\include\net_dgrm.h
# End Source File
# Begin Source File

SOURCE=..\common\include\net_loop.h
# End Source File
# Begin Source File

SOURCE=..\common\include\net_vcr.h
# End Source File
# Begin Source File

SOURCE=..\common\include\protocol.h
# End Source File
# Begin Source File

SOURCE=..\common\include\server.h
# End Source File
# End Group
# Begin Group "common_rend"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\include\d_iface.h
# End Source File
# Begin Source File

SOURCE=..\common\include\draw.h
# End Source File
# Begin Source File

SOURCE=..\common\include\model.h
# End Source File
# Begin Source File

SOURCE=..\common\include\modelgen.h
# End Source File
# Begin Source File

SOURCE=..\common\include\r_local.h
# End Source File
# Begin Source File

SOURCE=..\common\include\r_shared.h
# End Source File
# Begin Source File

SOURCE=..\common\include\render.h
# End Source File
# Begin Source File

SOURCE=..\common\include\screen.h
# End Source File
# Begin Source File

SOURCE=..\common\include\spritegn.h
# End Source File
# Begin Source File

SOURCE=..\common\include\vid.h
# End Source File
# Begin Source File

SOURCE=..\common\include\view.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\common\include\asm_i386.h
# End Source File
# Begin Source File

SOURCE=.\include\dosisms.h
# End Source File
# Begin Source File

SOURCE=.\include\net_ser.h
# End Source File
# Begin Source File

SOURCE=..\common\include\quakeasm.h
# End Source File
# Begin Source File

SOURCE=..\common\include\quakedef.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\quake.ico
# End Source File
# End Group
# Begin Source File

SOURCE=..\common\include\progdefs.q1
# End Source File
# End Target
# End Project
