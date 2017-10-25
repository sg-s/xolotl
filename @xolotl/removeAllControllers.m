%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% removes all controllers from xolotl object
%
function removeAllControllers(self)
	self.controllers = {};
	self.transpile;
	self.compile;
end