%{
           _       _   _
__  _____ | | ___ | |_| |
\ \/ / _ \| |/ _ \| __| |
>  < (_) | | (_) | |_| |
/_/\_\___/|_|\___/ \__|_|

V2matrix
^^^^^^^^
a static method that converts a voltage trace into a LeMasson matrix.  Usage ::

   [M, V_lim, dV_lim] = V2matrix(V, V_lim, dV_lim)

where V is a vector (a voltage time series), and ``V_lim`` and ``dV_lim`` are two-element vectors that specify the lower and upper bounds of ``V`` and ``dV``

This static method allows you to create a delay-embedding of a voltage trace, and then discretize the space and count the number of points in each bin. The resultant matrix is sometimes called a LeMasson matrix. ``M`` is the LeMasson matrix, which is always of size ``101x101``.

If you do not specify ``V_lim`` and ``dV_lim``, they will be computed automatically and returned.

See Also
--------

LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting. In: De Schutter E (ed) Computational Neuroscience: Realistic Modeling for Experimentalists. CRC Press, London pp 1–21

- xtools.matrixCost

%}



function [M, V_lim, dV_lim] = V2matrix(V, V_lim, dV_lim)

assert(isvector(V),'V has to be a vector')

V = V(:);
dV = [NaN; diff(V)];

if nargin == 1
	% return V_lim and dV_lim too
	m = abs(max(V));
	V_lim(2) = round(m,-floor(log10(m))) + 10^floor(log10(m));
	m = abs(min(V));
	V_lim(1) = round(m,-floor(log10(m))) + 10^floor(log10(m));
	if min(V) < 0
		V_lim(1) = -V_lim(1);
	end
	if max(V) < 0
		V_lim(2) = -V_lim(2);
	end

	% dV_lim
	m = abs(max(dV));
	dV_lim(2) = round(m,-floor(log10(m))) + 10^floor(log10(m));
	m = abs(min(dV));
	dV_lim(1) = round(m,-floor(log10(m))) + 10^floor(log10(m));
	if min(dV) < 0
		dV_lim(1) = -dV_lim(1);
	end
	if max(dV) < 0
		dV_lim(2) = -V_lim(2);
	end

end

assert(isvector(V_lim),'V_lim has to be a vector')
assert(isvector(dV_lim),'dV_lim has to be a vector')
assert(length(V_lim) == 2,'size of V_lim has to be 2x1')
assert(length(dV_lim) == 2,'size of dV_lim has to be 2x1')



% overflow
V(V<V_lim(1)) = NaN;
V(V>V_lim(2)) = NaN;
dV(dV<dV_lim(1)) = NaN;
dV(dV>dV_lim(2)) = NaN;


% discetize traces
V = V - V_lim(1);
V = V/(V_lim(2) - V_lim(1));
V = ceil(V*99)+1;

dV = dV - dV_lim(1);
dV = dV/(dV_lim(2) - dV_lim(1));
dV = ceil(dV*99)+1;

M = zeros(101,101);
% M(101,101) is used to collect overflow terms


% other way
for i = 1:length(V)
	if isnan(V(i))
		M(101,101) = M(101,101) + 1;
		continue
	end
	if isnan(dV(i))
		M(101,101) = M(101,101) + 1;
		continue
	end
	M(V(i),dV(i)) = M(V(i),dV(i)) + 1;
end
