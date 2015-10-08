import sys

def reconstruct_string(kmers):
    #builds a debruijn graph
    def de_bruijn(kmers):
        nodes = {}
        for kmer in kmers:
            root = kmer[:-1]
            next = kmer[1:]
            if root in nodes:
                nodes[root].append(next)
                nodes[root].sort()
            else:
                nodes[root] = []
                nodes[root].append(next)

        return nodes

    #builds a eulerian path
    def build_eulerian_path(graph):
        from random import choice

        def find_start(graph):
            for start in graph.keys():
                out_degree = len(graph[start])
                in_degree = sum(
                    [1 for a, points_to in graph.items() if start in points_to])
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

    def to_string(path):
        res = path[0];
        for i in range(1, len(path)):
            res += path[i][-1:]
        return res

    return to_string( build_eulerian_path( de_bruijn(kmers)))


if __name__ == "__main__":
    with open(sys.argv[1]) as fh:
        k = int(next(fh).strip())
        kmers = []
        for line in fh:
            kmers.append(line.strip())

    print reconstruct_string(kmers)
