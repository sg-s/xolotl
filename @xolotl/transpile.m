%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% MATLAB -> C++ transpiler
% creates a C++ file that can be compiled with mex
% this is a thin wrapper around the actual transpiler
% in transpileCore

function transpile(self)
	h = self.hash;
	out_file = ['mexBridge' h(1:6) '.cpp'];
	self.transpileCore('mexTemplate.cpp',out_file)

	out_file = ['mexBridge' h(1:6) 'NOCL.cpp'];
	self.transpileCore('mexTemplateNOCL.cpp',out_file)
	
end % end transpile
