//
//  Functions.hpp
//  Genetic Programming
//
//  Created by William Henderson on 07/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef Functions_hpp
#define Functions_hpp

#include <stdio.h>
#include "Node.hpp"
#include "IntFncNode.hpp"

enum spaceState {empty, player, opponent, offboard};

namespace Genetic_Programming_Functions {
    
    //for connect four:
    
    class Addition: public IntFncNode<int>::Function{
    public:
        Addition(): IntFncNode<int>::Function({std::type_index(typeid(int)), std::type_index(typeid(int))}){}
        int operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class Multiplication: public IntFncNode<int>::Function{
    public:
        Multiplication(): IntFncNode<int>::Function({std::type_index(typeid(int)), std::type_index(typeid(int))}){}
        int operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class Intif: public IntFncNode<int>::Function{
    public:
        Intif(): IntFncNode<int>::Function({std::type_index(typeid(bool)), std::type_index(typeid(int)), std::type_index(typeid(int))}){}
        int operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class Iflarger: public IntFncNode<bool>::Function{
    public:
        Iflarger(): IntFncNode<bool>::Function({std::type_index(typeid(int)), std::type_index(typeid(int))}){}
        bool operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class Ifequal: public IntFncNode<bool>::Function{
    public:
        Ifequal(): IntFncNode<bool>::Function({std::type_index(typeid(spaceState)), std::type_index(typeid(spaceState))}){}
        bool operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class Booland: public IntFncNode<bool>::Function{
    public:
        Booland(): IntFncNode<bool>::Function({std::type_index(typeid(bool)), std::type_index(typeid(bool))}){}
        bool operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class Boolor: public IntFncNode<bool>::Function{
    public:
        Boolor(): IntFncNode<bool>::Function({std::type_index(typeid(bool)), std::type_index(typeid(bool))}){}
        bool operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class Boolnot: public IntFncNode<bool>::Function{
    public:
        Boolnot(): IntFncNode<bool>::Function({std::type_index(typeid(bool))}){}
        bool operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class Getspacestate: public IntFncNode<spaceState>::Function{
    public:
        Getspacestate(): IntFncNode<spaceState>::Function({std::type_index(typeid(int)), std::type_index(typeid(int)), std::type_index(typeid(spaceState**))}){}
        spaceState operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    } ;
    
    //for symbolic regression
    
    class AdditionD: public IntFncNode<double>::Function{
    public:
        AdditionD(): IntFncNode<double>::Function({std::type_index(typeid(double)), std::type_index(typeid(double))}){}
        double operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class SubtractionD: public IntFncNode<double>::Function{
    public:
        SubtractionD(): IntFncNode<double>::Function({std::type_index(typeid(double)), std::type_index(typeid(double))}){}
        double operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class MultiplicationD: public IntFncNode<double>::Function{
    public:
        MultiplicationD(): IntFncNode<double>::Function({std::type_index(typeid(double)), std::type_index(typeid(double))}){}
        double operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
    
    class DivisionD: public IntFncNode<double>::Function{
    public:
        DivisionD(): IntFncNode<double>::Function({std::type_index(typeid(double)), std::type_index(typeid(double))}){}
        double operator()(std::vector<Node*> children);
        Node* simplify(Node* caller);
    };
}
    
#endif /* Functions_hpp */
