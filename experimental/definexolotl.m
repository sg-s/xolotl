%% About definexolotl.mlx
% This file defines the MATLAB interface to the library |xolotl|.
%
% Commented sections represent C++ functionality that MATLAB cannot automatically define. To include
% functionality, uncomment a section and provide values for &lt;SHAPE&gt;, &lt;DIRECTION&gt;, etc. For more
% information, see <matlab:helpview(fullfile(docroot,'matlab','helptargets.map'),'cpp_define_interface') Define MATLAB Interface for C++ Library>.



%% Setup. Do not edit this section.
function libDef = definexolotl()
libDef = clibgen.LibraryDefinition("xolotlData.xml");
%% OutputFolder and Libraries 
libDef.OutputFolder = "/Users/srinivas/GoogleDrive/code/xolotl/experimental";
libDef.Libraries = "";

%% C++ class |conductance| with MATLAB name |clib.xolotl.conductance| 
conductanceDefinition = addClass(libDef, "conductance", "MATLABName", "clib.xolotl.conductance", ...
    "Description", "clib.xolotl.conductance    Representation of C++ class conductance."); % Modify help description values as needed.

%% C++ class constructor for C++ class |conductance| 
% C++ Signature: conductance::conductance()
conductanceConstructor1Definition = addConstructor(conductanceDefinition, ...
    "conductance::conductance()", ...
    "Description", "clib.xolotl.conductance    Constructor of C++ class conductance."); % Modify help description values as needed.
validate(conductanceConstructor1Definition);

%% C++ class method |init| for C++ class |conductance| 
% C++ Signature: void conductance::init()
initDefinition = addMethod(conductanceDefinition, ...
    "void conductance::init()", ...
    "MATLABName", "init", ...
    "Description", "init    Method of C++ class conductance."); % Modify help description values as needed.
validate(initDefinition);

%% C++ class method |step| for C++ class |conductance| 
% C++ Signature: void conductance::step()
stepDefinition = addMethod(conductanceDefinition, ...
    "void conductance::step()", ...
    "MATLABName", "step", ...
    "Description", "step    Method of C++ class conductance."); % Modify help description values as needed.
validate(stepDefinition);

%% C++ class method |m_inf| for C++ class |conductance| 
% C++ Signature: double conductance::m_inf(double input1)
m_infDefinition = addMethod(conductanceDefinition, ...
    "double conductance::m_inf(double input1)", ...
    "MATLABName", "m_inf", ...
    "Description", "m_inf    Method of C++ class conductance."); % Modify help description values as needed.
defineArgument(m_infDefinition, "input1", "double");
defineOutput(m_infDefinition, "RetVal", "double");
validate(m_infDefinition);

%% C++ class method |h_inf| for C++ class |conductance| 
% C++ Signature: double conductance::h_inf(double input1)
h_infDefinition = addMethod(conductanceDefinition, ...
    "double conductance::h_inf(double input1)", ...
    "MATLABName", "h_inf", ...
    "Description", "h_inf    Method of C++ class conductance."); % Modify help description values as needed.
defineArgument(h_infDefinition, "input1", "double");
defineOutput(h_infDefinition, "RetVal", "double");
validate(h_infDefinition);

%% C++ class method |tau_m| for C++ class |conductance| 
% C++ Signature: double conductance::tau_m(double input1)
tau_mDefinition = addMethod(conductanceDefinition, ...
    "double conductance::tau_m(double input1)", ...
    "MATLABName", "tau_m", ...
    "Description", "tau_m    Method of C++ class conductance."); % Modify help description values as needed.
defineArgument(tau_mDefinition, "input1", "double");
defineOutput(tau_mDefinition, "RetVal", "double");
validate(tau_mDefinition);

%% C++ class method |tau_h| for C++ class |conductance| 
% C++ Signature: double conductance::tau_h(double input1)
tau_hDefinition = addMethod(conductanceDefinition, ...
    "double conductance::tau_h(double input1)", ...
    "MATLABName", "tau_h", ...
    "Description", "tau_h    Method of C++ class conductance."); % Modify help description values as needed.
defineArgument(tau_hDefinition, "input1", "double");
defineOutput(tau_hDefinition, "RetVal", "double");
validate(tau_hDefinition);

