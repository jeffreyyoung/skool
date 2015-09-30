import sys

#balanced graph (same number of incoming and outgoing edges)
def build_eulerian_path(graph):
	from random import choice
	from copy import deepcopy
	def find_start(graph):
		for start in graph.keys():
			out_degree = len( graph[start] )
			in_degree = sum( [1 for a,points_to in graph.items() if start in points_to] )
			if out_degree > in_degree:
				return start
	def find_end(graph):
		for key, value in graph.iteritems():
			for node in value:
				if node not in graph:
					return node

	def add_missing_edge(graph, start, end):
		if end in graph:
			graph[end].append(start)
		else:
			graph[end] = [start]

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
	def get_cycle(graph, start, end):
		cycle = build_sub_cycle(graph, start)

		while len(graph.keys()) > 0:
			insert_index = get_insert_index(graph, cycle)
			insert_cycle = build_sub_cycle(graph, cycle[insert_index])
			cycle = cycle[insert_index:] + cycle[1:insert_index+1] + insert_cycle[1:]
		return cycle

	start = find_start(graph);
	end = find_end(graph)
	add_missing_edge(graph, start, end)
	cycle = get_cycle(graph, start, end)
	for i in range( len(cycle) ):
		path = cycle[i:] + cycle[:i]
		if path[0] == start and path[-1] == end:
			return path;
	

if __name__ == "__main__":
	import sys
	with open( sys.argv[ 1 ] ) as fh:

		graph = {}

		for line in fh:
			root, points = line.split(' -> ', 1);
			graph[root] = [p.strip() for p in points.split(',')]

        print "->".join( build_eulerian_path(graph) )