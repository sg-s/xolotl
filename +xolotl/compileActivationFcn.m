
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### compileActivationFcn
%
%
% **Syntax**
%
% ```matlab
% binary_name = compileActivationFcn(conductance)
% ```
%
% **Description**
%
% This function compiles a binary that evaluates activation functions
% in some conductance
%
% See Also: 
% xolotl.compile
%

function binary_name = compileActivationFcn(conductance, cpp_folder)

xolotl_folder = fileparts(cpp_folder);
binary_loc = filelib.cachePath('xolotl');

binary_name = ['A_' conductance.hash];
mexBridge_name = fullfile(binary_loc, [binary_name '.cpp']);

% check if its already been compiled
if exist(fullfile(binary_loc,[binary_name '.' mexext]),'file') == 3
	return
end


% check that conductance exists 
corelib.assert(exist(conductance.cpp_class_path)==2,'Cannot file C++ header file')


% read lines from template
lines = filelib.read(fullfile(cpp_folder,'activationFcnTemplate.cpp'));


header_files{7} = conductance.cpp_class_path;

% insert network header and other critical headers
% the specific ordering here is EXTREMELY important
% and goes a long way in resolving circular dependencies 
header_files{1} = fullfile(cpp_folder,'conductance.hpp');
header_files{2} = fullfile(cpp_folder,'synapse.hpp');
header_files{3} = fullfile(cpp_folder,'mechanism.hpp');
header_files{4} = fullfile(cpp_folder,'compartment.hpp');
header_files{5} = fullfile(cpp_folder,'network.hpp');
header_files{6} = fullfile(cpp_folder,'extra_methods.hpp');




for i = 1:length(header_files)
	header_files{i} = strcat('#include "',header_files{i}, '"');
end

insert_here = filelib.find(lines,'//xolotl:include_headers_here');
corelib.assert(length(insert_here)==1,'Could not find insertion point for headers')
lines = [lines(1:insert_here); header_files(:); lines(insert_here+1:end)];



n_inputs = length(conductance.cpp_constructor_signature);
constructor = [conductance.cpp_class_name ' ThisChannel('];





for i = 1:n_inputs
	constructor = [constructor 'params[' mat2str(i-1) '],'];

end
constructor(end) = ')';
constructor = [constructor ';'];

insert_here = filelib.find(lines,'//xolotl:construct_channel_here');
corelib.assert(length(insert_here)==1,'Could not find insertion point for headers')
lines = [lines(1:insert_here); constructor; lines(insert_here+1:end)];



filelib.write(mexBridge_name,lines);


% compile it
% tell mex about where to look for C++ files

if ispc
	ipath = ['-I' xolotl_folder '/c++/'];
else
	ipath = ['-I"' xolotl_folder '/c++/"'];
end

if isunix && ~ismac
	warning('off','MATLAB:mex:GccVersion');
end

mex('-silent',ipath,mexBridge_name,'-outdir',binary_loc)


if isunix && ~ismac
	warning('on','MATLAB:mex:GccVersion');
end


