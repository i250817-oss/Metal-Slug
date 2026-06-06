#pragma once
#include "NeatGenome.h"

//CONSTRAINTS & INVARIANTS - Parameter Standard (Step 11 Summary)
const double PROB_WEIGHT_MUTATE = 0.80;
const double PROB_PERTURB = 0.90;
const double PROB_REPLACE = 0.10;
const double PROB_ADD_CONNECTION = 0.05;
const double PROB_ADD_NODE = 0.03;
const double PROB_CROSSOVER = 0.75;
const double PROB_INTERSPECIES_MATE = 0.001;
const double PROB_DISABLE_STAYS_OFF = 0.75;
const double COMPATIBILITY_THRESHOLD = 3.0; //delta_t
const double C1_EXCESS = 1.0;
const double C2_DISJOINT = 1.0;
const double C3_WEIGHT_DIFF = 0.4;
const int STAGNATION_LIMIT = 15;

#include "../Level and Environment System/perlin.h"

using namespace std;



//step 5: Speciation
class NeatSpecies {
public:
    int speciesId;
    NeatGenome* representative; //c11: Chosen randomly from previous generation
    
    NeatGenome** members; //no vector allowed
    int numMembers;
    int maxMembers;

    double maxFitnessEver;
    int generationsSinceImprovement;
    double totalAdjustedFitness;
    int expectedOffspring;

    NeatSpecies() {
        speciesId = -1;
        representative = nullptr;
        numMembers = 0;
        maxMembers = 1000; //large array to avoid vector
        members = new NeatGenome*[maxMembers];
        maxFitnessEver = 0.0;
        generationsSinceImprovement = 0;
        totalAdjustedFitness = 0.0;
        expectedOffspring = 0;
    }
    
    ~NeatSpecies() {
        delete[] members;
        //representative is just a pointer to a genome in the population,
        //which is owned and deleted by NeatPopulation.
    }
    
    void addMember(NeatGenome* genome) {
        if (numMembers < maxMembers) {
            members[numMembers++] = genome;
        }
    }
    
    void resetMembers() {
        numMembers = 0;
        totalAdjustedFitness = 0.0;
        expectedOffspring = 0;
    }
};

class NeatPopulation {
public:
    NeatGenome** population;
    int popSize;
    
    NeatSpecies** species;
    int numSpecies;
    int maxSpecies;
    
    NeatGenome* globalBest; //"keep a record of the global best player"
    int currentGeneration;
    int nextInnovationNumber;
    int nextNodeId;
    
    NeatPopulation(int size) {
        popSize = size;
        population = new NeatGenome*[popSize];
        
        maxSpecies = size;
        species = new NeatSpecies*[maxSpecies];
        numSpecies = 0;
        
        globalBest = nullptr;
        currentGeneration = 0;
        nextInnovationNumber = 1; //c1: Innovation numbers are positive
        nextNodeId = 1;
        
        //step 7: Minimal Initialization
        //start small, earn every neuron (Zero hidden nodes)
        for (int i = 0; i < popSize; i++) {
            population[i] = new NeatGenome();
            population[i]->genomeId = i;
            
            //nodes: 0=Bias, 1-5=Inputs, 6-10=Outputs
            population[i]->addNode(0, NODE_TYPE_BIAS);
            for (int n = 1; n <= 5; n++) population[i]->addNode(n, NODE_TYPE_INPUT);
            for (int n = 6; n <= 10; n++) population[i]->addNode(n, NODE_TYPE_OUTPUT);

            //connect inputs to outputs with random weights
            int innov = 1;
            for (int in = 0; in <= 5; in++) {
                for (int out = 6; out <= 10; out++) {
                    //use perlin with non-integer offsets to avoid 0 results
                    float px = i * 0.13f + 1.7f;
                    float py = innov * 0.17f + 2.3f;
                    double w = perlin(px, py) * 8.0; 
                    population[i]->addConnection(in, out, w, true, innov++);
                }
            }
            nextInnovationNumber = innov;
            nextNodeId = 11;
        }
    }
    
