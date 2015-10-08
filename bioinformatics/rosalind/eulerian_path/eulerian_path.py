import sys

#balanced graph (same number of incoming and outgoing nodes)
def build_eulerian_path(graph):
	from random import choice
	def find_start(graph):
		for start in graph.keys():
			out_degree = len( graph[start] )
			in_degree = sum( [1 for a,points_to in graph.items() if start in points_to] )
			if out_degree > in_degree:
				return start
	start_node = find_start(graph)
	nodes = [start_node]
	path = []
	while len(nodes) != 0:
		current_node = nodes[-1]
		if current_node not in graph:
			path.append(nodes.pop())
		else:
			out_nodes = graph[current_node]
			if len(out_nodes) > 0:
				edge = choice(out_nodes)
				graph[current_node].remove(edge)
				nodes.append(edge)
			else:
				path.append(nodes.pop())
	return path[::-1]

if __name__ == "__main__":
	import sys
	with open( sys.argv[ 1 ] ) as fh:

		graph = {}

		for line in fh:
			root, points = line.split(' -> ', 1);
			graph[root] = [p.strip() for p in points.split(',')]

        print "->".join( build_eulerian_path(graph) )
