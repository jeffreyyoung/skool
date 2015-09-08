import sys

with open( sys.argv[ 1 ] ) as fh:
	query = fh.next().strip()
	seq = fh.next().strip()

indexes = []

for i in range( len(seq) ):
	if seq[i : i + len( query )] == query:
		indexes.append(str(i));

print " ".join(indexes)
