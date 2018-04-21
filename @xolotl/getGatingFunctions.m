%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: recover gating functions from CPP file
%

function [m_inf, h_inf, tau_m, tau_h] =  getGatingFunctions(conductance)

C = cpplab(conductance);

m_inf = @(V) 0;
h_inf = @(V) 1;
tau_m = @(V) NaN;
tau_h = @(V) NaN;

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