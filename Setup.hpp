//
//  Setup.hpp
//  Genetic Programming
//
//  Created by William Henderson on 07/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef Setup_hpp
#define Setup_hpp

#include <stdio.h>
#include <vector>
#include <typeindex>

class Individual;
class Node;

class Setup{
    
    virtual Node* newIntFncNode(std::type_index type, int thisLength)=0; //randomly chooses function
    virtual Node* newVarNode(std::type_index type, int thisLength)=0; //randomly chooses var
    virtual Node* newConstNode(std::type_index type, int thisLength)=0; //randomly chooses val
    virtual Node* newADFNode(std::type_index type, int thisLength)=0;
    
public:
   
    std::vector<std::type_index> types;
    std::vector<std::vector<double> > probabilityfvr; //probability of funcNode, varNode, rcnNode, autFnc;
    std::vector<std::vector<int> > numberofthis;
    
    int maxInitialLength;
    std::type_index returntypeofindividuals;
    
    double crossoverRate;
    
    Setup(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_);
    Node* randomNode(int thislength, std::type_index type); //this function exists so that the types of IntFncNode (e.g. bool, int, double) to be used can be compiled in the application part;
    
    virtual double fitness(std::vector<Individual>::iterator begin, std::vector<Individual>::iterator end)=0;
    
};

#endif /* Setup_hpp */