%% C++ class constructor for C++ class |conductance| 
% C++ Signature: conductance::conductance(conductance const & input1)
conductanceConstructor2Definition = addConstructor(conductanceDefinition, ...
    "conductance::conductance(conductance const & input1)", ...
    "Description", "clib.xolotl.conductance    Constructor of C++ class conductance."); % Modify help description values as needed.
defineArgument(conductanceConstructor2Definition, "input1", "clib.xolotl.conductance", "input");
validate(conductanceConstructor2Definition);

%% C++ class public data member |name| for C++ class |conductance| 
% C++ Signature: std::string conductance::name
addProperty(conductanceDefinition, "name", "string", ...
    "Description", "string    Data member of C++ class conductance."); % Modify help description values as needed.

%% C++ class public data member |dt| for C++ class |conductance| 
% C++ Signature: double conductance::dt
addProperty(conductanceDefinition, "dt", "double", ...
    "Description", "double    Data member of C++ class conductance."); % Modify help description values as needed.

%% C++ class public data member |E| for C++ class |conductance| 
% C++ Signature: double conductance::E
addProperty(conductanceDefinition, "E", "double", ...
    "Description", "double    Data member of C++ class conductance."); % Modify help description values as needed.

%% C++ class public data member |g| for C++ class |conductance| 
% C++ Signature: double conductance::g
addProperty(conductanceDefinition, "g", "double", ...
    "Description", "double    Data member of C++ class conductance."); % Modify help description values as needed.

%% C++ class public data member |gbar| for C++ class |conductance| 
% C++ Signature: double conductance::gbar
addProperty(conductanceDefinition, "gbar", "double", ...
    "Description", "double    Data member of C++ class conductance."); % Modify help description values as needed.

%% C++ class public data member |container| for C++ class |conductance| 
% C++ Signature: compartment * conductance::container
addProperty(conductanceDefinition, "container", "clib.xolotl.compartment", 1, ... % '<MLTYPE>' can be clib.xolotl.compartment, or clib.array.xolotl.compartment
    "Description", "clib.xolotl.compartment    Data member of C++ class conductance."); % Modify help description values as needed.

%% C++ class public data member |m| for C++ class |conductance| 
% C++ Signature: double conductance::m
addProperty(conductanceDefinition, "m", "double", ...
    "Description", "double    Data member of C++ class conductance."); % Modify help description values as needed.

%% C++ class public data member |h| for C++ class |conductance| 
% C++ Signature: double conductance::h
addProperty(conductanceDefinition, "h", "double", ...
    "Description", "double    Data member of C++ class conductance."); % Modify help description values as needed.

%% C++ class |compartment| with MATLAB name |clib.xolotl.compartment| 
compartmentDefinition = addClass(libDef, "compartment", "MATLABName", "clib.xolotl.compartment", ...
    "Description", "clib.xolotl.compartment    Representation of C++ class compartment."); % Modify help description values as needed.

%% C++ class constructor for C++ class |compartment| 
% C++ Signature: compartment::compartment(std::string name)
compartmentConstructor1Definition = addConstructor(compartmentDefinition, ...
    "compartment::compartment(std::string name)", ...
    "Description", "clib.xolotl.compartment    Constructor of C++ class compartment."); % Modify help description values as needed.
defineArgument(compartmentConstructor1Definition, "name", "string");
validate(compartmentConstructor1Definition);

%% C++ class method |integrate| for C++ class |compartment| 
% C++ Signature: void compartment::integrate()
integrateDefinition = addMethod(compartmentDefinition, ...
    "void compartment::integrate()", ...
    "MATLABName", "integrate", ...
    "Description", "integrate    Method of C++ class compartment."); % Modify help description values as needed.
validate(integrateDefinition);

%% C++ class method |step| for C++ class |compartment| 
% C++ Signature: void compartment::step()
stepDefinition = addMethod(compartmentDefinition, ...
    "void compartment::step()", ...
    "MATLABName", "step", ...
    "Description", "step    Method of C++ class compartment."); % Modify help description values as needed.
validate(stepDefinition);

