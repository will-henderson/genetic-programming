//
//  StandardSetup.cpp
//  Genetic Programming
//
//  Created by William Henderson on 07/07/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "StandardSetup.hpp"
#include "Individual.hpp"

StandardSetup::StandardSetup(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_): Setup(types_, probabilityfvr_, numberofthis_, maxInitialLength_, returntypeofindividuals_, crossoverRate_){
}

double StandardSetup::fitness(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end){
    
    double totalFitness=0;
    std::vector<Individual>::iterator iterator = begin;
    
    while(iterator!=end){
        totalFitness += test(*iterator);
        iterator++;
    }

    return totalFitness;
}
