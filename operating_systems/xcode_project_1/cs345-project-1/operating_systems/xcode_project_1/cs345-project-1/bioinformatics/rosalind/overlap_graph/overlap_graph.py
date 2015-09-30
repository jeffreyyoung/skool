import sys

def overlap_graph(patterns):
	def to_string(nodes):
		strings = []
		for root, next in nodes.iteritems():
			 strings.append( root + ' -> ' + ','.join(next) )
		return '\n'.join(sorted(strings));

	def find_next(patterns, original):
		suffix = original[1:]
		
		for pattern in patterns:
			if pattern.startswith(suffix) and pattern is not original:
				return pattern;

	nodes = {}
	for pattern in patterns:

		next = find_next(patterns,  pattern);

		if pattern in nodes and next is not None:
			nodes[pattern].add(next)
		elif next is not None:
			nodes[pattern] = set()
			nodes[pattern].add(next)

	

	return to_string(nodes)


if __name__ == "__main__":
	with open( sys.argv[ 1 ] ) as fh:
		patterns = [];
		for line in fh:
			patterns.append(line.strip())

	print overlap_graph(patterns)