%% C++ class method |add| for C++ class |compartment| 
% C++ Signature: void compartment::add(conductance * input1)
addDefinition = addMethod(compartmentDefinition, ...
    "void compartment::add(conductance * input1)", ...
    "MATLABName", "add", ...
    "Description", "add    Method of C++ class compartment."); % Modify help description values as needed.
defineArgument(addDefinition, "input1", "clib.xolotl.conductance", "input", 1); % '<MLTYPE>' can be clib.xolotl.conductance, or clib.array.xolotl.conductance
validate(addDefinition);

%% C++ class method |init| for C++ class |compartment| 
% C++ Signature: void compartment::init()
initDefinition = addMethod(compartmentDefinition, ...
    "void compartment::init()", ...
    "MATLABName", "init", ...
    "Description", "init    Method of C++ class compartment."); % Modify help description values as needed.
validate(initDefinition);

%% C++ class method |set_n_steps| for C++ class |compartment| 
% C++ Signature: void compartment::set_n_steps(int n)
set_n_stepsDefinition = addMethod(compartmentDefinition, ...
    "void compartment::set_n_steps(int n)", ...
    "MATLABName", "set_n_steps", ...
    "Description", "set_n_steps    Method of C++ class compartment."); % Modify help description values as needed.
defineArgument(set_n_stepsDefinition, "n", "int32");
validate(set_n_stepsDefinition);

%% C++ class method |channels| for C++ class |compartment| 
% C++ Signature: conductance * compartment::channels(int input1)
channelsDefinition = addMethod(compartmentDefinition, ...
    "conductance * compartment::channels(int input1)", ...
    "MATLABName", "channels", ...
    "Description", "channels    Method of C++ class compartment."); % Modify help description values as needed.
defineArgument(channelsDefinition, "input1", "int32");
defineOutput(channelsDefinition, "RetVal", "clib.xolotl.conductance", 1);
validate(channelsDefinition);

%% C++ class constructor for C++ class |compartment| 
% C++ Signature: compartment::compartment(compartment const & input1)
compartmentConstructor2Definition = addConstructor(compartmentDefinition, ...
    "compartment::compartment(compartment const & input1)", ...
    "Description", "clib.xolotl.compartment    Constructor of C++ class compartment."); % Modify help description values as needed.
defineArgument(compartmentConstructor2Definition, "input1", "clib.xolotl.compartment", "input");
validate(compartmentConstructor2Definition);

