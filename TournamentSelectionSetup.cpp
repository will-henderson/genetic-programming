//
//  TournamentSelectionSetup.cpp
//  Genetic Programming
//
//  Created by William Henderson on 07/07/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "TournamentSelectionSetup.hpp"
#include "Individual.hpp"

TournamentSelectionSetup::TournamentSelectionSetup(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_, int numberOfBattles_): Setup(types_, probabilityfvr_, numberofthis_, maxInitialLength_, returntypeofindividuals_, crossoverRate_){

    numberOfBattles = numberOfBattles_;
}

double TournamentSelectionSetup::fitness(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end){
    
    int totalFitness = 0;
    std::vector<Individual>::iterator iterator = begin;
    int size = 0;
    
    while(iterator!=end){
        iterator->setFitness(0);
        iterator++;
        size++;
    }
    
    std::vector<int> numbers;
    numbers.reserve(size);
    for(int i=0; i<numberOfBattles; i++){
        for(int j=0; j<size; j++){
            numbers.push_back(j);
        }
        for(int j=size; j>0; j-=2){
            int randomA = rand()%j;
            int randomB = rand()%(j-1);
            totalFitness += test(*(begin+numbers[randomA]), *(begin+numbers[randomB]));
            
            numbers.erase(numbers.begin() + randomA); numbers.erase(numbers.begin() + randomB);
        }
    }
    return totalFitness;
}
