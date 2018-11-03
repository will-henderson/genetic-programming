//
//  FuncNode.hpp
//  Genetic Programming
//
//  Created by William Henderson on 08/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef IntFncNode_hpp
#define IntFncNode_hpp

#include <stdio.h>
#include "Node.hpp"
#include <vector>

template <class T>
class IntFncNode: public Node{
    
public:
    
    class Function{
    public:
        Function(std::vector<std::type_index> argtypes_): argtypes(argtypes_){}
        virtual T operator()(std::vector<Node*>)=0;
        std::vector<std::type_index> argtypes;
        virtual Node* simplify(Node* caller)=0;
    };
    

private:
    
    Function *func;

public:
    
    IntFncNode(int thisLength, Setup* s, Function* function);
    
    Node* virtualConstructor();
    
    void value(void* toreturn) const;
    
    Node* simplify();
};


template <class T>
IntFncNode<T>::IntFncNode(int thisLength, Setup* s, Function* function): Node(thisLength, s, function->argtypes, std::type_index(typeid(T))){
    func = function;
}

template <class T>
Node* IntFncNode<T>::virtualConstructor(){
    return new IntFncNode(*this);
}

template <class T>
void IntFncNode<T>::value(void* toreturn) const{
    *(static_cast<T*>(toreturn)) = (*func)(children);
}

template<class T>
Node* IntFncNode<T>::simplify(){
    for(int i=0; i<children.size(); i++){
        Node* temp = children[i]->simplify();
        if(temp != children[i]){
            delete children[i];
            children[i] = temp;
        }
    }
    return func->simplify(this);
}

#endif /* IntFncNode_hpp */

