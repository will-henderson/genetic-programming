//
//  gp.hpp
//  Genetic Programming
//
//  Created by William Henderson on 24/10/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

#ifndef gp_hpp
#define gp_hpp

#include <stdio.h>
#include <functional>
#include "Setup.hpp"
#include "StandardSetup.hpp"
#include "TournamentSelectionSetup.hpp"
#include "ADFSetup.hpp"
#include "IntFncNode.hpp"
#include "VarNode.hpp"
#include "ConstNode.hpp"
#include "AutomaticallyDefinedFunction.hpp"

typedef double (*testFunc)(Individual&);

struct ADFparameters{
    int numberToCreate;
    std::vector<std::type_index> childrenTypes;
    int maxInitialLength;
    double crossoverRate;
    int numberOfIndividuals;
    double proportionToCopy;
    int MaxBreedingLength;
};

template<class T>
struct Parameters{
    std::vector<typename IntFncNode<T>::Function*> f;
    std::vector<T*> v;
    std::vector<T(*)()> rc;
    std::vector<double> probabilityfvr;
    std::vector<ADFparameters> adfparameters;
};

template<class... Ts>
class GPproblem: public StandardSetup{
    
    testFunc testFunction;
    
    std::vector<void*> functions, vars, randomconstants, adfs, variablesForADFs;
    
    virtual Node* newIntFncNode(std::type_index type, int thisLength); //randomly chooses function
    virtual Node* newVarNode(std::type_index type, int thisLength); //randomly chooses var
    virtual Node* newConstNode(std::type_index type, int thisLength); //randomly chooses val
    virtual Node* newADFNode(std::type_index type, int thisLength);
    
    double test(Individual& toTest);
    
    template <class... Us>
    friend class ADFSetupHelper;
    
public:
    //
    GPproblem(testFunc t, Parameters<Ts>&... parameters, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_);
    ~GPproblem();
};

template<class... Ts>
class ADFSetupHelper: public ADFSetup{
    std::vector<std::type_index> childrenTypes;
    GPproblem<Ts...> *baseProblem;
    
    virtual Node* newIntFncNode(std::type_index type, int thisLength); //randomly chooses function
    virtual Node* newVarNode(std::type_index type, int thisLength); //randomly chooses var
    virtual Node* newConstNode(std::type_index type, int thisLength); //randomly chooses val
    virtual Node* newADFNode(std::type_index type, int thisLength);
    
public:
    ADFSetupHelper(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_, std::vector<std::type_index> childrenTypes_, GPproblem<Ts...> *baseProblem);
    std::vector<void*> setChildrenVars();
};

template<class A>
std::vector<std::type_index> setTypes(std::vector<std::type_index> types){
    types.push_back(std::type_index(typeid(A)));
    return types;
}

template<class A, class... B>
auto setTypes(std::vector<std::type_index> types) -> typename std::enable_if<sizeof...(B) != 0>::type{
    types.push_back(std::type_index(typeid(A)));
    return setTypes<B...>(types);
}

template<class A>
std::vector<std::vector<double> > probabilities(Parameters<A> pA, std::vector<std::vector<double> > toreturn){
    toreturn.push_back(pA.probabilityfvr);
    return toreturn;
}

template<class A, class... B>
auto probabilities(Parameters<A> pA, Parameters<B>... pB, std::vector<std::vector<double> > toreturn)  -> typename std::enable_if<sizeof...(B) != 0>::type{
    toreturn.push_back(pA.probabilityfvr);
    return probabilities(pB..., toreturn);
}

template<class A>
std::vector<std::vector<int> > numberOfThis(Parameters<A> pA, std::vector<std::vector<int> > toreturn){
    int n=0;
    for(int i=0; i<pA.adfparameters.size(); i++){
        n += pA.adfparameters[i].numberToCreate;
    }
    toreturn.push_back({static_cast<int>(pA.f.size()), static_cast<int>(pA.v.size()), static_cast<int>(pA.rc.size()), n});
    return toreturn;
}

template<class A, class... B>
auto numberOfThis(Parameters<A> pA, Parameters<B>... pB, std::vector<std::vector<int> > toreturn) -> typename std::enable_if<sizeof...(B) != 0>::type{
    return numberOfThis<B...>(pB..., numberOfThis<A>(pA, toreturn));
    }

template<class A>
void unpackParameters(Parameters<A> pA, std::vector<void*>::iterator f, std::vector<void*>::iterator v, std::vector<void*>::iterator rc){
    
    *f = new std::vector<typename IntFncNode<A>::Function*>(pA.f);
    *v = new std::vector<A*>(pA.v);
    *rc = new std::vector<A(*)()>(pA.rc);
}

template<class A, class... B>
auto unpackParameters(Parameters<A> pA, Parameters<B>... pB, std::vector<void*>::iterator f, std::vector<void*>::iterator v, std::vector<void*>::iterator rc) -> typename std::enable_if<sizeof...(B) != 0>::type{
    unpackParameters<A>(pA, f, rc, v);
    unpackParameters<B...>(pB..., f++, rc++, v++);
}

