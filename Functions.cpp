//
//  Functions.cpp
//  Genetic Programming
//
//  Created by William Henderson on 07/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "Functions.hpp"
#include "ConstNode.hpp"
#include "Individual.hpp"
#include <math.h>
#include <vector>
#include <typeindex>

namespace Genetic_Programming_Functions{
    
    //Connect 4

    int Addition::operator()(std::vector<Node*> children){
        int a, b;
        children[0]->value(&a); children[1]->value(&b);
        return a+b;
        
    }
    int Multiplication::operator()(std::vector<Node*> children){
        int a, b;
        children[0]->value(&a); children[1]->value(&b);
        return a*b;
    }
    int Intif::operator()(std::vector<Node*> children){

        bool a;
        int b;
        children[0]->value(&a);
        
        if(a){
            children[1]->value(&b);
        }
        else{
            children[2]->value(&b);
        }
        return b;
    }
    
    bool Iflarger::operator()(std::vector<Node*> children){
        int a,b;
        children[0]->value(&a); children[1]->value(&b);
        return a > b;
    }
    
    bool Booland::operator()(std::vector<Node*> children){
        bool a;
        children[0]->value(&a);
        if(a){
            children[1]->value(&a);
            return a;
        }
        else{
            return false;
        }
    }
    bool Boolor::operator()(std::vector<Node*> children){
        bool a;
        children[0]->value(&a);
        if(a){
            return true;
        }
        else{
            children[1]->value(&a);
            return a;
        }
    }
    bool Boolnot::operator()(std::vector<Node*> children){
        bool a;
        children[0]->value(&a);
        return !a;
    }

    bool Ifequal::operator()(std::vector<Node*> children){
        spaceState a,b;
        children[0]->value(&a); children[1]->value(&b);
        return a==b;
    }
    
    spaceState Getspacestate::operator()(std::vector<Node*> children){
        int a,b;
        spaceState** c;
        children[0]->value(&a); children[1]->value(&b); children[2]->value(&c);
        
        if(a > 6 || a < 0 || b > 6 || b < 0){
            return offboard;
        }
        else{
            return c[a][b];
        }
    }
    
    Node* Addition::simplify(Node* caller){
        ConstNode<int>* a = dynamic_cast<ConstNode<int>* >(caller->getChild(0));
        ConstNode<int>* b = dynamic_cast<ConstNode<int>* >(caller->getChild(1));
        if(a != nullptr && b!= nullptr){
            return new ConstNode<int>(a->val + b->val, *caller);
        }
        return caller;
    }
    
    Node* Multiplication::simplify(Node* caller){
        ConstNode<int>* a = dynamic_cast<ConstNode<int>* >(caller->getChild(0));
        ConstNode<int>* b = dynamic_cast<ConstNode<int>* >(caller->getChild(1));
        if(a != nullptr && b!= nullptr){
            return new ConstNode<int>(a->val * b->val, *caller);
        }
        return caller;
    }
    
    Node* Intif::simplify(Node* caller){
        ConstNode<bool>* a = dynamic_cast<ConstNode<bool>* >(caller->getChild(0));
        if(a != nullptr){
            if(a->val){
                return caller->getChild(1)->virtualConstructor();
            }
            else return caller->getChild(2)->virtualConstructor();
        }
        return caller;
    }
    
    Node* Iflarger::simplify(Node* caller){
        ConstNode<int>* a = dynamic_cast<ConstNode<int>* >(caller->getChild(0));
        ConstNode<int>* b = dynamic_cast<ConstNode<int>* >(caller->getChild(1));
        if(a != nullptr && b != nullptr){
            return new ConstNode<bool>(a>b, *caller);
        }
        return caller;
    }
    
    Node* Ifequal::simplify(Node* caller){
        ConstNode<spaceState>* a = dynamic_cast<ConstNode<spaceState>* >(caller->getChild(0));
        ConstNode<spaceState>* b = dynamic_cast<ConstNode<spaceState>* >(caller->getChild(1));
        if(a != nullptr && b != nullptr){
            return new ConstNode<bool>(a==b, *caller);
        }
        return caller;
    }
    
