//
//  ADFSetup.hpp
//  Genetic Programming
//
//  Created by William Henderson on 19/08/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef ADFSetup_hpp
#define ADFSetup_hpp

#include <stdio.h>
#include "Setup.hpp"

class ADFSetup: public Setup{
protected:
    std::vector<void* > childrenVars;
    
public:
    ADFSetup(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_);
    
    virtual std::vector<void*> setChildrenVars()=0;
    
    double fitness(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end);
    
};


#endif /* ADFSetup_hpp */
