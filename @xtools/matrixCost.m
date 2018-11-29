%{
           _       _   _
__  _____ | | ___ | |_| |
\ \/ / _ \| |/ _ \| __| |
>  < (_) | | (_) | |_| |
/_/\_\___/|_|\___/ \__|_|

matrixCost
^^^^^^^^^^
a static method to compute the distance between two LeMasson matrices. This is a useful way to determine how similar two voltage traces are.

Usage ::

	C = matrixCost(M1,M2)

where ``M1`` and ``M2`` are two matrices returned by xolotl.V2matrix() and represent discretized probability distributions of a derivative-embedded attractor of the voltage trace.


See Also
--------

LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting. In: De Schutter E (ed) Computational Neuroscience: Realistic Modeling for Experimentalists. CRC Press, London pp 1–21

- xtools.V2matrix

%}


function C = matrixCost(M1,M2)

assert(size(M1,2) == size(M2,2),'matrices not same size')
assert(size(M1,1) == size(M2,1),'matrices not same size')

N1 = sum(M1(:));
N2 = sum(M2(:));

C = 0;

for i = 1:size(M1,1)
	for j = 1:size(M1,2)
		C = C + (M1(i,j)/N1 - M2(i,j)/N2)^2;
	end
end
