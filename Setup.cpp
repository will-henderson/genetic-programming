//
//  Setup.cpp
//  Genetic Programming
//
//  Created by William Henderson on 07/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "Setup.hpp"

Setup::Setup(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_): types(types_), probabilityfvr(probabilityfvr_), numberofthis(numberofthis_), maxInitialLength(maxInitialLength_), returntypeofindividuals(returntypeofindividuals_), crossoverRate(crossoverRate_){}

Node* Setup::randomNode(int thisLength, std::type_index type){
    
    int locationoftype;
    for(locationoftype=0; types[locationoftype] != type; locationoftype++); //find locations of the type to return in the vector
    
    double randomA = (RAND_MAX-rand())/static_cast<double>(RAND_MAX);
    
    if(thisLength >= maxInitialLength && (numberofthis[locationoftype][1] != 0 || numberofthis[locationoftype][2] != 0)){
        if(numberofthis[locationoftype][1] == 0){
            return newConstNode(type, thisLength);
        }
        else if(numberofthis[locationoftype][2] == 0){
            return newVarNode(type, thisLength);
        }
        
        double sum = 0;
        for(int i=1; i<3; i++){
            sum +=probabilityfvr[locationoftype][i];
        }
        
        randomA = randomA*sum;

        if(probabilityfvr[locationoftype][1]>=randomA){
            return newVarNode(type, thisLength);
        }
        else{
            return newConstNode(type, thisLength);
        }
    }
    else{
        double sum=0;
        for(int i=0; i<4; i++){
            sum +=probabilityfvr[locationoftype][i];
        }
        randomA = randomA*sum;
        
        if(probabilityfvr[locationoftype][0]>=randomA) return newIntFncNode(type, thisLength);
        else if(probabilityfvr[locationoftype][1]+probabilityfvr[locationoftype][0]>=randomA) return newVarNode(type, thisLength);
        else if(probabilityfvr[locationoftype][2]+probabilityfvr[locationoftype][1]+probabilityfvr[locationoftype][0]>=randomA) return newConstNode(type, thisLength);
        else return newADFNode(type, thisLength);
    }
}
