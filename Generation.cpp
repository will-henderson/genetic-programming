 //
//  Generation.cpp
//  Genetic Programming
//
//  Created by William Henderson on 06/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include <math.h>
#include <cstdlib>
#include <algorithm>
#include "Generation.hpp"
#include "Node.hpp"
#include "StandardSetup.hpp"
#include "TournamentSelectionSetup.hpp"

Generation::Generation(int numberOfIndividuals, double propToCopy, int MaxBreedLength, Setup* setuP){
    
    size = numberOfIndividuals;
    numberToCopy = size - 2*round(0.5*size*(1-propToCopy)); //rounds up or down to ensure an even number of remaining individuals
    MaxBreedingLength = MaxBreedLength;
    setup = setuP;
    
    generation.reserve(numberOfIndividuals);
    for(int i=0; i<numberOfIndividuals; i++){
        generation.push_back(Individual(setup));
    }
}


void Generation::normalisedFitness(bool copy){
    
    double totalFitness = 0;
    
    int n;
    if(copy){
        n = numberToCopy;
    }
    else{
        n=0;
    }
    
    for(int i=0; i<n; i++){
        totalFitness += generation[i].getFitness();
    }

    totalFitness += setup->fitness(generation.begin()+n, generation.end());
    
    for(int i=0; i<size; i++){
        generation[i].normaliseFitness(totalFitness);
    }
}

void Generation::nextGeneration(){
    
    std::vector<Individual> temp; //array for new generation to be placed in
    temp.reserve(size);

    double currentNumber = 0; //give each individual a selection number based on their fitness
    for(int i=0; i<size; i++){
        currentNumber = generation[i].setSelectionNumber(currentNumber);
    }
    
    for(int i=0; i<size; i++){ //copy numberToCopy individuals, probability based on fitness
        int j;
        temp.push_back(fpSelect(j));
    }

    for(int i = numberToCopy; i<size; i+=2){ //breed individuals
        crossover(temp[i], temp[i+1]);
    }
    
    generation = temp;
}

void Generation::nextGeneration(std::vector<int>& order){
    
    if(order.empty()){
        std::vector<Individual> temp; //array for new generation to be placed in
        temp.reserve(size);
        
        double currentNumber = 0; //give each individual a selection number based on their fitness
        for(int i=0; i<size; i++){
            currentNumber = generation[i].setSelectionNumber(currentNumber);
        }
        
        for(int i=0; i<size; i++){ //copy numberToCopy individuals, probability based on fitness
            int j;
            temp.push_back(fpSelect(j));
            order.push_back(j);
        }
        
        for(int i = numberToCopy; i<size; i+=2){ //breed individuals
            crossover(temp[i], temp[i+1]);
        }
        
        generation = temp;
    }
    else{
        std::vector<Individual> temp; //array for new generation to be placed in
        temp.reserve(size);
        
        for(int i=0; i<order.size(); i++){
            temp.push_back(generation[order[i]]);
        }
        
        for(int i = numberToCopy; i<size; i+=2){ //breed individuals
            crossover(temp[i], temp[i+1]);
        }
        generation = temp;
    }
}

Individual Generation::fpSelect(int& middle) const{
    
    double random = (RAND_MAX-rand())/static_cast<double>(RAND_MAX);
    int loPos = 0;
    int hiPos = size-1;
    
    while(true){
        middle = (loPos + hiPos)/2;
        
        if(random >= generation[middle].getSelectionNumber()){
            if(middle == size-1 || random < generation[middle+1].getSelectionNumber()){
                return generation[middle];
            }
            else{
                loPos = middle+1;
            }
        }
        else{
            hiPos = middle-1;
        }
    }
}

