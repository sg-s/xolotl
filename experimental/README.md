# experimental C++ interface for xolotl


MATLAB's C++ interface allows us to generate a dynamic library 
from C++ code and talk to that code directly from within MATLAB. 
Objects that appear in MATLAB are defined solely by their C++
code, and therefore it is possible to write a library in C++
and have it magically work in MATLAB. Sounds neat. 

To test this, you will have to build this library yourself 
(though in production one expects to ship it pre-built). 

```matlab
build;
```

Now let's test it

```matlab

% import the C++ library. simplifies namespaces
import clib.xolotl.*

AB = compartment("AB");
AB = compartment("AB");
nav = NaV(1e3);
kd = Kd(300);
leak = Leak(1);
```

That's it, now we have fully-specified objects that we can 
interact with, modify, etc.:

```matlab
AB

AB = 

  compartment with properties:

             A: 0.0628
            Cm: 10
            dt: 0.1000
        sim_dt: 0.1000
             V: -60
        V_prev: -60
       n_steps: 1000
            Ca: 0.0100
       Ca_prev: 0.0100
         I_ext: 0
        V_data: [1×1000 double]
          name: "AB"
    n_channels: 0

```


The advantage of the C++ interface is that these are real objects--not fake pure MATLAB objects masquerading as C++ objects as cpplab allows. This means we can use C++ features like pointers to assemble the model:

```matlab
AB.add(nav)
AB.add(kd)
AB.add(leak)
```

And we can of course do things like set things:

```matlab
AB.I_ext = .5;
```


And we can of course integrate and plot:

```matlab
AB.integrate;
plot((1:AB.n_steps)*AB.dt,AB.V_data)
```

![](https://user-images.githubusercontent.com/6005346/127376671-4e8686d2-8063-4a5e-bad2-d913897cff0e.jpg)