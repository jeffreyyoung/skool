import sys
import itertools;
with open( sys.argv[ 1 ] ) as fh:
	seq = fh.next().strip()
	k = int( fh.next().strip() )

indexes = []
counts = []


for combo in itertools.product("ACGT", repeat=k):
	kmer = ''.join(combo)
	count = 0;
	for i in range( len(seq) ):
		currrent_kmer = seq[i : i + k]
		if currrent_kmer == kmer:
			count += 1;
	counts.append( str(count) )

print " ".join(counts)

