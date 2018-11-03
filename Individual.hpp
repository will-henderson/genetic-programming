//
//  Individual.hpp
//  Genetic Programming
//
//  Created by William Henderson on 05/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef Individual_hpp
#define Individual_hpp

#include <stdio.h>
#include "Node.hpp"
#include <ostream>

class Individual : public Node{
    
    double fitness;
    double normalisedFitness;
    double selectionNumber;
    
public:
    
    Individual(Setup* igp); //constructor for random individual
    
    Node* virtualConstructor();
    
    void value(void* toreturn) const;
    void setFitness(double fitnesS);
    void normaliseFitness(double totalFitness);
    double getFitness();
    double setSelectionNumber(double currentSelectionNumber); //Sets the selection number to current selection number and returns currentSelectionNumber + fitness
    double getSelectionNumber() const;
    Node* simplify();
};

#endif /* Individual_hpp */
