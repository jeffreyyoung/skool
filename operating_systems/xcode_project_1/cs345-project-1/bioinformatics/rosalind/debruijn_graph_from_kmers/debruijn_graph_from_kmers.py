import sys

def de_bruijn(kmers):
	def to_string(nodes):
		strings = []
		for root, next in nodes.iteritems():
			 strings.append( root + ' -> ' + ','.join(next) )
		return '\n'.join(sorted(strings));
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

	return to_string(nodes)


if __name__ == "__main__":
	with open( sys.argv[ 1 ] ) as fh:
		kmers = []
		for line in fh:
			kmers.append(line.strip())

	print de_bruijn(kmers)