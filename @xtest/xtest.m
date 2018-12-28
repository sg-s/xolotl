classdef xtest < matlab.unittest.TestCase
    

properties
    
    % xolotl objects
    single_comp@xolotl

    stg_network@xolotl



end



methods(TestMethodSetup)
    function createSingleCompartment(self)
        % 
        x = xolotl;

        A = 0.0628;

        channels = {'NaV','CaT','CaS','ACurrent','KCa','Kd','HCurrent'};
        prefix = 'prinz/';
        gbar(:,1) = [1000 25  60 500  50  1000 .1];
        E =         [50   30  30 -80 -80 -80   -20];

        x = xolotl;

        x.add('compartment','AB','Cm',10,'A',A);

        % add Calcium mechanism
        x.AB.add('prinz/CalciumMech');

        for i = 1:length(channels)
            x.AB.add([prefix channels{i}],'gbar',gbar(i),'E',E(i));
        end

        x.t_end = 1e4;
        x.integrate;
        x.t_end = 5e3;

        self.single_comp = x;


    end



    function createSTGNetwork(self)

        A = 0.0628;

        channels = {'NaV','CaT','CaS','ACurrent','KCa','Kd','HCurrent'};
        prefix = 'prinz/';
        gbar(:,1) = [1000 25  60 500  50  1000 .1];
        gbar(:,2) = [1000 0   40 200  0   250  .5];
        gbar(:,3) = [1000 24  20 500  0   1250 .5];
        E =         [50   30  30 -80 -80 -80   -20];

        x = xolotl;

        x.add('compartment','AB','Cm',10,'A',A);
        x.add('compartment','LP','Cm',10,'A',A);
        x.add('compartment','PY','Cm',10,'A',A);

        compartments = x.find('compartment');
        for j = 1:length(compartments)

            % add Calcium mechanism
            x.(compartments{j}).add('prinz/CalciumMech');

            for i = 1:length(channels)
                x.(compartments{j}).add([prefix channels{i}],'gbar',gbar(i,j),'E',E(i));
            end
        end

        x.LP.add('Leak','gbar',.3,'E',-50);
        x.PY.add('Leak','gbar',.1,'E',-50);

        x.connect('AB','LP','Chol');

        % set up synapses as in Fig. 2e

        x.connect('AB','PY','Chol','gmax',3);
        x.connect('AB','LP','Glut','gmax',30);
        x.connect('AB','PY','Glut','gmax',10);
        x.connect('LP','PY','Glut','gmax',1);
        x.connect('PY','LP','Glut','gmax',30);
        x.connect('LP','AB','Glut','gmax',30);

        x.t_end = 5e3;

        self.stg_network = x;


    end
end
    
methods (Test)
    
    testXolotlAdd(self)
    testGoToExamples(self)
    testRebase(self)


end % test
    
end