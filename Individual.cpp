//
//  Individual.cpp
//  Genetic Programming
//
//  Created by William Henderson on 05/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "Individual.hpp"

Individual::Individual(Setup* s): Node(0,s, {s->returntypeofindividuals}, s->returntypeofindividuals){number = -1;} 

Node* Individual::virtualConstructor(){
    return new Individual(*this);
}

void Individual::value(void* toreturn) const{
    children[0]->value(toreturn);
}

void Individual::setFitness(double fitnesS){
    fitness = fitnesS;
}

double Individual::getFitness(){
    return fitness;
}

void Individual::normaliseFitness(double totalFitness){
    normalisedFitness = fitness / totalFitness;
}

double Individual::setSelectionNumber(double currentSelectionNumber){
    selectionNumber = currentSelectionNumber;
    return selectionNumber + normalisedFitness;
}

double Individual::getSelectionNumber() const{
    return selectionNumber;
}

Node* Individual::simplify(){
    children[0]->simplify();
    return this;
}
