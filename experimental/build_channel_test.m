

clear all
close all
rehash
rehash toolboxcache

 % wipe everything clean
! rm *.xml
! rm -rf channel_test
! rm *.mlx
! rm definechannel_test.m



hppFiles = ["channel_test.hpp"];
cppFiles = "channel_test.cpp";



clibgen.generateLibraryDefinition(hppFiles,"SupportingSourceFiles",cppFiles,"IncludePath",".", "ReturnCArrays",false,"PackageName","channel_test","TreatObjectPointerAsScalar", true,"Verbose",true);

libdef = definechannel_test;



% addProperty(libdef.Classes(2), "V_data", "double", "n_steps", "Description", "clib.array.channel_test.double    Data member of C++ class compartment."); 


validate(libdef);
build(libdef)

addpath("./channel_test")



% % clean up
% ! rm definexolotl.m
% ! rm *.mlx