 % wipe everything clean
! rm *.xml
! rm -rf ctest
! rm *.mlx
! rm definectest.m



hppFiles = ["thing.hpp","mycontainer.hpp"];

clibgen.generateLibraryDefinition(hppFiles,"SupportingSourceFiles","method_definitions.cpp","IncludePath",".", "ReturnCArrays",false,"PackageName","ctest","TreatObjectPointerAsScalar", true) 


libdef = definectest;


addProperty(libdef.Classes(1), "things", "vector <thing*>", ...
    "Description", "things"); 

validate(libdef);
build(libdef)

addpath("./ctest")


% test
C = clib.ctest.mycontainer;
T = clib.ctest.thing;
C.addThing(T);

disp(['Value before init = ' mat2str(T.value)])
C.init
disp(['Value after init = ' mat2str(T.value)])
T.resetContainerValue()
disp(['asking thing to reset container, value = ' mat2str(C.value)])
