//
//  ADFSetup.cpp
//  Genetic Programming
//
//  Created by William Henderson on 19/08/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "ADFSetup.hpp"

ADFSetup::ADFSetup(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_): Setup(types_, probabilityfvr_, numberofthis_, maxInitialLength_, returntypeofindividuals_, crossoverRate_){} //Setting the void* vector left to subclass

double ADFSetup::fitness(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end){
    double totalFitness=0;
    return totalFitness;
}
