
%               _       _   _ 
%    __  _____ | | ___ | |_| |
%    \ \/ / _ \| |/ _ \| __| |
%     >  < (_) | | (_) | |_| |
%    /_/\_\___/|_|\___/ \__|_|
%
% ### time
%
% **Syntax**
%
% ```matlab
% plot(x.time,x.integrate)
% ```
%
% This helper method generates a time vector for your plots in seconds. 
%
%
% See Also: 
% xolotl.integrate

function T = time(self)

T = (1:(self.t_end/self.dt))*self.dt*1e-3;