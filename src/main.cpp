#include "main.h"

using namespace std;

int main() {
	if (!DEBUG) {
		clog.rdbuf(NULL);
	}

	Agraph_t* g = agread(stdin, NULL);

	/*DEBUG LOG*/clog <<"Started ordering"<< endl;

	list<Agnode_t*> topOrder = reversePosOrder(g);

	/*DEBUG LOG*/clog <<"\n------ Finished ordering"<< endl;

	/*DEBUG LOG*/clog <<"\n====== Started finding components\n"<< endl;

	list<Agnode_t*> componentNodes;
	Agraph_t* subG;
	for (Agnode_t*& node : topOrder) {
		if (stateN(node)->n == 2) {
			componentNodes = searchT(g, node);

			/*DEBUG LOG*/clog << "\n\n=====+ Found component "<< agnameof(componentNodes.front()) << endl;

			// create subgraph
			subG = agsubg(g, (char*)("cluster_" + string(agnameof(componentNodes.front()))).c_str(), TRUE);

			addNodesToGraph(subG, componentNodes);
		}
	}

	// outAsDot(g);
	agwrite(g, stdout);

	return 0;
}
void reversePosOrderR(Agraph_t* g, Agnode_t* node, list<Agnode_t*>& rpo) {
	/*DEBUG LOG*/clog << "<><><> OrderR call node: " << agnameof(node) << endl;
	state* nodeState = stateNCreate(node);

	nodeState->n = 1;

	/*DEBUG LOG*/clog<< "state set to " << stateN(node)->n  <<endl;

	// for all sons
	Agedge_t *edge;
	for (edge = agfstout(g, node); edge; edge = agnxtout(g, edge)) {
		/*DEBUG LOG*/clog << agnameof(node) <<" --> checking son "<< agnameof(edge->node) << endl;
		if (stateN(edge->node) == NULL) {
			reversePosOrderR(g, edge->node, rpo);
		}
	}

	/*DEBUG LOG*/clog << ">>> Pushed on rpo: " << agnameof(node) << endl;
	/*DEBUG LOG*/clog<< "state set to " << stateN(node)->n  <<endl;
	rpo.push_front(node);
	nodeState->n = 2;
}

list<Agnode_t*> reversePosOrder(Agraph_t* g) {
	list<Agnode_t*> rpo;

	for (Agnode_t* node = agfstnode(g); node; node = agnxtnode(g,node)){
		if (stateN(node) == NULL) {
			/*DEBUG LOG*/clog << "Order call"<< endl;
			reversePosOrderR(g, node, rpo);
		}
	}

	return rpo;
}

list<Agnode_t*> searchT (Agraph_t* g, Agnode_t* node) {
	/*DEBUG LOG*/clog << "Searching started from: "<< agnameof(node) << endl;

	list<Agnode_t*> nodeList;
	queue<Agnode_t*> queue;

	stateN(node)->n = 3;
	nodeList.push_back(node);
	queue.push(node);

	while (!queue.empty()) {
		node = queue.front();
		queue.pop();

		/*DEBUG LOG*/clog << "O-O- Node: "<< agnameof(node) <<" checking parents"<< endl;
		Agedge_t* inEdge;
		// for all parents
		for (inEdge = agfstin(g, node); inEdge; inEdge = agnxtin(g, inEdge)) {
			/*DEBUG LOG*/clog<<"state of: "<< agnameof(node) <<" = "<< stateN(inEdge->node)->n << endl;

			if (stateN(inEdge->node)->n == 2) {
				/*DEBUG LOG*/clog <<""<< agnameof(node) << " found node: " << agnameof(inEdge->node) << endl;
				stateN(inEdge->node)->n = 3;
				nodeList.push_back(inEdge->node);
				queue.push(inEdge->node);
			}
		}
		/*DEBUG LOG*/clog << "--- Node: "<< agnameof(node) <<" finished"<< endl;
		stateN(node)->n = 4;
	}

	/*DEBUG LOG*/clog << "Searching finished from node: "<< agnameof(node) << endl;
	return nodeList;
}

state* stateNCreate(Agnode_t* node) {
	/*DEBUG LOG*/clog<<"created state of: "<< agnameof(node) <<" +-+-"<< endl;
	return (state*)agbindrec(node, (char*)"stateN", sizeof(state), FALSE);
}

state* stateN(Agnode_t* node) {
	return (state*)aggetrec(node, (char*)"stateN", FALSE);
}

void addNodesToGraph(Agraph_t* g, list<Agnode_t*> nodes) {
	/*DEBUG LOG*/clog<<"Adding nodes"<< endl;
	for (Agnode_t*& node : nodes) {
		/*DEBUG LOG*/clog<<agnameof(node) <<"; ";
		agsubnode(g, node, TRUE);
	}
	/*DEBUG LOG*/clog<< endl;
}
