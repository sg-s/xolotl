%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### matrixCost

**Syntax**

```matlab
C = xtools.matrixCost(M1, M2)
```

**Description**

Compute the norm-squared distance between two matrices. If the matrices are LeMasson
matrices, which represent discretized probability distributions of a derivative-embedded
attractor of a voltage trace, then this distance serves as a measure of how dissimilar
the two voltage traces are.

!!! info "See Also"
    ->xtools.V2Matrix
		LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting. In: De Schutter E (ed) Computational Neuroscience: Realistic Modeling for Experimentalists. CRC Press, London pp 1–21


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
