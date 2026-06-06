#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include "../headers/string.h"

template <typename T>
inline void copyBuffer(T* dest, const T* src, int count) {
    if (!dest || !src) return;
    for (int i = 0; i < count; i++) {
        dest[i] = src[i];
    }
}

//MATHEMATICS - All Equations at a Glance
//activation: phi(x) = 1 / (1 + e^-4.9x)
inline double neatActivation(double x) {
    return 1.0 / (1.0 + exp(-4.9 * x));
}

#include "NodeGene.h"
#include "ConnectionGene.h"

using namespace std;



//eq (3) G = (N, C)
class NeatGenome {
public:
    int genomeId;
    
    //using standard arrays to avoid STL Vector
    NodeGene* nodes;
    int numNodes;
    int maxNodes;

    ConnectionGene* connections;
    int numConnections;
    int maxConnections;

    double fitness;
    double adjustedFitness; //f'_i = f_i / |Sk| (Eq 8)
    int speciesId;

    NeatGenome() {
        genomeId = 0;
        numNodes = 0;
        maxNodes = 10;
        nodes = new NodeGene[maxNodes];

        numConnections = 0;
        maxConnections = 10;
        connections = new ConnectionGene[maxConnections];
        
        fitness = 0.0;
        adjustedFitness = 0.0;
        speciesId = -1;
    }

    ~NeatGenome() {
        if (nodes) delete[] nodes;
        if (connections) delete[] connections;
    }

    //deep Copy constructor
    NeatGenome(const NeatGenome& other) {
        genomeId = other.genomeId;
        fitness = other.fitness;
        adjustedFitness = other.adjustedFitness;
        speciesId = other.speciesId;

        numNodes = other.numNodes;
        maxNodes = other.maxNodes;
        nodes = new NodeGene[maxNodes];
        copyBuffer(nodes, other.nodes, numNodes);

        numConnections = other.numConnections;
        maxConnections = other.maxConnections;
        connections = new ConnectionGene[maxConnections];
        copyBuffer(connections, other.connections, numConnections);
    }

    NeatGenome& operator=(const NeatGenome& other) {
        if (this == &other) return *this;
        if (nodes) delete[] nodes;
        if (connections) delete[] connections;

        genomeId = other.genomeId;
        fitness = other.fitness;
        adjustedFitness = other.adjustedFitness;
        speciesId = other.speciesId;

        numNodes = other.numNodes;
        maxNodes = other.maxNodes;
        nodes = new NodeGene[maxNodes];
        copyBuffer(nodes, other.nodes, numNodes);

        numConnections = other.numConnections;
        maxConnections = other.maxConnections;
        connections = new ConnectionGene[maxConnections];
        copyBuffer(connections, other.connections, numConnections);
        return *this;
    }

    void addNode(int id, int type) {
        if (numNodes >= maxNodes) {
            maxNodes *= 2;
            NodeGene* newNodes = new NodeGene[maxNodes];
            copyBuffer(newNodes, nodes, numNodes);
            delete[] nodes;
            nodes = newNodes;
        }
        nodes[numNodes].id = id;
        nodes[numNodes].type = type;
        numNodes++;
    }

    void addConnection(int inNode, int outNode, double weight, bool isEnabled, int innovationNum) {
        //c5: Structural mutations always append to the genome.
        if (numConnections >= maxConnections) {
            maxConnections *= 2;
            ConnectionGene* newConns = new ConnectionGene[maxConnections];
            copyBuffer(newConns, connections, numConnections);
            delete[] connections;
            connections = newConns;
        }
        connections[numConnections].inNode = inNode;
        connections[numConnections].outNode = outNode;
        connections[numConnections].weight = weight;
        connections[numConnections].isEnabled = isEnabled;
        connections[numConnections].innovationNum = innovationNum;
        numConnections++;
    }

    //MATHEMATICS - All Equations at a Glance: Network eval
    //a_v = phi( sum(wj * a_uj) ) (Eq 4)
    void evaluate(double* inputs, int numInputs, double* outputs, int& numOutputs) {
        int maxNodeId = 0;
        for (int i = 0; i < numNodes; i++) {
            if (nodes[i].id > maxNodeId) maxNodeId = nodes[i].id;
        }
        
        //manual array instead of unordered_map
        double* activations = new double[maxNodeId + 1];
        for(int i=0; i<=maxNodeId; i++) activations[i] = 0.0;

        int inputIdx = 0;
        for (int i = 0; i < numNodes; i++) {
            if (nodes[i].type == NODE_TYPE_INPUT && inputIdx < numInputs) {
                activations[nodes[i].id] = inputs[inputIdx++];
            }
            if (nodes[i].type == NODE_TYPE_BIAS) {
                activations[nodes[i].id] = 1.0;
            }
        }

        //5 passes to propagate signals through deep or recurrent networks
        for (int pass = 0; pass < 5; pass++) {
            for (int i = 0; i < numConnections; i++) {
                if (!connections[i].isEnabled) continue; //c3: Disabled genes remain but are skipped
                int u = connections[i].inNode;
                int v = connections[i].outNode;
                double w = connections[i].weight;
                activations[v] += w * activations[u]; //sum wj * a_uj
            }
        }

        numOutputs = 0;
        for (int i = 0; i < numNodes; i++) {
            if (nodes[i].type == NODE_TYPE_HIDDEN || nodes[i].type == NODE_TYPE_OUTPUT) {
                //a_v = phi(...)
                activations[nodes[i].id] = neatActivation(activations[nodes[i].id]);
                if (nodes[i].type == NODE_TYPE_OUTPUT) {
                    outputs[numOutputs++] = activations[nodes[i].id];
                }
            }
        }

        delete[] activations;
    }

