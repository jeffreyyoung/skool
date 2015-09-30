import sys

with open( sys.argv[ 1 ] ) as fh:
	seq = fh.next().strip()
	line2 = fh.next().strip().split(" ");
	k = int(line2[0])
	l = int(line2[1])
	t = int(line2[2])

clumps = {}

def incrementCount(kmer):
	if kmer in counts:
		counts[kmer] += 1
	else:
		counts[kmer] = 1

for j in range(0, len(seq) - l):
	counts = {}
	sub_seq = seq[j: j + l]

	for i in range(0, len(sub_seq)):
		incrementCount( sub_seq[i : i + k] )

	for key, value in counts.iteritems():
		if value >= t:
			clumps[key] = True;

result = '';
for key, value in clumps.iteritems():
	result += key + ' ';

result.strip();
print result;

