def expand(graph, start, end, current_path, traversed_edges, paths):
    if current_path[0] == start and current_path[-1] == end:
        paths.append(list(current_path))
    if current_path[-1] in graph:
        for edge in graph[current_path[-1]]:
            if (current_path[-1], edge[0]) not in traversed_edges:
                path = list(current_path)
                path.append(edge[0])
                edges = set(traversed_edges)
                edges.add((current_path[-1], edge[0]))
                expand(graph, start, end, path, edges, paths )

def get_all_paths(graph, start, end):
    paths = []
    expand(graph, start, end, [start],set(), paths)
    return paths;

def get_edge_weight(graph, start, end):
    if start in graph:
        for e in graph[start]:
            if e[0] == end:
                return int(e[1]);
    return None;

def get_weight(graph, path):
    weight = 0;
    for i in range( len(path) - 1):
        weight += get_edge_weight(graph, path[i], path[i+1])
    return weight

def longest_path(graph, start, end):
    all_paths = get_all_paths(graph, start, end)
    longest_path = []
    longest_path_weight = 0;
    for path in all_paths:
        weight = get_weight(graph, path);
        if weight >= longest_path_weight:
            longest_path_weight = weight
            longest_path = path
    return (longest_path, longest_path_weight)


if __name__ == "__main__":
    import sys
    with open( sys.argv[ 1 ] ) as fh:
    	source_node = next(fh).strip()
        sink_node = next(fh).strip();
        patterns = []
        graph = {}
        for line in fh:
            t = line.strip().replace('->',':').split(':');
            source = t[0]
            sink = t[1]
            weight = t[2]
            if source not in graph.keys():
                graph[source] = [(sink, weight)]
            else:
                graph[source].append( (sink, weight) )
        result = longest_path(graph, source_node, sink_node)
        print result[1]
        print '->'.join(result[0])