typedef ADFSetup* (*ADFSetupCreator)(std::type_index, std::vector<std::type_index>, int, double, void*);
template<class A>
void createADFs(Parameters<A> pA, std::vector<void*>::iterator adfs, std::vector<void*>::iterator vadfs, const ADFSetupCreator setup, void* bp){
    for(int i=0; i<pA.adfparameters.size(); i++){
        ADFparameters p = pA.adfparameters[i];
        *adfs = new std::vector<AutomaticallyDefinedFunction<A> >;
        *vadfs = new std::vector<A*>;
        static_cast<std::vector<AutomaticallyDefinedFunction<A> >*>(*adfs)->push_back(AutomaticallyDefinedFunction<A>(p.childrenTypes, p.numberOfIndividuals,  p.proportionToCopy, p.MaxBreedingLength, setup(std::type_index(typeid(A)), p.childrenTypes, p.maxInitialLength, p.crossoverRate, bp)));
    }
}

template<class A, class... B>
void createADFs(Parameters<A> pA, Parameters<B>... pB, std::vector<void*>::iterator adfs, std::vector<void*>::iterator vadfs, const ADFSetupCreator setup){
    createADFs<A>(pA, adfs, vadfs, setup);
    createADFs<B...>(pB..., adfs++, vadfs++, setup);
}

template<class... Ts>
ADFSetup* adfSetupCreator(std::type_index returnType_, std::vector<std::type_index> childrenTypes_, int maxInitialLength_, double crossoverRate_, void* baseproblem){
    GPproblem<Ts...> *bp = static_cast<GPproblem<Ts...>*>(baseproblem);
    return new ADFSetupHelper<Ts...>(bp->types, bp->probabilityfvr, bp->numberofthis, maxInitialLength_, returnType_, crossoverRate_, childrenTypes_, bp);
}

template<class... Ts>
GPproblem<Ts...>::GPproblem(testFunc t, Parameters<Ts>&... parameters, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_): StandardSetup(setTypes<Ts...>({}), probabilities<Ts...>(parameters..., {}), numberOfThis<Ts...>(parameters..., {}), maxInitialLength_, returntypeofindividuals_, crossoverRate_), testFunction(t), functions(sizeof...(Ts)), vars(sizeof...(Ts)), randomconstants(sizeof...(Ts)), adfs(sizeof...(Ts)), variablesForADFs(sizeof...(Ts)){
    
    //unpack parameters
    
    unpackParameters<Ts...>(parameters..., functions.begin(), vars.begin(), randomconstants.begin());
    
    //create ADFS
    createADFs<Ts...>(parameters..., adfs.begin(), variablesForADFs.begin(), adfSetupCreator<Ts...>, this);
};

template<class... Ts>
double GPproblem<Ts...>::test(Individual& toTest){
    return testFunction(toTest);
}

template<class A>
Node* ifn(int i, int thisLength, Setup* s, void* functions){
    std::vector<typename IntFncNode<A>::Function*> *f = static_cast<typename std::vector<typename IntFncNode<A>::Function*>*>(functions);
    return new IntFncNode<A>(thisLength, s, (*f)[rand()%f->size()]);
}
    

template<class A, class... B>
auto ifn(int i, int thisLength, Setup* s, void* functions) -> typename std::enable_if<sizeof...(B) != 0>::type{
    if(i){
        return ifn<B...>(i, thisLength, s, functions);
    }
    else{
        std::vector<typename IntFncNode<A>::Function*> f = *(static_cast<typename std::vector<typename IntFncNode<A>::Function*>*>(functions));
        return new IntFncNode<A>(thisLength, s, f[rand()%f.size()]);
    }
}

template<class A>
Node* vn(int i, int thisLength, Setup* s, void* vars){
    std::vector<A*> v = *(static_cast<typename std::vector<A*>*>(vars));
    return new VarNode<A>(thisLength, v[rand()%v.size()]);
}

template<class A, class... B>
auto vn(int i, int thisLength, Setup* s, void* vars) -> typename std::enable_if<sizeof...(B) != 0>::type{
    if(i){
        return vn<B...>(i-1, thisLength, s, vars);
    }
    else{
        std::vector<A*> v = *(static_cast<typename std::vector<A*>*>(vars));
        return new VarNode<A>(thisLength, v[rand()%v.size()]);
    }
}

template<class A>
Node*  cn(int i, int thisLength, Setup* s, void* randomconstants){
    std::vector<A(*)()> c = *(static_cast<typename std::vector<A(*)()>*>(randomconstants));
    return new ConstNode<A>(thisLength, c[rand()%c.size()]());
}

template<class A, class... B>
auto cn(int i, int thisLength, Setup* s, void* randomconstants) -> typename std::enable_if<sizeof...(B) != 0>::type{
    if(i){
        return cn<B...>(i-1, thisLength, s, randomconstants);
    }
    else{
        std::vector<A(*)()> c = *(static_cast<typename std::vector<A(*)()>*>(randomconstants));
        return new ConstNode<A>(thisLength, s, c[rand()%c.size()]());
    }
}

