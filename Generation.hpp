//
//  Generation.hpp
//  Genetic Programming
//
//  Created by William Henderson on 06/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef Generation_hpp
#define Generation_hpp

#include <stdio.h>
#include <vector>
#include "Individual.hpp"
#include "Setup.hpp"

class Generation{
    
    //parameters of the Generation
    int size;
    int numberToCopy;
    int MaxBreedingLength;
    Setup *setup;
    
   // Individual *generation;
    std::vector<Individual> generation;
    
    //Selects an individual from the generation with probability based on fitness, marks as selected.
    Individual fpSelect(int& i) const; //same as above but sets i to the position of the selected individual
    
    void crossover(Individual& a, Individual& b) const;
    
    
public:
    
    Generation(int numberOfIndividuals, double proportionToCopy, int MaxBreedingLength, Setup* setup);
    //constructor for random initial population
    
    void normalisedFitness(bool copy);
    //sets the normalised fitness of all the individuals
    
    void nextGeneration();
    //breeds the individuals for the next generation;
    
    void nextGeneration(std::vector<int>& i); //if empty, records the numbers of individuals pushed back into the new generation,
                                              //if not empty, reproduces these individuals for the next generation
    Individual bestOfGeneration();
    Individual* getIndividualPtr(int i);
    std::vector<double> getAllFitness();
    double avgFitness();
    double maxFitness();
    
};



#endif /* Generation_hpp */
