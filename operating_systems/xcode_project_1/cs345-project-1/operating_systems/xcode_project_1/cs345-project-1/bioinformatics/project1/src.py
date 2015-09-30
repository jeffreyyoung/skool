
#parse fasta file
import sys
from frequent_kmers_with_mismatches import get_frequent_kmers_with_mismatches
from skew import get_skew_indices;

with open( sys.argv[1] ) as fh:
	seq = '';
	for line in fh:
		if line[0] == '>':
			seq_header = line.strip()
		else:
			seq += line.strip()

#reduce search space by looking for minimum skew
indexes = get_skew_indices(seq);

print indexes;
index = indexes[0]

print get_frequent_kmers_with_mismatches( seq[ index : index + 500], 9, 1 )

#frequent words with mismatches


