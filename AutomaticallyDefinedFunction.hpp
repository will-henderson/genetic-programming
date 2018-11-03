//
//  AutomaticallyDefinedFunction.hpp
//  Genetic Programming
//
//  Created by William Henderson on 17/08/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef AutomaticallyDefinedFunction_hpp
#define AutomaticallyDefinedFunction_hpp

#include <stdio.h>
#include "IntFncNode.hpp"
#include "ADFSetup.hpp"
#include "Generation.hpp"

template<class T>
class AutomaticallyDefinedFunction: public IntFncNode<T>::Function{
public:
    //normal function member functions
    AutomaticallyDefinedFunction(std::vector<std::type_index> argtypes, int numberOfIndividuals, double proportionToCopy, int MaxBreedingLength, ADFSetup* setup);
    ~AutomaticallyDefinedFunction();
    
    T operator()(std::vector<Node*> children);
    Node* simplify(Node* caller);
    
    Individual* selectedIndividual;
    Generation* generation;
    void nextGeneration(std::vector<int>& order);
    std::vector<void*> childrenVars;
    void setSelectedIndividual(int i); //if -1 sets as numbertocopy
};

template<class T>
AutomaticallyDefinedFunction<T>::AutomaticallyDefinedFunction(std::vector<std::type_index> argtypes, int numberOfIndividuals, double proportionToCopy, int MaxBreedingLength, ADFSetup* setup): IntFncNode<T>::Function(argtypes){
    childrenVars = setup->setChildrenVars();
    generation = new Generation(numberOfIndividuals, proportionToCopy, MaxBreedingLength, setup);
}

template<class T>
AutomaticallyDefinedFunction<T>::~AutomaticallyDefinedFunction(){
    delete generation;
}

template<class T>
T AutomaticallyDefinedFunction<T>::operator()(std::vector<Node*> children){
    
    for(int i=0; i<children.size(); i++){ //sets the values of childrenVals (the VarNodes val pointers) in ADF
        children[i]->value(childrenVars[i]);
    }
    T toreturn;
    selectedIndividual->value(&toreturn);
    return toreturn;
}

template<class T>
Node* AutomaticallyDefinedFunction<T>::simplify(Node* caller){
    return caller;
}

template<class T>
void AutomaticallyDefinedFunction<T>::nextGeneration(std::vector<int>& order){
    generation->nextGeneration(order);
}
template<class T>
void AutomaticallyDefinedFunction<T>::setSelectedIndividual(int i){
        selectedIndividual = generation->getIndividualPtr(i);
}


#endif /* AutomaticallyDefinedFunction_hpp */
