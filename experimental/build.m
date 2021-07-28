

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



addProperty(libdef.Classes(2), "V_data", "double", "n_steps", "Description", "clib.array.xolotl.double    Data member of C++ class compartment."); 


validate(libdef);
build(libdef)

addpath("./xolotl")

