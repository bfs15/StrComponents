#include "main.h"
#include "Path.h"

namespace std {

queue<Agnode_t*>  Path::nodeQueue;

void Path::pathAdd(Path* p) {
	map<Agsym_t*, unsigned long long>::iterator it;
	for (it = p->pathsN.begin(); it!= p->pathsN.end(); ++it) {
		pathsN[it->first] += it->second;
	}
}

void Path::pathAddProcess (Agraph_t* g, Agnode_t* father, Agnode_t* son) {
	Path* pathOfSon;

	if ((pathOfSon = Path::pathFromNode(son))) {
		/*DEBUG LOG*/clog <<"*Son processed, adding path of: \""<< agnameof(son) <<"\" to: \""<< agnameof(father) <<"\""<< endl;
		// already processed, value of it's path already calculated, use it
		pathAdd(pathOfSon);
		return;
	}

	pathOfSon = Path::pathCreateOnNode(son);

	Agedge_t *edge;
	// Add node for later processing if it has another father
	// This father will later be checked if already processed or not.
	for (edge = agfstin(g, son); edge; edge = agnxtin(g, edge)) {
		if (edge->node != father) {
			Path::nodeQueue.push(son);
			break;
		}
	}

	// calculate path of son
	/*DEBUG LOG*/clog <<"++++ Calculating path of Son: \""<< agnameof(son) <<"\" came from: \""<< agnameof(father) <<"\""<< endl;

	// for all sons of son
	for (edge = agfstout(g, son); edge; edge = agnxtout(g, edge)) {
		pathOfSon->pathAddProcess(g, son, edge->node);
	}

	/*DEBUG LOG*/clog <<"---- END calculating path of Son: \""<< agnameof(son) <<"\" came from: \""<< agnameof(father) <<"\""<< endl;
	/*DEBUG LOG*/if (DEBUG) pathOfSon->printPath();

	pathAdd(pathOfSon);
}

// Static ////////

// static
Path* Path::pathSet(Agraph_t* g, Agnode_t* node) {
	Path* path = Path::pathFromNode(node);
	bool hasAttr;

	Agsym_t* attribute = NULL;
	while ((attribute = agnxtattr(g, AGNODE, attribute))) {
		unsigned long long value = attrIntFromNode(node, attribute, hasAttr);

		if (hasAttr) {
			if (path == NULL) {
				path = Path::pathCreateOnNode(node);
			}
			/*DEBUG LOG*/ clog << attribute->name << "=" << value << "; ";
			path->pathsN[attribute] = value;
		}
	}
	return path;
}

// static
Path* Path::pathFromNode(Agnode_t* node) {
	return (Path*)aggetrec(node, (char*)"Path", FALSE);
}

// static
Path* Path::pathCreateOnNode(Agnode_t* node) {
	Path* path;
	path = (Path*)agbindrec(node, (char*)"Path",
	sizeof(Path), FALSE);

	path->pathsN = {};

	return path;
}

// static
void Path::pushLabeled(Agraph_t* g) {
	for (Agnode_t* node = agfstnode(g); node; node = agnxtnode(g,node)){
		/*DEBUG LOG*/clog << "\n\nChecking labels of " << agnameof(node) << "\n";

		if (pathSet(g, node) != NULL) {
			nodeQueue.push(node);
			/*DEBUG LOG*/clog << "\nPushed " << agnameof(nodeQueue.back()) << "\n";
		}
	}
}

// Attribute handling ////////

void Path::attributeSet(Agnode_t* node, Agsym_t* attribute, unsigned long long value) {
	string valueStr = to_string(value);
	agxset(node, attribute, (char *)valueStr.c_str());
}

void Path::attrSync(Agnode_t* node) {
	map<Agsym_t*, unsigned long long>::iterator it;
	for (it = pathsN.begin(); it!= pathsN.end(); ++it) {
		attributeSet(node, it->first, it->second);
	}
}

// Print ////////

void Path::printPath() {
	map<Agsym_t*, unsigned long long>::iterator it;
	for (it = pathsN.begin(); it!= pathsN.end(); ++it) {
		cout << it->first->name <<"="<< it->second << " | ";
	}
	cout << endl;
}

// static
void Path::printNodeDot(Agraph_t* g, Agnode_t* node) {
	Path* path = Path::pathFromNode(node);

	cout <<"\t"<< agnameof(node);

	if(path != NULL) {
		path->attrSync(node);

		cout <<" [";

		Agsym_t* attribute = NULL;
		unsigned long long attrN = 0;
		bool hasAttr;

		while ((attribute = agnxtattr(g, AGNODE, attribute))) {
			unsigned long long value = attrIntFromNode(node, attribute, hasAttr);
			if (hasAttr) { // prints only if value is meaningful
				if (attrN > 0) {
					cout << "," ;
				}
				attrN++;
				cout << attribute->name << "=" << value;
			}
		}

		cout<< "]";
	}
	cout << ";" << endl;
}

} /* namespace std */
