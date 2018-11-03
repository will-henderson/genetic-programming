//
//  Node.cpp
//  Genetic Programming
//
//  Created by William Henderson on 05/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#include "Node.hpp"
#include <cstdlib>

Node::Node(unsigned short thisLength, Setup* s, std::vector<std::type_index> childrentypes, std::type_index r): returntype(r){
    
    unsigned short numberOfChildren = childrentypes.size();
    distanceToHead = thisLength;
    children.reserve(numberOfChildren);
    
    for(int i=0; i<numberOfChildren; i++){
        children.push_back(s->randomNode(thisLength+1, childrentypes[i])); //pointer so doesnt call copy constructor
        children[i]->parent = this;
    }
}

void Node::setParent(Node* newParent){
    parent = newParent;
}

Node* Node::getParent(){
    return parent;
}

Node* Node::getChild(unsigned short i){
    return children[i];
}

void Node::switchChildren(Node* newChild){
    
    for(int i=0; i<parent->children.size(); i++){
        if(parent->children[i] == this){
            parent->children[i] = newChild->virtualConstructor();
            parent->children[i]->parent = parent;
            remeasureDistanceToHead(newChild->distanceToHead - distanceToHead);
            break;
        }
    }
}

void Node::numberNodes(std::vector<unsigned long>& currentNumber, std::vector<std::type_index>& types){
    for(int i=0; true; i++){
        if(returntype == types[i]){
            number = (currentNumber[i])++;
            break;
        }
    }
    for(int i=0; i<children.size(); i++){
        children[i]->numberNodes(currentNumber, types);
    }
}

Node* Node::numberToNode(unsigned long chosenNumber, std::type_index chosenType){
    if(number==chosenNumber && returntype == chosenType){
        return this;
    }
    
    int lower = 0, upper = children.size();
    for(int i=children.size()-1; i>=0; i--){
        if(chosenType == children[i]->returntype){
            if(chosenNumber >= children[i]->number){
                lower = i;
                break;
            }
            upper = i;
        }
    }
    
    for(int i=upper-1; i>=lower; i--){
        Node* toreturn = children[i]->numberToNode(chosenNumber, chosenType);
        if(toreturn != 0){
            return toreturn;
        }
    }
    
    return 0;
    
}

int Node::length(){
    
    int max=0;
    for(int i=0; i<children.size(); i++){
        int t = children[i]->length();
        if(t>max) max=t;
    }
    return 1+max;
}

int Node::length(Node* end){ //traverses up the node from end to this
    
    int length = 0;
    
    Node* runner = end;
    while(runner != this){
        length++;
        runner = runner->parent;
    }
    return length;
}

void Node::remeasureDistanceToHead(short difference){
    distanceToHead += difference;
    
    for(int i=0; i<children.size(); i++){
        children[i]->remeasureDistanceToHead(difference);
    }
}

int Node::getDistanceToHead(){
    return distanceToHead;
}

bool Node::isBelow(Node* b, Node* head){
    if(distanceToHead < b->distanceToHead) return false;
    else return isBelowRecursion(b, head);
}

bool Node::isBelowRecursion(Node* b, Node* head){
    if(parent == b) return true;
    else if(parent == head) return false;
    else return parent->isBelow(b, head);

}


 const Node& Node::operator =(const Node& n){
     
     for(int i=0; i<children.size(); i++){
         delete children[i];
     }
     children.clear();
     
     returntype = n.returntype;
     number = n.number;
     distanceToHead = n.distanceToHead;
     
     int numberOfChildren = n.children.size();
     children.reserve(numberOfChildren);
     for(int i=0; i<numberOfChildren; i++){
        children.push_back(n.children[i]->virtualConstructor());
        children[i]->parent = this;
    }
    return *this;
}

Node::~Node(){
    for(int i=0; i<children.size(); i++){
        delete children[i];
        children[i]=NULL;
    }
    children.clear();
}

Node::Node(const Node& n): returntype(n.returntype), number(n.number), distanceToHead(n.distanceToHead){
    
    children.reserve(n.children.size());
    for(int i=0; i<n.children.size(); i++){
        children.push_back(n.children[i]->virtualConstructor());
        children[i]->parent = this;
    }
}

Node::Node(const Node& n, bool b): returntype(n.returntype), number(n.number), distanceToHead(n.distanceToHead), parent(n.parent){
    if(b){
        children.reserve(n.children.size());
        for(int i=0; i<n.children.size(); i++){
            children.push_back(n.children[i]->virtualConstructor());
            children[i]->parent = this;
        }
    }
}

void switchChildrenPointers(Node* a, Node* b){
    
    Node* temp = b->parent;
    
    for(int i=0; i<a->parent->children.size(); i++){
        if(a->parent->children[i] == a){
            a->parent->children[i] = b;
            b->parent = a->parent;
            break;
        }
    }
    for(int i=0; i<temp->children.size(); i++){
        if(temp->children[i] == b){
            temp->children[i] = a;
            a->parent = temp;
            break;
        }
    }
    int difference = b->distanceToHead - a->distanceToHead;
    a->remeasureDistanceToHead(difference);
    b->remeasureDistanceToHead(-difference);
}


