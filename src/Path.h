#ifndef PATH_H_
#define PATH_H_

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <queue>
#include <map>
#include <string>
#include <cstring>

#include <graphviz/cgraph.h>

namespace std {

class Path {
public:
	Agrec_t h;
	std::map<Agsym_t*, unsigned long long> pathsN;

	static
	queue<Agnode_t*> nodeQueue;

	/**
	 * Increment path values from p to curent
	 * @param p path to be added to 'this'
	 */
	void pathAdd(Path* p);

	/**
	 * Adds path of son to father, recursively calculates path of son if needed
	 */
	void pathAddProcess (Agraph_t* g, Agnode_t* father, Agnode_t* son);

	/**
	 * set's path values to the node's attributes
	 */
	static
	Path* pathSet(Agraph_t* g, Agnode_t* node);

	/**
	 * return node's Path
	 */
	static
	Path* pathFromNode(Agnode_t* node);

	/**
	 * creates Path on the node
	 */
	static
	Path* pathCreateOnNode(Agnode_t* node);

	/**
	 * adds path to labeled and pushes them to nodeQueue
	 */
	static
	void pushLabeled(Agraph_t* g);

	/**
	 * sets node attribute to value
	 */
	void attributeSet(Agnode_t* node, Agsym_t* attribute, unsigned long long value);

	/**
	 * sets node attributes to the values of it's path
	 */
	void attrSync(Agnode_t* node);

	/**
	 * prints pathsN values
	 */
	void printPath();

	/**
	 * prints node in dot format with it's attributes
	 */
	static
	void printNodeDot(Agraph_t* g, Agnode_t* node);
};

#endif /* PATH_H_ */

} /* namespace std */
