#pragma once
#include "NeatGenome.h"



//the wrapper that allows the AI to play the game ("Self Playing mode")
class NeatAgent {
public:
    NeatGenome* brain;
    bool isDead;
    double fitnessScore;
    
    //game state tracking
    double timeAlive;
    int kills;
    
    NeatAgent() {
        brain = nullptr;
        isDead = false;
        fitnessScore = 0.0;
        timeAlive = 0.0;
        kills = 0;
    }
    
    void setBrain(NeatGenome* genome) {
        brain = genome;
    }
    
    void generateDefaultBrain(int numInputs, int numOutputs) {
        if (brain) delete brain;
        brain = new NeatGenome();
        brain->addNode(0, NODE_TYPE_BIAS);
        for (int i = 1; i <= numInputs; i++) brain->addNode(i, NODE_TYPE_INPUT);
        for (int i = numInputs + 1; i <= numInputs + numOutputs; i++) brain->addNode(i, NODE_TYPE_OUTPUT);
        
        int innov = 1;
        for (int in = 0; in <= numInputs; in++) {
            for (int out = numInputs + 1; out <= numInputs + numOutputs; out++) {
                brain->addConnection(in, out, (rand() % 200 - 100) / 100.0, true, innov++);
            }
        }
    }
    
    //provide inputs from the game environment
    //(e.g. distance to enemy, player Y velocity, etc.)
    void thinkAndAct(double* inputs, int numInputs, bool& jump, bool& shoot, bool& moveLeft, bool& moveRight) {
        if (!brain || isDead) return;
        
        int numOutputs = 4;
        double outputs[4]; 
        
        //MATHEMATICS - All Equations at a Glance: Network eval
        //a_v = phi( sum(wj * a_uj) ) (Eq 4)
        brain->evaluate(inputs, numInputs, outputs, numOutputs);
        
        //map output activations to physical keyboard actions
        jump = (outputs[0] > 0.5);
        shoot = (outputs[1] > 0.5);
        moveLeft = (outputs[2] > 0.5);
        moveRight = (outputs[3] > 0.5);
    }
    
    void updateFitness() {
        //evaluate: compute f_i for all i in P(t) (Step 10, line 1)
        //this is highly dependent on the game (similar to Step 9 DPNV fitness)
        fitnessScore = timeAlive + (kills * 10.0); //example fitness calculation
        if (brain) {
            brain->fitness = fitnessScore;
        }
    }
};