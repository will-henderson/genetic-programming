//
//  VarNode.hpp
//  Genetic Programming
//
//  Created by William Henderson on 07/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef VarNode_hpp
#define VarNode_hpp

#include <stdio.h>
#include "Node.hpp"

template<class T>
class VarNode : public Node{
 
public:
    T* var;
    
    VarNode(int thisLength, T* var); //a pointer to a var
    
    Node* virtualConstructor();
    
    void value(void* toreturn) const;
    
    Node* simplify();
};

template<class T>
VarNode<T>::VarNode(int thisLength, T* v): Node(thisLength,nullptr,{},std::type_index(typeid(T))){
    var = v;
}

template<class T>
Node* VarNode<T>::virtualConstructor(){
    return new VarNode(*this);
}

template<class T>
void VarNode<T>::value(void* toreturn) const{
    *(static_cast<T*>(toreturn)) = *var;
}

template<class T>
Node* VarNode<T>::simplify(){
    return this;
}
#endif /* VarNode_hpp */
