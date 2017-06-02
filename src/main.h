#ifndef MAIN_H_
#define MAIN_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <queue>
#include <map>
#include <list>
#include <string>
#include <cstring>

#include <graphviz/cgraph.h>

#define DEBUG FALSE

namespace std {

list<Agnode_t*> reversePosOrder(Agraph_t* g);

void reversePosOrder(Agraph_t* g, Agnode_t* node, list<Agnode_t*>& rpo);

list<Agnode_t*> searchT (Agraph_t* g, Agnode_t* node);

void addNodesToGraph(Agraph_t* g, list<Agnode_t*> nodes);

int* state(Agnode_t*);

int* stateCreate(Agnode_t*);

void outAsDot(Agraph_t* g);

};

#endif /* MAIN_H_ */
