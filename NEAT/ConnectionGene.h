#pragma once



//MATHEMATICS - Formal Genome Definition
//connection genes: cj = (uj, vj, wj, ej, ij)
class ConnectionGene {
public:
    int inNode;
    int outNode;
    double weight;
    bool isEnabled;
    int innovationNum;
};