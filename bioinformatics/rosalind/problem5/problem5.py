import sys

with open( sys.argv[ 1 ] ) as fh:
	seq = fh.next().strip()


indexes = [];
minValue = 99999999;
curValue = 0;
for i in range(0, len(seq)):
	char = seq[i];
	if char == 'C':
		curValue += -1;
	if char == 'G':
		curValue += 1;

	if curValue == minValue:
		indexes.append(str(i + 1));

	if curValue < minValue:
		indexes = [str(i + 1)];
		minValue = curValue;

print " ".join(indexes)