//
//  TournamentSelectionSetup.hpp
//  Genetic Programming
//
//  Created by William Henderson on 07/07/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef TournamentSelectionSetup_hpp
#define TournamentSelectionSetup_hpp

#include <stdio.h>
#include "Setup.hpp"

class TournamentSelectionSetup :public Setup{

public:
    int numberOfBattles;
    
    TournamentSelectionSetup(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_, int numberOfBattles_);
    
    double fitness(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end);
    
    virtual double test(Individual& toTest1, Individual& toTest2)=0;
    
};


#endif /* TournamentSelectionSetup_hpp */

