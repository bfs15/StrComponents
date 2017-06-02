#include "main.h"
#include "Path.h"

namespace std {

int main() {
	if (!DEBUG) {
		clog.rdbuf(NULL);
	}

	Agraph_t* g = agread(stdin, NULL);

	list<Agnode_t*> topOrder = reversePosOrder(g);

	list<Agnode_t*> componentNodes;
	Agraph_t* subG;
	for (Agnode_t*& node : topOrder) {
		if (*state(node) == 2) {
			componentNodes = searchT(g, node);
			// TODO: change subgraph name!!
			// create subgraph
			subG = agsubg(g, "cluster_" + string(agnameof(componentNodes.front())), TRUE);

			addNodesToGraph(subG, componentNodes);
		}
	}

	outAsDot(g);

	return 0;
}

list<Agnode_t*> reversePosOrder(Agraph_t* g) {
	list<Agnode_t*> rpo;

	for (Agnode_t* node = agfstnode(g); node; node = agnxtnode(g,node)){
		if (state(node) == NULL) {
			/*DEBUG LOG*/clog << "Order call from: " << agnameof(node) << endl;
			reversePosOrder(g, node, rpo);
		}
	}

	return rpo;
}

void reversePosOrder(Agraph_t* g, Agnode_t* node, list<Agnode_t*>& rpo) {
	int* nodeState = stateCreate(node);

	*nodeState = 1;

	// for all sons
	Agedge_t *edge;
	for (edge = agfstout(g, node); edge; edge = agnxtout(g, edge)) {
		if (state(edge->node) == NULL) {
			reversePosOrder(g, edge->node, rpo);
		}
	}

	rpo.push_front(node);
	*nodeState = 2;
}

int* state(Agnode_t* node) {
	return (int*)aggetrec(node, (char*)"state", FALSE);
}

int* stateCreate(Agnode_t* node) {
	return (int*)agbindrec(node, (char*)"state", sizeof(int), FALSE);
}

list<Agnode_t*> searchT (Agraph_t* g, Agnode_t* node) {
	list<Agnode_t*> nodeList;
	queue<Agnode_t*> queue;

	*state(node) = 3;
	queue.push(node);

	while (!queue.empty()) {
		node = queue.front();
		queue.pop();

		Agedge_t* inEdge;
		// for all fathers
		for (inEdge = agfstin(g, node); inEdge; inEdge = agnxtin(g, inEdge)) {
			/*DEBUG LOG*/clog <<"\n====\n" << "Going up from: \""<< agnameof(node) << "\" to \"" << agnameof(inEdge->node) <<"\""<< endl;

			if (*state(inEdge->node) == 2) {
				*state(inEdge->node) = 3;
				nodeList.push_back(inEdge->node);
				queue.push(inEdge->node);
			}
		}
		*state(node) = 4;
	}

	return nodeList;
}


void addNodesToGraph(Agraph_t* g, list<Agnode_t*> nodes) {
	for (Agnode_t*& node : nodes) {
		agsubnode(g, node, TRUE);
	}
}


void outAsDot(Agraph_t* g) {
	cout << "strict digraph countedPaths {\n";

	for (Agnode_t* node = agfstnode(g); node; node = agnxtnode(g, node)){
		// print name of node and pathsN
		Path::printNodeDot(g, node);
		// END print pathsN

		// print out edges of the node
		for (Agedge_t* edge = agfstout(g, node); edge; edge = agnxtout(g, edge)){
			cout << "\t" << agnameof(agtail(edge))
			<< " -> " << agnameof(aghead(edge)) <<";\n";
		}
	}

	cout << "}\n";
}

}; // namespace std
