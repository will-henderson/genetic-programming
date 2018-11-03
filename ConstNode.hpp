//
//  ConstNode.hpp
//  Genetic Programming
//
//  Created by William Henderson on 07/07/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef ConstNode_hpp
#define ConstNode_hpp

#include <stdio.h>
#include "Node.hpp"

template<class T>
class ConstNode : public Node{

public:
    T val; //the type of variable gives the position it is found in the array of vars entered when value is called;
    
    ConstNode(int thisLength, T v); //a constant - used when creating the initial generation
    ConstNode(T val_, const Node& n); //used for simplifying - val is the value, n is the node it is replacing;
    
    Node* virtualConstructor();
    
    void value(void* toreturn) const;
    
    Node* simplify();
};

template<class T>
ConstNode<T>::ConstNode(int thisLength, T v): Node(thisLength,nullptr,{},std::type_index(typeid(T))){
    val = v;
}

template<class T>
ConstNode<T>::ConstNode(T val_, const Node& n): Node(n, false){
    val = val_;
}

template<class T>
Node* ConstNode<T>::virtualConstructor(){
    return new ConstNode(*this);
}

template<class T>
void ConstNode<T>::value(void* toreturn) const{
    *(static_cast<T*>(toreturn)) = val;
}

template<class T>
Node* ConstNode<T>::simplify(){
    return this;
}

#endif /* ConstNode_hpp */
