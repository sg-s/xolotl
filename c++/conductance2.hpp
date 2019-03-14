


/*
This file implements additional methods to the conductance
class. The reason it exists in a different file is because
methods here refer to members of compartment, and this is 
one way I break circular dependencies in the code. 
*/


// #include <chrono>



void conductance::integrateMT() {


	steps_left = container->steps_left;

    while (true) {

    	// std::this_thread::sleep_for(std::chrono::milliseconds(1));

    	integrate(container->V_prev, container->Ca_prev);

    	// if (steps_left >= container->steps_left) {
    	// 	integrate(container->V_prev, container->Ca_prev);
    	// 	steps_left--;
    	// }


    	if (container->steps_left == 0) {
    		return;
    	}

    }
}