    Node* Booland::simplify(Node* caller){
        ConstNode<bool>* a = dynamic_cast<ConstNode<bool>* >(caller->getChild(0));
        ConstNode<bool>* b = dynamic_cast<ConstNode<bool>* >(caller->getChild(1));
        if(a != nullptr && b != nullptr){
            return new ConstNode<bool>(a&&b, *caller);
        }
        if(a != nullptr){
            if(a->val){
                return caller->getChild(1)->virtualConstructor();
            }
            else return new ConstNode<bool>(false, *caller);
        }
        if(b != nullptr){
            if(b->val){
                return caller->getChild(0)->virtualConstructor();
            }
            else return new ConstNode<bool>(false, *caller);
        }
        return caller;
    }
    
    Node* Boolor::simplify(Node* caller){
        ConstNode<bool>* a = dynamic_cast<ConstNode<bool>* >(caller->getChild(0));
        ConstNode<bool>* b = dynamic_cast<ConstNode<bool>* >(caller->getChild(1));
        if(a != nullptr && b != nullptr){
            return new ConstNode<bool>(a||b, *caller);
        }
        if(a != nullptr){
            if(a->val){
                return new ConstNode<bool>(true, *caller);
            }
            else return caller->getChild(1)->virtualConstructor();
        }
        if(b != nullptr){
            if(b->val){
                return new ConstNode<bool>(true, *caller);
            }
            else return caller->getChild(0)->virtualConstructor();
        }
        return caller;
    }
    
    Node* Boolnot::simplify(Node* caller){
        ConstNode<bool>* a = dynamic_cast<ConstNode<bool>* >(caller->getChild(0));
        if(a != nullptr){
            return new ConstNode<bool>(!a, *caller);
        }
        //if child is also boolnot do something nice;
        return caller;
    }
    
    Node* Getspacestate::simplify(Node* caller){ //no way to simplify this
        return caller;
    }
    
    //Symbolic Regression
    
    double AdditionD::operator()(std::vector<Node*> children){
        double a, b;
        children[0]->value(&a); children[1]->value(&b);
        return a+b;
    }
    
    double SubtractionD::operator()(std::vector<Node*> children){
        double a, b;
        children[0]->value(&a); children[1]->value(&b);
        return a-b;
    }
    
    double MultiplicationD::operator()(std::vector<Node*> children){
        double a, b;
        children[0]->value(&a); children[1]->value(&b);
        return a*b;
    }
    
    double DivisionD::operator()(std::vector<Node*> children){
        double a, b;
        children[0]->value(&a); children[1]->value(&b);
        return a/b;
    }
    
    Node* AdditionD::simplify(Node* caller){
        ConstNode<double>* a = dynamic_cast<ConstNode<double>* >(caller->getChild(0));
        ConstNode<double>* b = dynamic_cast<ConstNode<double>* >(caller->getChild(1));
        if(a != nullptr && b!= nullptr){
            return new ConstNode<int>(a->val + b->val, *caller);
        }
        return caller;
    }
    Node* SubtractionD::simplify(Node* caller){
        ConstNode<double>* a = dynamic_cast<ConstNode<double>* >(caller->getChild(0));
        ConstNode<double>* b = dynamic_cast<ConstNode<double>* >(caller->getChild(1));
        if(a != nullptr && b!= nullptr){
            return new ConstNode<int>(a->val + b->val, *caller);
        }
        return caller;
    }
    
    Node* MultiplicationD::simplify(Node* caller){
        ConstNode<double>* a = dynamic_cast<ConstNode<double>* >(caller->getChild(0));
        ConstNode<double>* b = dynamic_cast<ConstNode<double>* >(caller->getChild(1));
        if(a != nullptr && b!= nullptr){
            return new ConstNode<int>(a->val * b->val, *caller);
        }
        return caller;
    }
    
    Node* DivisionD::simplify(Node* caller){
        ConstNode<double>* a = dynamic_cast<ConstNode<double>* >(caller->getChild(0));
        ConstNode<double>* b = dynamic_cast<ConstNode<double>* >(caller->getChild(1));
        if(a != nullptr && b!= nullptr){
            return new ConstNode<int>(a->val / b->val, *caller);
        }
        return caller;
    }
}
