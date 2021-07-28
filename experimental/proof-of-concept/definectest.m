%% About definectest.mlx
% This file defines the MATLAB interface to the library |ctest|.
%
% Commented sections represent C++ functionality that MATLAB cannot automatically define. To include
% functionality, uncomment a section and provide values for &lt;SHAPE&gt;, &lt;DIRECTION&gt;, etc. For more
% information, see <matlab:helpview(fullfile(docroot,'matlab','helptargets.map'),'cpp_define_interface') Define MATLAB Interface for C++ Library>.



%% Setup. Do not edit this section.
function libDef = definectest()
libDef = clibgen.LibraryDefinition("ctestData.xml");
%% OutputFolder and Libraries 
libDef.OutputFolder = "/Users/srinivas/GoogleDrive/code/xolotl/test/proof-of-concept";
libDef.Libraries = "";

%% C++ class |mycontainer| with MATLAB name |clib.ctest.mycontainer| 
mycontainerDefinition = addClass(libDef, "mycontainer", "MATLABName", "clib.ctest.mycontainer", ...
    "Description", "clib.ctest.mycontainer    Representation of C++ class mycontainer."); % Modify help description values as needed.

%% C++ class constructor for C++ class |mycontainer| 
% C++ Signature: mycontainer::mycontainer()
mycontainerConstructor1Definition = addConstructor(mycontainerDefinition, ...
    "mycontainer::mycontainer()", ...
    "Description", "clib.ctest.mycontainer    Constructor of C++ class mycontainer."); % Modify help description values as needed.
validate(mycontainerConstructor1Definition);

%% C++ class method |addThing| for C++ class |mycontainer| 
% C++ Signature: void mycontainer::addThing(thing * thing_)
addThingDefinition = addMethod(mycontainerDefinition, ...
    "void mycontainer::addThing(thing * thing_)", ...
    "MATLABName", "addThing", ...
    "Description", "addThing    Method of C++ class mycontainer."); % Modify help description values as needed.
defineArgument(addThingDefinition, "thing_", "clib.ctest.thing", "input", 1); % '<MLTYPE>' can be clib.ctest.thing, or clib.array.ctest.thing
validate(addThingDefinition);

%% C++ class method |init| for C++ class |mycontainer| 
% C++ Signature: void mycontainer::init()
initDefinition = addMethod(mycontainerDefinition, ...
    "void mycontainer::init()", ...
    "MATLABName", "init", ...
    "Description", "init    Method of C++ class mycontainer."); % Modify help description values as needed.
validate(initDefinition);

%% C++ class constructor for C++ class |mycontainer| 
% C++ Signature: mycontainer::mycontainer(mycontainer const & input1)
mycontainerConstructor2Definition = addConstructor(mycontainerDefinition, ...
    "mycontainer::mycontainer(mycontainer const & input1)", ...
    "Description", "clib.ctest.mycontainer    Constructor of C++ class mycontainer."); % Modify help description values as needed.
defineArgument(mycontainerConstructor2Definition, "input1", "clib.ctest.mycontainer", "input");
validate(mycontainerConstructor2Definition);

%% C++ class public data member |value| for C++ class |mycontainer| 
% C++ Signature: double mycontainer::value
addProperty(mycontainerDefinition, "value", "double", ...
    "Description", "double    Data member of C++ class mycontainer."); % Modify help description values as needed.

%% C++ class public data member |n_things| for C++ class |mycontainer| 
% C++ Signature: int mycontainer::n_things
addProperty(mycontainerDefinition, "n_things", "int32", ...
    "Description", "int32    Data member of C++ class mycontainer."); % Modify help description values as needed.

%% C++ class |thing| with MATLAB name |clib.ctest.thing| 
thingDefinition = addClass(libDef, "thing", "MATLABName", "clib.ctest.thing", ...
    "Description", "clib.ctest.thing    Representation of C++ class thing."); % Modify help description values as needed.

%% C++ class constructor for C++ class |thing| 
% C++ Signature: thing::thing()
thingConstructor1Definition = addConstructor(thingDefinition, ...
    "thing::thing()", ...
    "Description", "clib.ctest.thing    Constructor of C++ class thing."); % Modify help description values as needed.
validate(thingConstructor1Definition);

%% C++ class method |resetContainerValue| for C++ class |thing| 
% C++ Signature: void thing::resetContainerValue()
resetContainerValueDefinition = addMethod(thingDefinition, ...
    "void thing::resetContainerValue()", ...
    "MATLABName", "resetContainerValue", ...
    "Description", "resetContainerValue    Method of C++ class thing."); % Modify help description values as needed.
validate(resetContainerValueDefinition);

%% C++ class constructor for C++ class |thing| 
% C++ Signature: thing::thing(thing const & input1)
thingConstructor2Definition = addConstructor(thingDefinition, ...
    "thing::thing(thing const & input1)", ...
    "Description", "clib.ctest.thing    Constructor of C++ class thing."); % Modify help description values as needed.
defineArgument(thingConstructor2Definition, "input1", "clib.ctest.thing", "input");
validate(thingConstructor2Definition);

%% C++ class public data member |value| for C++ class |thing| 
% C++ Signature: double thing::value
addProperty(thingDefinition, "value", "double", ...
    "Description", "double    Data member of C++ class thing."); % Modify help description values as needed.

%% C++ class public data member |container| for C++ class |thing| 
% C++ Signature: mycontainer * thing::container
addProperty(thingDefinition, "container", "clib.ctest.mycontainer", 1, ... % '<MLTYPE>' can be clib.ctest.mycontainer, or clib.array.ctest.mycontainer
    "Description", "clib.ctest.mycontainer    Data member of C++ class thing."); % Modify help description values as needed.

%% Validate the library definition
validate(libDef);

end
