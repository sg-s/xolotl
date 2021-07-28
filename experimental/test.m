

clear all
close all
rehash
rehash toolboxcache

 % wipe everything clean
! rm *.xml
! rm -rf xolotl
! rm *.mlx
! rm definexolotl.m



hppFiles = ["xolotl.hpp","compartment.hpp","conductance.hpp","NaV.hpp","Leak.hpp","Kd.hpp"];
cppFiles = "xolotl.cpp";



clibgen.generateLibraryDefinition(hppFiles,"SupportingSourceFiles",cppFiles,"IncludePath",".", "ReturnCArrays",false,"PackageName","xolotl","TreatObjectPointerAsScalar", true,"Verbose",true);

libdef = definexolotl;



addProperty(libdef.Classes(2), "arr", "clib.array.xolotl.Int", "n_steps", "Description", "clib.array.xolotl.Int    Data member of C++ class compartment."); 


% getMatDefinition = addMethod(libdef.Classes(2), "int const * const compartment::getMat(size_t len2) const","MATLABName", "getMat");
% defineArgument(getMatDefinition, "len2", "uint64");
% defineOutput(getMatDefinition, "RetVal", "int32", "len2"); 
% validate(getMatDefinition);




validate(libdef);
build(libdef)

addpath("./xolotl")

% now test it
x = clib.xolotl.xolotl;
AB = clib.xolotl.compartment("AB");
LP = clib.xolotl.compartment("LP");
x.add(AB)
x.add(LP)
x.init()