%% C++ class public data member |A| for C++ class |compartment| 
% C++ Signature: double compartment::A
addProperty(compartmentDefinition, "A", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |Cm| for C++ class |compartment| 
% C++ Signature: double compartment::Cm
addProperty(compartmentDefinition, "Cm", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |dt| for C++ class |compartment| 
% C++ Signature: double compartment::dt
addProperty(compartmentDefinition, "dt", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |sim_dt| for C++ class |compartment| 
% C++ Signature: double compartment::sim_dt
addProperty(compartmentDefinition, "sim_dt", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |V| for C++ class |compartment| 
% C++ Signature: double compartment::V
addProperty(compartmentDefinition, "V", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |V_prev| for C++ class |compartment| 
% C++ Signature: double compartment::V_prev
addProperty(compartmentDefinition, "V_prev", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |n_steps| for C++ class |compartment| 
% C++ Signature: int compartment::n_steps
addProperty(compartmentDefinition, "n_steps", "int32", ...
    "Description", "int32    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |Ca| for C++ class |compartment| 
% C++ Signature: double compartment::Ca
addProperty(compartmentDefinition, "Ca", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |Ca_prev| for C++ class |compartment| 
% C++ Signature: double compartment::Ca_prev
addProperty(compartmentDefinition, "Ca_prev", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |I_ext| for C++ class |compartment| 
% C++ Signature: double compartment::I_ext
addProperty(compartmentDefinition, "I_ext", "double", ...
    "Description", "double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |V_data| for C++ class |compartment| 
% C++ Signature: double * compartment::V_data
%addProperty(compartmentDefinition, "V_data", "clib.array.xolotl.Double", <SHAPE>, ... % '<MLTYPE>' can be clib.array.xolotl.Double, or double
%    "Description", "clib.array.xolotl.Double    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |name| for C++ class |compartment| 
% C++ Signature: std::string compartment::name
addProperty(compartmentDefinition, "name", "string", ...
    "Description", "string    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class public data member |n_channels| for C++ class |compartment| 
% C++ Signature: int compartment::n_channels
addProperty(compartmentDefinition, "n_channels", "int32", ...
    "Description", "int32    Data member of C++ class compartment."); % Modify help description values as needed.

%% C++ class |xolotl| with MATLAB name |clib.xolotl.xolotl| 
xolotlDefinition = addClass(libDef, "xolotl", "MATLABName", "clib.xolotl.xolotl", ...
    "Description", "clib.xolotl.xolotl    Representation of C++ class xolotl."); % Modify help description values as needed.

%% C++ class constructor for C++ class |xolotl| 
% C++ Signature: xolotl::xolotl()
xolotlConstructor1Definition = addConstructor(xolotlDefinition, ...
    "xolotl::xolotl()", ...
    "Description", "clib.xolotl.xolotl    Constructor of C++ class xolotl."); % Modify help description values as needed.
validate(xolotlConstructor1Definition);

%% C++ class method |init| for C++ class |xolotl| 
% C++ Signature: void xolotl::init()
initDefinition = addMethod(xolotlDefinition, ...
    "void xolotl::init()", ...
    "MATLABName", "init", ...
    "Description", "init    Method of C++ class xolotl."); % Modify help description values as needed.
validate(initDefinition);

%% C++ class method |integrate| for C++ class |xolotl| 
% C++ Signature: void xolotl::integrate()
integrateDefinition = addMethod(xolotlDefinition, ...
    "void xolotl::integrate()", ...
    "MATLABName", "integrate", ...
    "Description", "integrate    Method of C++ class xolotl."); % Modify help description values as needed.
validate(integrateDefinition);

%% C++ class method |add| for C++ class |xolotl| 
% C++ Signature: void xolotl::add(compartment * comp_)
addDefinition = addMethod(xolotlDefinition, ...
    "void xolotl::add(compartment * comp_)", ...
    "MATLABName", "add", ...
    "Description", "add    Method of C++ class xolotl."); % Modify help description values as needed.
defineArgument(addDefinition, "comp_", "clib.xolotl.compartment", "input", 1); % '<MLTYPE>' can be clib.xolotl.compartment, or clib.array.xolotl.compartment
validate(addDefinition);

%% C++ class method |compartments| for C++ class |xolotl| 
% C++ Signature: compartment * xolotl::compartments(int input1)
compartmentsDefinition = addMethod(xolotlDefinition, ...
    "compartment * xolotl::compartments(int input1)", ...
    "MATLABName", "compartments", ...
    "Description", "compartments    Method of C++ class xolotl."); % Modify help description values as needed.
defineArgument(compartmentsDefinition, "input1", "int32");
defineOutput(compartmentsDefinition, "RetVal", "clib.xolotl.compartment", 1);
validate(compartmentsDefinition);

%% C++ class constructor for C++ class |xolotl| 
% C++ Signature: xolotl::xolotl(xolotl const & input1)
xolotlConstructor2Definition = addConstructor(xolotlDefinition, ...
    "xolotl::xolotl(xolotl const & input1)", ...
    "Description", "clib.xolotl.xolotl    Constructor of C++ class xolotl."); % Modify help description values as needed.
defineArgument(xolotlConstructor2Definition, "input1", "clib.xolotl.xolotl", "input");
validate(xolotlConstructor2Definition);

%% C++ class public data member |dt| for C++ class |xolotl| 
% C++ Signature: double xolotl::dt
addProperty(xolotlDefinition, "dt", "double", ...
    "Description", "double    Data member of C++ class xolotl."); % Modify help description values as needed.

%% C++ class public data member |sim_dt| for C++ class |xolotl| 
% C++ Signature: double xolotl::sim_dt
addProperty(xolotlDefinition, "sim_dt", "double", ...
    "Description", "double    Data member of C++ class xolotl."); % Modify help description values as needed.

%% C++ class public data member |n_compartments| for C++ class |xolotl| 
% C++ Signature: int xolotl::n_compartments
addProperty(xolotlDefinition, "n_compartments", "int32", ...
    "Description", "int32    Data member of C++ class xolotl."); % Modify help description values as needed.

%% C++ class |NaV| with MATLAB name |clib.xolotl.NaV| 
NaVDefinition = addClass(libDef, "NaV", "MATLABName", "clib.xolotl.NaV", ...
    "Description", "clib.xolotl.NaV    Representation of C++ class NaV."); % Modify help description values as needed.

%% C++ class constructor for C++ class |NaV| 
% C++ Signature: NaV::NaV()
NaVConstructor1Definition = addConstructor(NaVDefinition, ...
    "NaV::NaV()", ...
    "Description", "clib.xolotl.NaV    Constructor of C++ class NaV."); % Modify help description values as needed.
validate(NaVConstructor1Definition);

%% C++ class constructor for C++ class |NaV| 
% C++ Signature: NaV::NaV(double gbar_)
NaVConstructor2Definition = addConstructor(NaVDefinition, ...
    "NaV::NaV(double gbar_)", ...
    "Description", "clib.xolotl.NaV    Constructor of C++ class NaV."); % Modify help description values as needed.
defineArgument(NaVConstructor2Definition, "gbar_", "double");
validate(NaVConstructor2Definition);

%% C++ class method |m_inf| for C++ class |NaV| 
% C++ Signature: double NaV::m_inf(double V)
m_infDefinition = addMethod(NaVDefinition, ...
    "double NaV::m_inf(double V)", ...
    "MATLABName", "m_inf", ...
    "Description", "m_inf    Method of C++ class NaV."); % Modify help description values as needed.
defineArgument(m_infDefinition, "V", "double");
defineOutput(m_infDefinition, "RetVal", "double");
validate(m_infDefinition);

%% C++ class method |h_inf| for C++ class |NaV| 
% C++ Signature: double NaV::h_inf(double V)
h_infDefinition = addMethod(NaVDefinition, ...
    "double NaV::h_inf(double V)", ...
    "MATLABName", "h_inf", ...
    "Description", "h_inf    Method of C++ class NaV."); % Modify help description values as needed.
defineArgument(h_infDefinition, "V", "double");
defineOutput(h_infDefinition, "RetVal", "double");
validate(h_infDefinition);

%% C++ class method |tau_m| for C++ class |NaV| 
% C++ Signature: double NaV::tau_m(double V)
tau_mDefinition = addMethod(NaVDefinition, ...
    "double NaV::tau_m(double V)", ...
    "MATLABName", "tau_m", ...
    "Description", "tau_m    Method of C++ class NaV."); % Modify help description values as needed.
defineArgument(tau_mDefinition, "V", "double");
defineOutput(tau_mDefinition, "RetVal", "double");
validate(tau_mDefinition);

%% C++ class method |tau_h| for C++ class |NaV| 
% C++ Signature: double NaV::tau_h(double V)
tau_hDefinition = addMethod(NaVDefinition, ...
    "double NaV::tau_h(double V)", ...
    "MATLABName", "tau_h", ...
    "Description", "tau_h    Method of C++ class NaV."); % Modify help description values as needed.
defineArgument(tau_hDefinition, "V", "double");
defineOutput(tau_hDefinition, "RetVal", "double");
validate(tau_hDefinition);

%% C++ class constructor for C++ class |NaV| 
% C++ Signature: NaV::NaV(NaV const & input1)
NaVConstructor3Definition = addConstructor(NaVDefinition, ...
    "NaV::NaV(NaV const & input1)", ...
    "Description", "clib.xolotl.NaV    Constructor of C++ class NaV."); % Modify help description values as needed.
defineArgument(NaVConstructor3Definition, "input1", "clib.xolotl.NaV", "input");
validate(NaVConstructor3Definition);

%% C++ class |Leak| with MATLAB name |clib.xolotl.Leak| 
LeakDefinition = addClass(libDef, "Leak", "MATLABName", "clib.xolotl.Leak", ...
    "Description", "clib.xolotl.Leak    Representation of C++ class Leak."); % Modify help description values as needed.

%% C++ class constructor for C++ class |Leak| 
% C++ Signature: Leak::Leak()
LeakConstructor1Definition = addConstructor(LeakDefinition, ...
    "Leak::Leak()", ...
    "Description", "clib.xolotl.Leak    Constructor of C++ class Leak."); % Modify help description values as needed.
validate(LeakConstructor1Definition);

%% C++ class constructor for C++ class |Leak| 
% C++ Signature: Leak::Leak(double gbar_)
LeakConstructor2Definition = addConstructor(LeakDefinition, ...
    "Leak::Leak(double gbar_)", ...
    "Description", "clib.xolotl.Leak    Constructor of C++ class Leak."); % Modify help description values as needed.
defineArgument(LeakConstructor2Definition, "gbar_", "double");
validate(LeakConstructor2Definition);

%% C++ class method |integrate| for C++ class |Leak| 
% C++ Signature: void Leak::integrate(double V)
integrateDefinition = addMethod(LeakDefinition, ...
    "void Leak::integrate(double V)", ...
    "MATLABName", "integrate", ...
    "Description", "integrate    Method of C++ class Leak."); % Modify help description values as needed.
defineArgument(integrateDefinition, "V", "double");
validate(integrateDefinition);

%% C++ class constructor for C++ class |Leak| 
% C++ Signature: Leak::Leak(Leak const & input1)
LeakConstructor3Definition = addConstructor(LeakDefinition, ...
    "Leak::Leak(Leak const & input1)", ...
    "Description", "clib.xolotl.Leak    Constructor of C++ class Leak."); % Modify help description values as needed.
defineArgument(LeakConstructor3Definition, "input1", "clib.xolotl.Leak", "input");
validate(LeakConstructor3Definition);

%% C++ class |Kd| with MATLAB name |clib.xolotl.Kd| 
KdDefinition = addClass(libDef, "Kd", "MATLABName", "clib.xolotl.Kd", ...
    "Description", "clib.xolotl.Kd    Representation of C++ class Kd."); % Modify help description values as needed.

%% C++ class constructor for C++ class |Kd| 
% C++ Signature: Kd::Kd()
KdConstructor1Definition = addConstructor(KdDefinition, ...
    "Kd::Kd()", ...
    "Description", "clib.xolotl.Kd    Constructor of C++ class Kd."); % Modify help description values as needed.
validate(KdConstructor1Definition);

%% C++ class constructor for C++ class |Kd| 
% C++ Signature: Kd::Kd(double gbar_)
KdConstructor2Definition = addConstructor(KdDefinition, ...
    "Kd::Kd(double gbar_)", ...
    "Description", "clib.xolotl.Kd    Constructor of C++ class Kd."); % Modify help description values as needed.
defineArgument(KdConstructor2Definition, "gbar_", "double");
validate(KdConstructor2Definition);

%% C++ class method |m_inf| for C++ class |Kd| 
% C++ Signature: double Kd::m_inf(double V)
m_infDefinition = addMethod(KdDefinition, ...
    "double Kd::m_inf(double V)", ...
    "MATLABName", "m_inf", ...
    "Description", "m_inf    Method of C++ class Kd."); % Modify help description values as needed.
defineArgument(m_infDefinition, "V", "double");
defineOutput(m_infDefinition, "RetVal", "double");
validate(m_infDefinition);

%% C++ class method |tau_m| for C++ class |Kd| 
% C++ Signature: double Kd::tau_m(double V)
tau_mDefinition = addMethod(KdDefinition, ...
    "double Kd::tau_m(double V)", ...
    "MATLABName", "tau_m", ...
    "Description", "tau_m    Method of C++ class Kd."); % Modify help description values as needed.
defineArgument(tau_mDefinition, "V", "double");
defineOutput(tau_mDefinition, "RetVal", "double");
validate(tau_mDefinition);

%% C++ class constructor for C++ class |Kd| 
% C++ Signature: Kd::Kd(Kd const & input1)
KdConstructor3Definition = addConstructor(KdDefinition, ...
    "Kd::Kd(Kd const & input1)", ...
    "Description", "clib.xolotl.Kd    Constructor of C++ class Kd."); % Modify help description values as needed.
defineArgument(KdConstructor3Definition, "input1", "clib.xolotl.Kd", "input");
validate(KdConstructor3Definition);

%% Validate the library definition
validate(libDef);

end
