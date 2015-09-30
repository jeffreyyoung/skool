import sys
#balanced graph (same number of incoming and outgoing edges)
def build_eulerian_cycle(graph):
	from random import choice
	def remove_edge(graph, a, b):
		graph[a].remove(b);
		if len(graph[a]) is 0:
			del graph[a]
	def build_sub_cycle(graph, start_node):
		cycle = [start_node]
		current = start_node;
		next = None
		while next != start_node:
			next = choice(graph[current])
			remove_edge(graph, current, next);
			cycle.append(next)
			current = next;
		return cycle
	def get_insert_index(graph, tour):
		for i in range(len(tour)):
			node = tour[i]
			if node in graph:
				return i

	start = choice(graph.keys())
	cycle = build_sub_cycle(graph, start)

	while len(graph.keys()) > 0:
		insert_index = get_insert_index(graph, cycle)
		insert_cycle = build_sub_cycle(graph, cycle[insert_index])
		cycle = cycle[insert_index:] + cycle[1:insert_index+1] + insert_cycle[1:]
	return cycle

if __name__ == "__main__":
	import sys
	with open( sys.argv[ 1 ] ) as fh:

		graph = {}

		for line in fh:
			root, points = line.split(' -> ', 1);
			graph[root] = [p.strip() for p in points.split(',')]

        print "->".join(build_eulerian_cycle(graph))