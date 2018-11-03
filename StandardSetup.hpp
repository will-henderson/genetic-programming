//
//  StandardSetup.hpp
//  Genetic Programming
//
//  Created by William Henderson on 07/07/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef StandardSetup_hpp
#define StandardSetup_hpp

#include <stdio.h>
#include "Setup.hpp"

class StandardSetup: public Setup{
    
public:
    StandardSetup(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate);

    virtual double test(Individual& toTest)=0; //returns adjusted fitness and sets the individuals fitness to this also
    
    double fitness(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end);
};

#endif /* StandardSetup_hpp */
