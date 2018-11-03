//
//  main.cpp
//  Genetic Programming
//
//  Created by William Henderson on 05/03/2018.
//  Copyright © 2018 William Henderson. All rights reserved.
//

//An example program that finds the equation of a curve, though a set of points

#include <random>
#include "gp.hpp"
#include "Functions.hpp"

using namespace Genetic_Programming_Functions;

double *dependentVar;

int numberOfTestPoints;
double *y, *z; //pointers to where the variables the functions use are
double *xs, *ys, *zs; //arrays of independent and dependent variables

double test(Individual& toTest){
    //the test function, returns a value that is the sum of the distances from the curve to each data point
    
    double fitness = 0;
    
    for(int i=0; i<numberOfTestPoints; i++){
        *y = ys[i];
        *z = zs[i];
        double v;
        toTest.value(&v);
        fitness+= fabs(xs[i] - v);
    }
    
    fitness = 1/(1+fitness); // adjust fitness
    
    if(!isnormal(fitness)) fitness = 0;
    
    toTest.setFitness(fitness);
    
    return fitness;
    
}

int main(int argc, const char * argv[]) {
    
    numberOfTestPoints = 100;
    
    y = new double;
    z = new double;
    ys = new double[numberOfTestPoints];
    zs = new double[numberOfTestPoints];
    xs = new double[numberOfTestPoints];
    
    std::default_random_engine g = std::default_random_engine();
    std::normal_distribution<double> d = std::normal_distribution<double>(0, 0.01);
    
    for(int i=0; i<numberOfTestPoints; i++){
        double randomY = (RAND_MAX-rand())/static_cast<double>(RAND_MAX);
        double randomZ = (RAND_MAX-rand())/static_cast<double>(RAND_MAX);
        ys[i] = randomY;
        zs[i] = randomZ;
        double randomY3 = randomY*randomY*randomY;
        xs[i] = randomY - randomY3/randomZ + d(g); //curve to fit
    }
    
    const int numberOfGenerations = 40;
 
    //Functions;
    AdditionD* addition = new AdditionD;
    SubtractionD* subtraction = new SubtractionD;
    MultiplicationD* multiplication = new MultiplicationD;
    DivisionD* division = new DivisionD;
    
    Parameters<double> parameters{
        {addition, multiplication, subtraction, division}, //functions
        {y, z}, //variables
        {[](){return ((RAND_MAX - rand())/static_cast<double>(RAND_MAX))*10;}}, //generator for random constants
        {0.6, 0.4, 0, 0}, //probaility of functions, vars, and consts in th initial generation
        {} //parameters for adfs, which are empty
    };
    
    GPproblem<double> setup = GPproblem<double>(test, parameters, 6, std::type_index(typeid(double)), 4);
    
    Generation run = Generation(10000, 0.06, 10, &setup);
    
    run.normalisedFitness(false);
    
    for(int i=0; i<numberOfGenerations; i++){
        std::vector<int> order;
        order.reserve(5000);
        run.nextGeneration(order);
        run.normalisedFitness(true);
    }
    
    delete[] y; delete[] z; delete[] ys; delete zs; delete xs;
}
