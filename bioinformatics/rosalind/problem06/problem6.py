import sys

with open( sys.argv[ 1 ] ) as fh:
	pattern = fh.next().strip()
	seq = fh.next().strip()
	ALLOWED_MISMATCHES = int( fh.next().strip() )

def get_edit_distance(str1, str2):
	edit_distance = 0;
	if len(str1) != len(str2):	
		return 99999999;

	for i in range(0, len(str1)):
		if str1[i] != str2[i]:
			edit_distance += 1;
	return edit_distance;
indexes = [];
for j in range(0, len(seq) - len(pattern)):
	sub_seq = seq[j: j + len(pattern)]
	#print sub_seq;
	edit_distance = get_edit_distance(sub_seq, pattern)
	#print edit_distance;
	if edit_distance <= ALLOWED_MISMATCHES:
		indexes.append(str(j));

print " ".join( indexes )




