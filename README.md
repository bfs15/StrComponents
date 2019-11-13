# Graph Strong Component getter

CI065 / CI755 - Algoritmos e Teoria dos Grafos

Trabalho de Implementação 3

Bruno Freitas Serbena GRR20151343

The algorithm works the following way:

```C++
strongComponents(G) {
	// Initialize all nodes with node.state = not found
	List<node> topoOrder = reversePosOrder(G)

	// By searching the ancestors of nodes on topological order,
	// each search won't go farther than the sc component of the node searched,
	// because on any given search the nodes on it's source sc components
	// will already be found (they came first on the topological order);
	// and since it searches ancestors, it won't go to decendant sc comps.
	for each topoOrder node {
		if (node not found yet){
			component = searchTransposed(node)
			// Finds all ancestors of node and itself
			// These found nodes will be a component
			addComponent(G, component)
		}
	}
}


reversePosOrder(G) {
	List<node> rpo

	for each G node {
		if (node not ordered) {
			reversePosOrderR(G, node, rpo)
		}
	}

	return rpo
}


reversePosOrderR(G, node, rpo) {
	node.state = seen

	for each node son {
		if (son.state == NULL)
			reversePosOrderR(G, son, rpo)
	}

	node.state = ordered
	rpo.push_front(node)
}


searchTransposed(G, node) {
	// List of nodes found in seach
	list<Agnode_t*> nodeList;
	// bfs queue
	queue<Agnode_t*> queue;
	// Start with the root note in the queue
	node.state = found;
	nodeList.push_back(node);
	queue.push(node);

	while (!queue.empty()) {
		node = queue.pop();

		for each node parent {
			if (parent.state not found) {
				parent.state = found;
				nodeList.push_back(parent);
				// search this guy's parents also
				queue.push(parent);
			}
		}
	}
	return nodeList;
}
```