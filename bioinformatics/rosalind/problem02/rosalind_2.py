import sys

with open( sys.argv[ 1 ] ) as fh:
	seq = fh.next().strip()

compliments = {
	"A": "T",
	"T": "A",
	"C": "G",
	"G": "C"
}

comp_seq = ""
for i in range( len( seq ) ):
	char = seq[i]	
	comp_seq += compliments[char]
	
print comp_seq[ ::-1 ];
