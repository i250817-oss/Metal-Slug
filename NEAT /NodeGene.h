#pragma once




const int NODE_TYPE_INPUT = 0;
const int NODE_TYPE_HIDDEN = 1;
const int NODE_TYPE_OUTPUT = 2;
const int NODE_TYPE_BIAS = 3;

//MATHEMATICS - Formal Genome Definition
//node genes: nk = (id_k, type_k)
class NodeGene {
public:
    int id;
    int type;
};