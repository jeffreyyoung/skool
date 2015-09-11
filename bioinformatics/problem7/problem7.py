import sys
import itertools

with open( sys.argv[ 1 ] ) as fh:
	seq = fh.next().strip()
	k, m = map( int, fh.next().strip().split() )

counts = {}
highest_count = 0;

def mutations(word, hamming_distance, charset='ATCG'):
    # this enumerates all the positions in word
    for indices in itertools.combinations( range( len( word ) ), hamming_distance ):
        for replacements in itertools.product(charset, repeat=hamming_distance):
            mutation = list(word)
            for index, replacement in zip( indices, replacements ):
                mutation[ index ] = replacement
            yield "".join( mutation )

def increment_count(kmer):
	global highest_count;
	if kmer in counts:
		counts[kmer] += 1
	else:
		counts[kmer] = 1

	if counts[mutation] > highest_count:
		highest_count = counts[mutation]

for j in range(0, len(seq) - k):
	kmer = seq[j: j + k]
	mutation_cache = {};
	for mutation in mutations(kmer, m):
		if mutation not in mutation_cache:
			increment_count(mutation)

		mutation_cache[mutation] = True;

		
highest_freq_kmers = []

for key, val in counts.iteritems():
	if val == highest_count:
		highest_freq_kmers.append( key )

print " ".join( highest_freq_kmers )


