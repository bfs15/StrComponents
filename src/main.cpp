#include "main.h"
#include "Path.h"

using namespace std;

int main() {
	if (!DEBUG) {
		clog.rdbuf(NULL);
	}

	Agraph_t *g = agread(stdin, NULL);

	Path::pushLabeled(g);

	/*DEBUG LOG*/clog << "\n====\n" << "Stack size " << Path::nodeQueue.size() << "\n";

	while(!Path::nodeQueue.empty()) {
		Agnode_t* node = Path::nodeQueue.front();
		Path::nodeQueue.pop();
		/*DEBUG LOG*/clog << endl << "--------------" << endl << "POP! " << agnameof(node) << "\n";

		Agedge_t* inEdge;
		// for all fathes
		for (inEdge = agfstin(g, node); inEdge; inEdge = agnxtin(g, inEdge)) {
		/*DEBUG LOG*/clog <<"\n====\n" << "Going up from: \""<< agnameof(node) << "\" to \"" << agnameof(inEdge->node) <<"\""<< endl;
			goUp(g, inEdge->node);
		}
	}

	outAsDot(g);

	return 0;
}

void goUp (Agraph_t* g, Agnode_t* node) {
	Path* path;

	if ((path = Path::pathFromNode(node))) {
		/*DEBUG LOG*/clog << "Already processed, returned: " << agnameof(node) << endl;
		/*DEBUG LOG*/clog << "sz: " << path->pathsN.size() << endl;
		return; // already processed, doesn't need to go up
	}

	path = Path::pathCreateOnNode(node);

	Agedge_t *edge;
	// for all sons
	for (edge = agfstout(g, node); edge; edge = agnxtout(g, edge)) {
		// calculate their paths
		path->pathAddProcess(g, node, edge->node);
	}

	/*DEBUG LOG*/clog <<"\n====\n" << "Finished: \""<< agnameof(node) << "\" Path equals " << endl;
	/*DEBUG LOG*/if (DEBUG) path->printPath();

	Agedge_t* inEdge;
	// for all fathers
	for (inEdge = agfstin(g, node); inEdge; inEdge = agnxtin(g, inEdge)) {
	/*DEBUG LOG*/clog <<"\n====\n" << "Going up from: \""<< agnameof(node) << "\" to \"" << agnameof(inEdge->node) <<"\""<< endl;
		goUp(g, inEdge->node);
	}
}


unsigned long long charsToInt(char* s, bool& hasInt) {
	char* pEnd;
	unsigned long long ret = strtoull(s, &pEnd, 10);
	hasInt = (pEnd != s);
	return ret;
}

unsigned long long attrIntFromNode(Agnode_t* node, Agsym_t* attribute, bool& hasAttr) {
	return charsToInt(agxget(node, attribute), hasAttr);
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
