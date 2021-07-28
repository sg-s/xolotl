

#include "thing.hpp"
#include "mycontainer.hpp"

void thing::resetContainerValue() {
    container->value = 0;
}



void mycontainer::init() {

    // broadcast parameters to things
    if (this->n_things == 0) {
        return;
    }

    for (int i = 0; i < this->n_things; i++) {
        things[i]->value = this->value;
    }
}



void mycontainer::addThing(thing * thing_) {
    this->things.push_back(thing_);
    this->n_things++;
    thing_->container = this;
}
