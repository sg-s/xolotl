%% About definechannel_test.mlx
% This file defines the MATLAB interface to the library |channel_test|.
%
% Commented sections represent C++ functionality that MATLAB cannot automatically define. To include
% functionality, uncomment a section and provide values for &lt;SHAPE&gt;, &lt;DIRECTION&gt;, etc. For more
% information, see <matlab:helpview(fullfile(docroot,'matlab','helptargets.map'),'cpp_define_interface') Define MATLAB Interface for C++ Library>.



%% Setup. Do not edit this section.
function libDef = definechannel_test()
libDef = clibgen.LibraryDefinition("channel_testData.xml");
%% OutputFolder and Libraries 
libDef.OutputFolder = "/Users/srinivas/GoogleDrive/code/xolotl/experimental";
libDef.Libraries = "";

%% C++ class |channel| with MATLAB name |clib.channel_test.channel| 
channelDefinition = addClass(libDef, "channel", "MATLABName", "clib.channel_test.channel", ...
    "Description", "clib.channel_test.channel    Representation of C++ class channel."); % Modify help description values as needed.

%% C++ class constructor for C++ class |channel| 
% C++ Signature: channel::channel(double gbar,double E,int p,int q)
channelConstructor1Definition = addConstructor(channelDefinition, ...
    "channel::channel(double gbar,double E,int p,int q)", ...
    "Description", "clib.channel_test.channel    Constructor of C++ class channel."); % Modify help description values as needed.
defineArgument(channelConstructor1Definition, "gbar", "double");
defineArgument(channelConstructor1Definition, "E", "double");
defineArgument(channelConstructor1Definition, "p", "int32");
defineArgument(channelConstructor1Definition, "q", "int32");
validate(channelConstructor1Definition);

%% C++ class method |set_m_inf| for C++ class |channel| 
% C++ Signature: void channel::set_m_inf(std::string input1)
set_m_infDefinition = addMethod(channelDefinition, ...
    "void channel::set_m_inf(std::string input1)", ...
    "MATLABName", "set_m_inf", ...
    "Description", "set_m_inf    Method of C++ class channel."); % Modify help description values as needed.
defineArgument(set_m_infDefinition, "input1", "string");
validate(set_m_infDefinition);

%% C++ class method |evaluate| for C++ class |channel| 
% C++ Signature: double channel::evaluate()
evaluateDefinition = addMethod(channelDefinition, ...
    "double channel::evaluate()", ...
    "MATLABName", "evaluate", ...
    "Description", "evaluate    Method of C++ class channel."); % Modify help description values as needed.
defineOutput(evaluateDefinition, "RetVal", "double");
validate(evaluateDefinition);

%% C++ class constructor for C++ class |channel| 
% C++ Signature: channel::channel(channel const & input1)
channelConstructor2Definition = addConstructor(channelDefinition, ...
    "channel::channel(channel const & input1)", ...
    "Description", "clib.channel_test.channel    Constructor of C++ class channel."); % Modify help description values as needed.
defineArgument(channelConstructor2Definition, "input1", "clib.channel_test.channel", "input");
validate(channelConstructor2Definition);

%% C++ class public data member |name| for C++ class |channel| 
% C++ Signature: std::string channel::name
addProperty(channelDefinition, "name", "string", ...
    "Description", "string    Data member of C++ class channel."); % Modify help description values as needed.

%% C++ class public data member |E| for C++ class |channel| 
% C++ Signature: double channel::E
addProperty(channelDefinition, "E", "double", ...
    "Description", "double    Data member of C++ class channel."); % Modify help description values as needed.

%% C++ class public data member |g| for C++ class |channel| 
% C++ Signature: double channel::g
addProperty(channelDefinition, "g", "double", ...
    "Description", "double    Data member of C++ class channel."); % Modify help description values as needed.

%% C++ class public data member |gbar| for C++ class |channel| 
% C++ Signature: double channel::gbar
addProperty(channelDefinition, "gbar", "double", ...
    "Description", "double    Data member of C++ class channel."); % Modify help description values as needed.

%% C++ class |prinz| with MATLAB name |clib.channel_test.prinz| 
prinzDefinition = addClass(libDef, "prinz", "MATLABName", "clib.channel_test.prinz", ...
    "Description", "clib.channel_test.prinz    Representation of C++ class prinz."); % Modify help description values as needed.

%% C++ class method |NaV_m_inf| for C++ class |prinz| 
% C++ Signature: static double prinz::NaV_m_inf(double V)
NaV_m_infDefinition = addMethod(prinzDefinition, ...
    "static double prinz::NaV_m_inf(double V)", ...
    "MATLABName", "NaV_m_inf", ...
    "Description", "NaV_m_inf    Method of C++ class prinz."); % Modify help description values as needed.
defineArgument(NaV_m_infDefinition, "V", "double");
defineOutput(NaV_m_infDefinition, "RetVal", "double");
validate(NaV_m_infDefinition);

%% C++ class constructor for C++ class |prinz| 
% C++ Signature: prinz::prinz()
prinzConstructor1Definition = addConstructor(prinzDefinition, ...
    "prinz::prinz()", ...
    "Description", "clib.channel_test.prinz    Constructor of C++ class prinz."); % Modify help description values as needed.
validate(prinzConstructor1Definition);

%% C++ class constructor for C++ class |prinz| 
% C++ Signature: prinz::prinz(prinz const & input1)
prinzConstructor2Definition = addConstructor(prinzDefinition, ...
    "prinz::prinz(prinz const & input1)", ...
    "Description", "clib.channel_test.prinz    Constructor of C++ class prinz."); % Modify help description values as needed.
defineArgument(prinzConstructor2Definition, "input1", "clib.channel_test.prinz", "input");
validate(prinzConstructor2Definition);

%% Validate the library definition
validate(libDef);

end
