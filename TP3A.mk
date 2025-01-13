##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=TP3A
ConfigurationName      :=Debug
WorkspaceConfiguration :=Debug
WorkspacePath          :="C:/Users/Lee Dab/Documents/Documents"
ProjectPath            :="C:/Users/Lee Dab/Documents/Workspace/A.N. c++/TP3A"
IntermediateDirectory  :=../../../Documents/build-$(WorkspaceConfiguration)/__/Workspace/A_N__c++/TP3A
OutDir                 :=$(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Lee Dab
Date                   :=30/12/2023
CodeLitePath           :="C:/Program Files/CodeLite"
MakeDirCommand         :=mkdir
LinkerName             :=C:/MinGW/bin/g++.exe
SharedObjectLinkerName :=C:/MinGW/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputDirectory        :=C:/Users/Lee Dab/Documents/Documents/build-$(WorkspaceConfiguration)/bin
OutputFile             :=..\..\..\Documents\build-$(WorkspaceConfiguration)\bin\$(ProjectName).exe
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :=$(IntermediateDirectory)/ObjectsList.txt
PCHCompileFlags        :=
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overridden using an environment variable
##
AR       := C:/MinGW/bin/ar.exe -r
CXX      := C:/MinGW/bin/g++.exe
CC       := C:/MinGW/bin/gcc.exe
CXXFLAGS :=  -gdwarf-2 -O0 -Wall $(Preprocessors)
CFLAGS   :=  -gdwarf-2 -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
Objects0=$(IntermediateDirectory)/tp3_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/vecteur_template.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: MakeIntermediateDirs $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"
	@if not exist "$(OutputDirectory)" $(MakeDirCommand) "$(OutputDirectory)"

$(IntermediateDirectory)/.d:
	@if not exist "$(IntermediateDirectory)" $(MakeDirCommand) "$(IntermediateDirectory)"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/tp3_main.cpp$(ObjectSuffix): tp3_main.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Lee Dab/Documents/Workspace/A.N. c++/TP3A/tp3_main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/tp3_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/tp3_main.cpp$(PreprocessSuffix): tp3_main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/tp3_main.cpp$(PreprocessSuffix) tp3_main.cpp

$(IntermediateDirectory)/vecteur_template.cpp$(ObjectSuffix): vecteur_template.cpp 
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/Users/Lee Dab/Documents/Workspace/A.N. c++/TP3A/vecteur_template.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/vecteur_template.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/vecteur_template.cpp$(PreprocessSuffix): vecteur_template.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/vecteur_template.cpp$(PreprocessSuffix) vecteur_template.cpp

##
## Clean
##
clean:
	$(RM) -r $(IntermediateDirectory)


