classdef xtest < matlab.unittest.TestCase
    

properties
    
    % xolotl object
    x@xolotl

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

        self.x = x;


    end
end
    
methods (Test)
    
    testXolotlAdd(self)


end % test
    
end