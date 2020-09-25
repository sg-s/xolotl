
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### getGatingFunctions
%
%
% **Syntax**
%
% ```matlab
% [m_inf, h_inf, tau_m, tau_h] =  xolotl.getGatingFunctions(conductance)
% ```
%
% **Description**
%
% static method of `xolotl` that returns function handles
% that represent the gating and activation functions of a
% particular conductance.
%
%
% `conductance` is a string that specifies a
% conductance C++ header file. The outputs are function
% handles that can be evaluated independently. This method
% is used internally in `xolotl.show()`
%
%
% This method supports tab-completion. You should be able to press
% tab to get a list of conductances you can get the
% gating function of.
%
%
% !!! warning
% XXXXThis may return function handles that are incorrect or malformed for some channels. This is only guaranteed to work when activation functions are simply defined inline in the C++ code. 
%
% See Also: 
% xolotl.show
%



function [m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance)

C = cpplab(conductance);

m_inf = @(V,Ca) NaN;
h_inf = @(V,Ca) NaN;
tau_m = @(V,Ca) NaN;
tau_h = @(V,Ca) NaN;

for i = 1:length(C.cpp_child_functions)
    if strcmp(C.cpp_child_functions(i).fun_name,'m_inf')
        m_inf = C.cpp_child_functions(i).fun_handle;
    elseif strcmp(C.cpp_child_functions(i).fun_name,'h_inf')
        h_inf = C.cpp_child_functions(i).fun_handle;
    elseif strcmp(C.cpp_child_functions(i).fun_name,'tau_m')
        tau_m = C.cpp_child_functions(i).fun_handle;
    elseif strcmp(C.cpp_child_functions(i).fun_name,'tau_h')
        tau_h = C.cpp_child_functions(i).fun_handle;
    end
end


% vectorize all function handles
m_inf = strrep(func2str(m_inf),'/','./');
m_inf = strrep(m_inf,'*','.*');
m_inf = str2func(m_inf);

h_inf = strrep(func2str(h_inf),'/','./');
h_inf = strrep(h_inf,'*','.*');
h_inf = str2func(h_inf);

tau_m = strrep(func2str(tau_m),'/','./');
tau_m = strrep(tau_m,'*','.*');
tau_m = str2func(tau_m);

tau_h = strrep(func2str(tau_h),'/','./');
tau_h = strrep(tau_h,'*','.*');
tau_h = str2func(tau_h);