    ~NeatPopulation() {
        for (int i = 0; i < popSize; i++) delete population[i];
        delete[] population;
        for (int i = 0; i < numSpecies; i++) delete species[i];
        delete[] species;
        if (globalBest) delete globalBest;
    }
    
    double calculateCompatibility(NeatGenome* g1, NeatGenome* g2) {
        return g1->calculateCompatibility(*g2);
    }
    
    void speciate() {
        for (int i = 0; i < numSpecies; i++) species[i]->resetMembers();
        
        for (int i = 0; i < popSize; i++) {
            bool found = false;
            for (int s = 0; s < numSpecies; s++) {
                if (calculateCompatibility(population[i], species[s]->representative) < 3.0) {
                    species[s]->addMember(population[i]);
                    found = true;
                    break;
                }
            }
            if (!found && numSpecies < maxSpecies) {
                species[numSpecies] = new NeatSpecies();
                species[numSpecies]->speciesId = numSpecies;
                species[numSpecies]->representative = new NeatGenome(*(population[i]));
                species[numSpecies]->addMember(population[i]);
                numSpecies++;
            }
        }
    }

    void evolveGeneration() {
        speciate();
        
        //find total adjusted fitness to assign offspring
        double totalAdjustedFitness = 0.0;
        for (int i = 0; i < popSize; i++) {
            totalAdjustedFitness += population[i]->adjustedFitness;
        }

        NeatGenome** nextGeneration = new NeatGenome*[popSize];
        int nextGenCount = 0;

        for (int i = 0; i < numSpecies; i++) {
            if (species[i]->numMembers == 0) continue;
            
            //calculate expected offspring for this species
            double speciesFitness = 0.0;
            NeatGenome* champion = species[i]->members[0];
            for (int j = 0; j < species[i]->numMembers; j++) {
                speciesFitness += species[i]->members[j]->adjustedFitness;
                if (species[i]->members[j]->fitness > champion->fitness) {
                    champion = species[i]->members[j];
                }
            }
            updateGlobalBest(champion);

            int expected = ((speciesFitness / totalAdjustedFitness) * popSize);
            
            //champion always survives if species > 5
            if (species[i]->numMembers > 5 && nextGenCount < popSize) {
                nextGeneration[nextGenCount++] = new NeatGenome(*champion);
                expected--;
            }

            for (int e = 0; e < expected && nextGenCount < popSize; e++) {
                //pseudo-randomly pick two parents from the species
                int p1Idx = (currentGeneration * 13 + e * 7) % species[i]->numMembers;
                int p2Idx = (currentGeneration * 17 + e * 11) % species[i]->numMembers;
                NeatGenome* p1 = species[i]->members[p1Idx];
                NeatGenome* p2 = species[i]->members[p2Idx];

                //ensure p1 is the more fit parent
                if (p2->fitness > p1->fitness) {
                    NeatGenome* temp = p1;
                    p1 = p2;
                    p2 = temp;
                }

                //crossover
                NeatGenome* child = p1->crossover(*p2);

                //mutate
                child->mutateWeights(currentGeneration);
                
                //structural mutations
                if (((currentGeneration * 19 + e * 23) % 100) < 30) {
                    child->mutateAddConnection(nextInnovationNumber);
                }
                if (((currentGeneration * 29 + e * 31) % 100) < 3) {
                    child->mutateAddNode(nextInnovationNumber, nextNodeId);
                }

                nextGeneration[nextGenCount++] = child;
            }
        }

        //fill remaining slots
        while (nextGenCount < popSize) {
            nextGeneration[nextGenCount++] = new NeatGenome(globalBest ? *globalBest : *population[0]);
        }

        //replace old population
        for (int i = 0; i < popSize; i++) {
            delete population[i];
            population[i] = nextGeneration[i];
        }
        delete[] nextGeneration;

        currentGeneration++;
    }
    
    //"keep a record of the global best player... keyboard key 'B' to activate"
    void updateGlobalBest(NeatGenome* currentChampion) {
        if (!globalBest || currentChampion->fitness > globalBest->fitness) {
            if (globalBest) delete globalBest;
            globalBest = new NeatGenome(*currentChampion); //uses deep copy
        }
    }
};