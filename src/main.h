#ifndef main_H_
#define main_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <queue>
#include <map>
#include <list>
#include <string>
#include <cstring>

#include <graphviz/cgraph.h>

#define DEBUG TRUE

using namespace std;

typedef struct {
	Agrec_t h;
	long long int n;
} state;

list<Agnode_t*> reversePosOrder(Agraph_t* g);

// void reversePosOrderR(Agraph_t* g, Agnode_t* node, list<Agnode_t*>& rpo);

list<Agnode_t*> searchT (Agraph_t* g, Agnode_t* node);

void addNodesToGraph(Agraph_t* g, list<Agnode_t*> nodes);

state* stateN(Agnode_t* node);

state* stateNCreate(Agnode_t*);

void outAsDot(Agraph_t* g);

#endif /* main_H_ */
