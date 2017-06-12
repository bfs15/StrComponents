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

#define DEBUG FALSE

using namespace std;

enum state { posOrder1, posOrder2, searchT1, searchT2 };

typedef struct {
	Agrec_t h;
	state stateN;
} nodeData;

list<Agnode_t*> reversePosOrder(Agraph_t* g);

void reversePosOrderR(Agraph_t* g, Agnode_t* node, list<Agnode_t*>& rpo);

list<Agnode_t*> searchT (Agraph_t* g, Agnode_t* node);

void addNodesToGraph(Agraph_t* g, list<Agnode_t*> nodes);

nodeData* nodeDataGet(Agnode_t* node);

nodeData* nodeDataCreate(Agnode_t*);

#endif /* main_H_ */