    double calculateCompatibility(const NeatGenome& other) {
        double excess = 0, disjoint = 0, weightDiff = 0, matching = 0;
        
        int i = 0, j = 0;
        while (i < numConnections && j < other.numConnections) {
            int innov1 = connections[i].innovationNum;
            int innov2 = other.connections[j].innovationNum;
            
            if (innov1 == innov2) {
                weightDiff += abs(connections[i].weight - other.connections[j].weight);
                matching++; i++; j++;
            } else if (innov1 < innov2) {
                disjoint++; i++;
            } else {
                disjoint++; j++;
            }
        }
        excess = (numConnections - i) + (other.numConnections - j);
        
        double N = max(numConnections, other.numConnections);
        if (N < 1) N = 1;
        
        const double C1 = 1.0, C2 = 1.0, C3 = 0.4;
        return (C1 * excess / N) + (C2 * disjoint / N) + (C3 * (matching > 0 ? weightDiff / matching : 0));
    }

    NeatGenome* crossover(const NeatGenome& other) {
        NeatGenome* child = new NeatGenome();
        
        //assume 'this' is the more fit parent. 
        //we inherit all nodes from 'this'.
        for (int i = 0; i < numNodes; i++) {
            child->addNode(nodes[i].id, nodes[i].type);
        }

        int i = 0, j = 0;
        while (i < numConnections && j < other.numConnections) {
            int innov1 = connections[i].innovationNum;
            int innov2 = other.connections[j].innovationNum;
            
            if (innov1 == innov2) {
                //matching gene: inherit randomly
                //since we can't use rand(), we use a pseudo-random hash of innovNum
                bool fromFirst = ((innov1 * 17) % 2) == 0;
                ConnectionGene cg = fromFirst ? connections[i] : other.connections[j];
                //75% chance to disable if either parent is disabled
                if (!connections[i].isEnabled || !other.connections[j].isEnabled) {
                    if (((innov1 * 23) % 4) != 0) cg.isEnabled = false; 
                }
                child->addConnection(cg.inNode, cg.outNode, cg.weight, cg.isEnabled, cg.innovationNum);
                i++; j++;
            } else if (innov1 < innov2) {
                //disjoint gene in more fit parent: inherit
                child->addConnection(connections[i].inNode, connections[i].outNode, connections[i].weight, connections[i].isEnabled, connections[i].innovationNum);
                i++;
            } else {
                //disjoint gene in less fit parent: skip
                j++;
            }
        }
        //excess genes from more fit parent
        while (i < numConnections) {
            child->addConnection(connections[i].inNode, connections[i].outNode, connections[i].weight, connections[i].isEnabled, connections[i].innovationNum);
            i++;
        }
        return child;
    }

    void mutateAddConnection(int& globalInnovation) {
        int inIdx = (((long)this * 13 + globalInnovation * 7) % numNodes);
        int outIdx = (((long)this * 17 + globalInnovation * 11) % numNodes);
        
        if (nodes[outIdx].type == NODE_TYPE_INPUT || nodes[outIdx].type == NODE_TYPE_BIAS) return;

        for (int i = 0; i < numConnections; i++) {
            if (connections[i].inNode == nodes[inIdx].id && connections[i].outNode == nodes[outIdx].id) return;
        }

        double w = ((globalInnovation % 100) / 50.0) - 1.0; 
        addConnection(nodes[inIdx].id, nodes[outIdx].id, w, true, globalInnovation++);
    }

    void mutateAddNode(int& globalInnovation, int& globalNodeId) {
        if (numConnections == 0) return;
        int connIdx = (((long)this * 19 + globalInnovation * 23) % numConnections);
        if (!connections[connIdx].isEnabled) return;

        connections[connIdx].isEnabled = false;
        
        int inNode = connections[connIdx].inNode;
        int outNode = connections[connIdx].outNode;
        double oldWeight = connections[connIdx].weight;

        int newNodeId = globalNodeId++;
        addNode(newNodeId, NODE_TYPE_HIDDEN);

        addConnection(inNode, newNodeId, 1.0, true, globalInnovation++);
        addConnection(newNodeId, outNode, oldWeight, true, globalInnovation++);
    }

    void mutateWeights(int generation) {
        for (int i = 0; i < numConnections; i++) {
            if (((long)this + i + generation) % 100 < 80) {
                if (((long)this + i * 3 + generation) % 100 < 90) {
                    connections[i].weight += (((long)this + i * 7 + generation) % 100 / 50.0) - 1.0;
                } else {
                    connections[i].weight = (((long)this + i * 11 + generation) % 100 / 50.0) - 1.0;
                }
            }
        }
    }
};