template<class... Ts>
Node* GPproblem<Ts...>::newIntFncNode(std::type_index type, int thisLength){
    int i;
    for(i=0; types[i] != type; i++);
    return ifn<Ts...>(i, thisLength, this, functions[i]);
}

template<class... Ts>
Node* GPproblem<Ts...>::newVarNode(std::type_index type, int thisLength){
    int i;
    for(i=0; types[i] != type; i++);
    return vn<Ts...>(i, thisLength, this, vars[i]);
}

template<class... Ts>
Node* GPproblem<Ts...>::newConstNode(std::type_index type, int thisLength){
    int i;
    for(i=0; types[i] != type; i++);
    return cn<Ts...>(i, thisLength, this, randomconstants[i]);
}

template<class... Ts>
Node* GPproblem<Ts...>::newADFNode(std::type_index type, int thisLength){
    int i;
    for(i=0; types[i] != type; i++);
    return ifn<Ts...>(i, thisLength, this, adfs[i]);
}

template<class A>
void destroy(std::vector<void*>::iterator f, std::vector<void*>::iterator v, std::vector<void*>::iterator rc, std::vector<void*>::iterator adfs, std::vector<void*>::iterator vadfs){
    delete static_cast<std::vector<typename IntFncNode<A>::Function*>*>(*f);
    delete static_cast<std::vector<A*>*>(*v);
    delete static_cast<std::vector<A(*)()>*>(*rc);
    delete static_cast<std::vector<AutomaticallyDefinedFunction<A> >*>(*adfs);
    
    std::vector<A*>* vadfsp = static_cast<std::vector<A*>*>(*vadfs);
    if(*vadfs!=NULL){
        for(int i=0; i<vadfsp->size(); i++){
            delete (*vadfsp)[i];
        }
    }
    delete vadfsp;
}

template<class A, class... B>
auto destroy(std::vector<void*>::iterator f, std::vector<void*>::iterator v, std::vector<void*>::iterator rc, std::vector<void*>::iterator adfs, std::vector<void*>::iterator vadfs) -> typename std::enable_if<sizeof...(B) != 0>::type{
    destroy<A>(f, v, rc, adfs, vadfs);
    destroy<B...>(f++, v++, rc++, adfs++, vadfs++);
}

template<class... Ts>
GPproblem<Ts...>::~GPproblem(){
    destroy<Ts...>(functions.begin(), vars.begin(), randomconstants.begin(), adfs.begin(), variablesForADFs.begin());
}

template<class... Ts>
ADFSetupHelper<Ts...>::ADFSetupHelper(std::vector<std::type_index> types_, std::vector<std::vector<double> > probabilityfvr_, std::vector<std::vector<int> > numberofthis_, int maxInitialLength_, std::type_index returntypeofindividuals_, double crossoverRate_, std::vector<std::type_index> childrenTypes_, GPproblem<Ts...>* baseProblem_): ADFSetup(types_, probabilityfvr_, numberofthis_, maxInitialLength_, returntypeofindividuals_, crossoverRate_), childrenTypes(childrenTypes_), baseProblem(baseProblem_){}

template<class A>
void createType(std::vector<std::type_index>& childrenTypes, std::vector<void*> toreturn, std::vector<void*>::iterator vars){
    std::type_index type = std::type_index(typeid(A));
    for(int i=0; i<childrenTypes.size(); i++){
        if(type == childrenTypes[i]){
            A *a = new A;
            toreturn.push_back(a);
            static_cast<std::vector<A*>*>(*vars)->push_back(a);
        }
    }
}

template<class A, class... B>
auto createType(std::vector<std::type_index>& childrenTypes, std::vector<void*> toreturn, std::vector<void*>::iterator vars) -> typename std::enable_if<sizeof...(B) != 0>::type{
    createType<A>(childrenTypes, toreturn, vars);
    createType<B...>(childrenTypes, toreturn, vars++);
}

template<class... Ts>
std::vector<void*> ADFSetupHelper<Ts...>::setChildrenVars(){
    childrenVars.clear();
    createType<Ts...>(childrenTypes, childrenVars, baseProblem->variablesForADFs.begin());
    return childrenVars;
}

template<class... Ts>
Node* ADFSetupHelper<Ts...>::newIntFncNode(std::type_index type, int thisLength){
    return baseProblem->newIntFncNode(type, thisLength);
}

template<class... Ts>
Node* ADFSetupHelper<Ts...>::newVarNode(std::type_index type, int thisLength){
    return baseProblem->newVarNode(type, thisLength);
}

template<class... Ts>
Node* ADFSetupHelper<Ts...>::newConstNode(std::type_index type, int thisLength){
    return baseProblem->newConstNode(type, thisLength);
}

template<class... Ts>
Node* ADFSetupHelper<Ts...>::newADFNode(std::type_index type, int thisLength){
    return baseProblem->newADFNode(type, thisLength);
}


#endif /* gp_hpp */