void Generation::crossover(Individual& a, Individual& b) const{

    if(setup->crossoverRate>=0){
        int noTypes = setup->types.size();
    
        std::vector<unsigned long> i0; std::vector<unsigned long> i1; //number the nodes
        i0.reserve(noTypes); i1.reserve(noTypes);
    
        for(int j=0; j<noTypes; j++){i0.push_back(0); i1.push_back(0);}
        a.getChild(0)->numberNodes(i0, setup->types); b.getChild(0)->numberNodes(i1, setup->types); //after this i = number of nodes
    
        //calculate number of crossovers for each individual
        int noPoints0, noPoints1;
        if(setup->crossoverRate>=1){ //constant number of crossovers per individual
            noPoints0 = setup->crossoverRate; //rounds down
            noPoints1 = setup->crossoverRate;
        }
        else{
            int noNodes0=0, noNodes1=0;
            for(int j=0; j<noTypes; j++){
                noNodes0+=i0[j];
                noNodes1+=i1[j];
            }
            noPoints0 = noNodes0*setup->crossoverRate;
            noPoints1 = noNodes1*setup->crossoverRate;
        }
        
        //there is room for some creativity about what to do if noPoints0 != noPoints1
        int noPoints = (noPoints0 + noPoints1)/2;
        if(noPoints==0){
            noPoints=1; //should have at least one cross over point;
        }
        
    
        //Find a type which is in both individuals, if there is no such type, just return the two individuals;
        
        //first randomly select noPoints types , or less if there are only a limited number of nodes of right type
        std::vector<int> noOfEachTypeSelected;
        for(int j=0; j<noTypes; j++){noOfEachTypeSelected.push_back(0);}
        
        bool moreSwitchPoints = true;
        for(int j=0; j<noPoints && moreSwitchPoints; j++){
            moreSwitchPoints=false;
            int k = rand();
            for(int l=0; l<noTypes; l++){
                int m = k%noTypes;
                if(i0[m]>noOfEachTypeSelected[m] && i1[m]>noOfEachTypeSelected[m]){
                    noOfEachTypeSelected[m]++;
                    moreSwitchPoints = true;
                    break;
                }
                k++;
            }
        }
        
        //randomly select nodes of the chosen type from each individual;
        std::vector<Node*> cp0, cp1;
        
        for(int j=0; j<noTypes; j++){
            
            std::vector<int> alreadyChosenNumbers0;
            alreadyChosenNumbers0.reserve(noOfEachTypeSelected[j]);
            std::vector<int> alreadyChosenNumbers1;
            alreadyChosenNumbers1.reserve(noOfEachTypeSelected[j]);
            for(int k=0; k<noOfEachTypeSelected[j]; k++){ //problem: same node could be selected twice
                //randomly choose
                int l0 = rand();
                while(true){
                    int chosenNumber = l0%i0[j];
                    bool alreadyChosen = false;
                    for(int i=0; i<alreadyChosenNumbers0.size(); i++){
                        if(chosenNumber == alreadyChosenNumbers0[i]){
                            alreadyChosen=true;
                            break;
                        }
                    }
                    if(!alreadyChosen){
                        alreadyChosenNumbers0.push_back(chosenNumber);
                        cp0.push_back(a.getChild(0)->numberToNode(chosenNumber, setup->types[j]));
                        break;
                    }
                    l0++;
                }
                
                int l1 = rand();
                while(true){
                    int chosenNumber = l1%i1[j];
                    bool alreadyChosen = false;
                    for(int i=0; i<alreadyChosenNumbers1.size(); i++){
                        if(chosenNumber == alreadyChosenNumbers1[i]){
                            alreadyChosen=true;
                            break;
                        }
                    }
                    if(!alreadyChosen){
                        alreadyChosenNumbers1.push_back(chosenNumber);
                        cp1.push_back(b.getChild(0)->numberToNode(chosenNumber, setup->types[j]));
                        break;
                    }
                    l1++;
                }
            }
        }
        
        //ensure swaps do not happen if the crossover point is on a branch that will have already been swapped
        
        std::vector<std::vector<bool> > isBelow0, isBelow1;
    
        for(int i=0; i<cp0.size(); i++){
            std::vector<bool> temp0, temp1;
            for(int j=0; j<i; j++){
                temp0.push_back(cp0[j]->isBelow(cp0[i], &(a)));
                temp1.push_back(cp1[j]->isBelow(cp1[i], &(b)));
            }
            temp0.push_back(0);
            temp1.push_back(0);
            for(int j=i+1; j<cp0.size(); j++){
                temp0.push_back(cp0[j]->isBelow(cp0[i], &(a)));
                temp1.push_back(cp1[j]->isBelow(cp1[i], &(b)));
            }
            isBelow0.push_back(temp0);
            isBelow1.push_back(temp1);
        }
        
        std::vector<Node*> cp0ordered , cp1ordered;
        
        int size = cp0.size();
        
        for(int k=size; k>0; k--){
            bool noPointPossible = true;
            for(int i=0; i<k; i++){
                bool belowNone = true;
                for(int j=0; j<size; j++){
                    if(isBelow0[i][j] || isBelow1[i][j]){
                        belowNone = false;
                        break;
                    }
                }
                
                if(belowNone){
                    cp0ordered.push_back(cp0[i]);
                    cp1ordered.push_back(cp1[i]);
                    isBelow0.erase(isBelow0.begin() + i);
                    isBelow1.erase(isBelow1.begin() + i);
                    cp0.erase(cp0.begin() + i);
                    cp1.erase(cp1.begin() + i);
                    noPointPossible = false;
                    break;
                }
            }
            if(noPointPossible){ //get rid of one of the crossover points
                //randomly choose one to remove;
                int l=rand()%k;
                isBelow0.erase(isBelow0.begin() + l);
                isBelow1.erase(isBelow1.begin() + l);
                cp0.erase(cp0.begin() + l);
                cp1.erase(cp1.begin() + l);
            }
        }
        
        //switch children for the chosen nodes
        for(int j=0; j<cp0ordered.size(); j++){

            bool switchCp0(cp0ordered[j]->getDistanceToHead() + cp1ordered[j]->length() <= MaxBreedingLength), switchCp1(cp1ordered[j]->getDistanceToHead() + cp0ordered[j]->length() <= MaxBreedingLength); //whether or not cp0 will replace cp1
        
            if(switchCp0 && switchCp1){ //just switch the pointers
                switchChildrenPointers(cp0ordered[j], cp1ordered[j]);
            }
            else{
                //switch children;
                if(switchCp0){
                    cp0ordered[j]->switchChildren(cp1ordered[j]);
                    delete cp0ordered[j];//a copy of cp1 switched for cp0
                }//otherwise original 0 is just returned
                if(switchCp1){
                    cp1ordered[j]->switchChildren(cp0ordered[j]);
                    delete cp1ordered[j];
                } //otherwise the original 1 is just returned
            }
        }
    }
}

Individual Generation::bestOfGeneration(){
    
    Individual *best = &generation[0];
    for(int i=0; i<size; i++){
        if(generation[i].getFitness() >= best->getFitness()){
            best = &generation[i];
        }
    }
    return *best;
}

Individual* Generation::getIndividualPtr(int i){
    return &generation[i];
}

std::vector<double> Generation::getAllFitness(){
    std::vector<double> toreturn;
    for(int i=0; i<size; i++){
        toreturn.push_back(generation[i].getFitness());
    }
    return toreturn; 
}

double Generation::avgFitness(){
    double sum=0;
    for(int i=0; i<size; i++){
        sum+=generation[i].getFitness();
    }
    return sum/size;
}

double Generation::maxFitness(){
    double max=0;
    for(int i=0; i<size; i++){
        if(max<generation[i].getFitness()) max = generation[i].getFitness();
    }
    return max;
}
