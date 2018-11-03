//
//  Node.hpp
//  Genetic Programming
//
//  Created by William Henderson on 05/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>
#include "Setup.hpp"
#include <typeindex>
#include <vector>

class Node{

protected:
    
    Node *parent;
    unsigned long number; //number dependent on type
    unsigned short distanceToHead;
    std::vector<Node*> children; //pointer to void rather than node since we don't know <int>/<double> etc of children
    std::type_index returntype;
    
public:
    
    Node(unsigned short thisLength, Setup* s, std::vector<std::type_index> childrentypes, std::type_index r);
    
    virtual Node* virtualConstructor() =0;
    
    void setParent(Node* newParent);
    Node* getParent();
    Node* getChild(unsigned short i);
    void switchChildren(Node* newChild); //the pointer from the parent to the calling object now points to newChild;
    virtual void value(void* toreturn) const =0;
    void numberNodes(std::vector<unsigned long>& currentNumber, std::vector<std::type_index>&);
    Node* numberToNode(unsigned long chosenNumber, std::type_index chosenType);
    int length(Node* end); //length between two nodes
    int length(); //maximum length of a branch from a node
    void remeasureDistanceToHead(short difference);
    int getDistanceToHead();
    bool isBelow(Node* b, Node* head);
    bool isBelowRecursion(Node* b, Node* head);
    
    virtual Node* simplify()=0; //a Node changes itself to a simplified version if possible;
    
    const Node& operator =(const Node&);
    
    virtual ~Node(); //destructor
    Node(const Node& n); //Copy Constructor
    Node(const Node& n, bool b); //Copy the variables of a node, but not its children if b = false, otherwise calls copy constructor
    
    friend void switchChildrenPointers(Node* a, Node* b);
    
};
#endif /* Node_hpp */
