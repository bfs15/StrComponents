
#include "main.h"

using namespace std;

int main() {
	// By searching the ancestors of nodes on topological order,
	// each search won't go farther than the sc component of the node searched,
	// because on any given search the nodes on it's source sc components
	// will already be found (they came first on the topological order);
	// and since it searches ancestors, it won't go to decendant sc comps.
	Agraph_t* g = agread(stdin, NULL);

	/**/if(DEBUG)clog<<"Started reverse pos ordering"<< endl;

	// Initialize all nodes with node.state = not found
	// `nodeD->stateN = posOrder1;`
	list<Agnode_t*> topoOrder = reversePosOrder(g);

	/**/if(DEBUG)clog<<"\n------ Finished reverse pos ordering"<< endl;

	/**/if(DEBUG)clog<<"\n====== Started finding components\n"<< endl;

	list<Agnode_t*> componentNodes;
	
	// For each topoOrder node
	Agraph_t* subG;
	for (Agnode_t*& node : topoOrder) {
		// if not searched yet
		if (nodeDataGet(node)->stateN < searchT1) {
			// Finds all ancestors of node and itself
			componentNodes = searchT(g, node);
			/**/if(DEBUG)clog<<"\n\n=====+ Found component "<< agnameof(componentNodes.front()) << endl;
			// These found nodes will be a component
			// Create subgraph for the component
			subG = agsubg(g, (char*)("cluster_" + string(agnameof(componentNodes.front()))).c_str(), TRUE);

			addNodesToGraph(subG, componentNodes);
		}
	}

	agwrite(g, stdout);

	agclose(g);

	return 0;
}


list<Agnode_t*> reversePosOrder(Agraph_t* g) {
	// list with Reverse Pos Order
	list<Agnode_t*> rpo;

	// for each graph node
	Agnode_t* node;
	for (node = agfstnode(g); node; node = agnxtnode(g,node)){
		// if not processed yet
		if (nodeDataGet(node) == NULL) {
			/**/if(DEBUG)clog<<"++++++++++ Order call on node "<< agnameof(node) << endl;
			// orders every son of it (+itself)
			reversePosOrderR(g, node, rpo);
			/**/if(DEBUG)clog<<"---------- Order call finished on node "<< agnameof(node) << endl;
		}
	}

	return rpo;
}


void reversePosOrderR(Agraph_t* g, Agnode_t* node, list<Agnode_t*>& rpo) {
	/**/if(DEBUG)clog<<"<><><> OrderR call node: " << agnameof(node) << endl;
	nodeData* nodeD = nodeDataCreate(node);

	nodeD->stateN = posOrder1;
	/**/if(DEBUG)clog<<"state set to "<< nodeDataGet(node)->stateN  <<endl;

	// for all sons
	Agedge_t *edge;
	for (edge = agfstout(g, node); edge; edge = agnxtout(g, edge)) {
		/**/if(DEBUG)clog<< agnameof(node) <<" --> checking son "<< agnameof(edge->node) << endl;
		if (nodeDataGet(edge->node) == NULL) {
			reversePosOrderR(g, edge->node, rpo);
		}
	}

	rpo.push_front(node);
	/**/if(DEBUG)clog<<">>> Pushed on rpo node " << agnameof(node) << endl;
	nodeD->stateN = posOrder2;
	/**/if(DEBUG)clog<<"state set to " << nodeDataGet(node)->stateN  <<endl;
}

/**
 * searches in inEdge direction (parents)
 * analogous to searching in a translated graph
 */
list<Agnode_t*> searchT (Agraph_t* g, Agnode_t* node) {
	/**/if(DEBUG)clog<<"+0-0+ Searching started from node "<< agnameof(node) << endl;

	// List of nodes found in seach
	list<Agnode_t*> nodeList;
	// bfs queue
	queue<Agnode_t*> queue;
	// Start with the root note in the queue
	nodeDataGet(node)->stateN = searchT1;
	nodeList.push_back(node);
	queue.push(node);

	while (!queue.empty()) {
		node = queue.front();
		queue.pop();

		/**/if(DEBUG)clog<<"O-O- node "<< agnameof(node) <<" checking parents"<< endl;
		// for each node parent
		Agedge_t* inEdge;
		for (inEdge = agfstin(g, node); inEdge; inEdge = agnxtin(g, inEdge)) {
			/**/if(DEBUG)clog<<"state of node "<< agnameof(node) <<" = "<< nodeDataGet(inEdge->node)->stateN << endl;

			// if (parent.state not found)
			if (nodeDataGet(inEdge->node)->stateN == posOrder2) {
				/**/if(DEBUG)clog<< agnameof(node) << " found node " << agnameof(inEdge->node) << endl;
				// parent.state = found;
				nodeDataGet(inEdge->node)->stateN = searchT1;
				nodeList.push_back(inEdge->node);
				// search this guy's parents also
				queue.push(inEdge->node);
			}
		}
		/**/if(DEBUG)clog<<"--- node "<< agnameof(node) <<" finished checking parents"<< endl;
		nodeDataGet(node)->stateN = searchT2;
	}

	/**/if(DEBUG)clog<<"-0-0- Searching finished from node "<< agnameof(node) << endl;
	return nodeList;
}


void addNodesToGraph(Agraph_t* g, list<Agnode_t*> nodes) {
	/**/if(DEBUG)clog<<"Adding nodes"<< endl;
	for (Agnode_t*& node : nodes) {
		/**/if(DEBUG)clog<<agnameof(node) <<"; ";
		agsubnode(g, node, TRUE);
	}
	/**/if(DEBUG)clog<< endl;
}


nodeData* nodeDataGet(Agnode_t* node) {
	return (nodeData*)aggetrec(node, (char*)"nodeData", FALSE);
}


nodeData* nodeDataCreate(Agnode_t* node) {
	/**/if(DEBUG)clog<<"+*+*+ created state of node "<< agnameof(node) << endl;
	return (nodeData*)agbindrec(node, (char*)"nodeData", sizeof(nodeData), FALSE);
}
