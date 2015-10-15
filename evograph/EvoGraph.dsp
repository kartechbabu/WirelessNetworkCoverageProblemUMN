# Microsoft Developer Studio Project File - Name="EvoGraph" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=EvoGraph - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EvoGraph.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EvoGraph.mak" CFG="EvoGraph - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EvoGraph - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "EvoGraph - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\Gpkernel" /I ".\Newmat10" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr /YX /FD /GZ /TP /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"Debug/HFCGP.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "EvoGraph___Win32_Release"
# PROP BASE Intermediate_Dir "EvoGraph___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\Gpkernel" /I ".\Newmat10" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr /YX /FD /GZ /TP /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".\Gpkernel" /I ".\Newmat10" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr /YX /FD /GZ /TP /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EvoGraph - Win32 Debug"
# Name "EvoGraph - Win32 Release"
# Begin Group "GPKernel"

# PROP Default_Filter ""
# Begin Group "StructHashTable"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Gpkernal\Hashtable\Bucket.cpp
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\Hashtable\Bucket.h
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\Hashtable\HashTable.cpp
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\Hashtable\HashTable.h
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\Hashtable\Struct.cpp
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\Hashtable\Struct.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Gpkernal\boltzman.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\bstworst.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\Change.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\ckpoint.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\Crossovr.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\defines.h
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\ephem.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\eval.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\event.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\event.h
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\exch.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\fitness.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\genspace.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\gp.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\individ.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\lilgp.h
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\main.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\memory.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\multifitness.cpp
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\multifitness.h
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\mutate.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\mutate_erc.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\output.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\params.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\populate.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\pretty.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\protoapp.h
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\PROTOS.H
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\random.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\reproduc.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\select.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\sigma.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\tournmnt.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\tree.c
# End Source File
# Begin Source File

SOURCE=.\Gpkernal\types.h
# End Source File
# End Group
# Begin Group "Bondgraph"

# PROP Default_Filter ""
# Begin Group "Matrix"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Newmat10\BANDMAT.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\Boolean.h
# End Source File
# Begin Source File

SOURCE=.\Newmat10\CHOLESKY.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Newmat10\CONTROLW.H
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Newmat10\EVALUE.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Newmat10\FFT.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Newmat10\HHOLDER.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Newmat10\Include.h
# End Source File
# Begin Source File

SOURCE=.\Newmat10\JACOBI.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Newmat10\MYEXCEPT.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\MYEXCEPT.H
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWFFT.CPP

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Newmat10\Newmat.h
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMAT1.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMAT2.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMAT3.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\Newmat4.cpp
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMAT5.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\Newmat6.cpp
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMAT7.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMAT8.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMAT9.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMATAP.H
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMATEX.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMATIO.H
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMATNL.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMATNL.H
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMATRC.H
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMATRM.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\NEWMATRM.H
# End Source File
# Begin Source File

SOURCE=.\Newmat10\Numeric.cpp
# End Source File
# Begin Source File

SOURCE=.\Newmat10\Numeric.h
# End Source File
# Begin Source File

SOURCE=.\Newmat10\PRECISIO.H
# End Source File
# Begin Source File

SOURCE=.\Newmat10\SOLUTION.CPP
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\Newmat10\SOLUTION.H
# End Source File
# Begin Source File

SOURCE=.\Newmat10\SORT.CPP

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Newmat10\SUBMAT.CPP
# End Source File
# Begin Source File

SOURCE=.\Newmat10\SVD.CPP
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Group
# Begin Group "AppGraph"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AppGraph\App.c
# End Source File
# Begin Source File

SOURCE=.\AppGraph\App.h
# End Source File
# Begin Source File

SOURCE=.\AppGraph\Appdef.h
# End Source File
# Begin Source File

SOURCE=.\AppGraph\Function.c
# End Source File
# Begin Source File

SOURCE=.\AppGraph\Function.h
# End Source File
# End Group
# Begin Group "GraphLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\GraphLib\Algorithm.h

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GraphLib\BreadthFirstTraversal.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GraphLib\BreadthFirstTraversal.h

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GraphLib\DepthFirstTraversal.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GraphLib\DepthFirstTraversal.h

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GraphLib\DirectedGraph.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GraphLib\DirectedGraph.h

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GraphLib\Edge.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Graph.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Graph.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\GraphException.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\GraphUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Main.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Prim.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Prim.h

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GraphLib\random.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GraphLib\ShortestPath.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\GraphLib\ShortestPath.h

!IF  "$(CFG)" == "EvoGraph - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EvoGraph - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\GraphLib\UndirectedGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\UndirectedGraph.h
# End Source File
# Begin Source File

SOURCE=.\GraphLib\vertex.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphLib\Vertex.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DevHistory.txt
# End Source File
# Begin Source File

SOURCE=.\HFC_MERC.txt
# End Source File
# End Target
# End Project
