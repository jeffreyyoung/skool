import sys

def de_bruijn(text, k):
	def to_string(nodes):
		strings = []
		for root, next in nodes.iteritems():
			 strings.append( root + ' -> ' + ','.join(next) )
		return '\n'.join(sorted(strings));
	nodes = {}
	l = k - 1
	for i in range(len( text ) - l):
		root = text[i:i+l]
		next = text[i + 1: i +1 + l]

		if root in nodes:
			nodes[root].add(next)
		else:
			nodes[root] = set()
			nodes[root].add(next)

	return to_string(nodes)


if __name__ == "__main__":
	with open( sys.argv[ 1 ] ) as fh:
		k = int( next(fh).strip() );
		text = next(fh).strip()

	print de_bruijn(text, k)