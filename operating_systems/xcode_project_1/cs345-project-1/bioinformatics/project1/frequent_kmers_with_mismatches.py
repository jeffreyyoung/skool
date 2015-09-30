import sys
import itertools

def get_reverse_compliment(kmer):
	compliments = {
		"A": "T",
		"T": "A",
		"C": "G",
		"G": "C"
	}

	comp_seq = ""
	for i in range( len( kmer ) ):
		char = kmer[i]	
		comp_seq += compliments[char]
		
	return comp_seq[ ::-1 ];

def mutations(word, hamming_distance, charset='ATCG'):
    # this enumerates all the positions in word
    for indices in itertools.combinations( range( len( word ) ), hamming_distance ):
        for replacements in itertools.product(charset, repeat=hamming_distance):
            mutation = list(word)
            for index, replacement in zip( indices, replacements ):
                mutation[ index ] = replacement
            yield "".join( mutation )

def increment_count(kmer, counts):
	if kmer in counts:
		counts[kmer] += 1
	else:
		counts[kmer] = 1


def get_frequent_kmers_with_mismatches(seq, k, m):
	counts = {}
	highest_count = 0;

	for j in range(0, len(seq) - k):
		kmer = seq[j: j + k]
		mutation_cache = {};
		for mutation in mutations(kmer, m):
			if mutation not in mutation_cache:
				increment_count(get_reverse_compliment(mutation), counts)
				increment_count(mutation, counts)

				if counts[mutation] > highest_count:
					highest_count = counts[mutation]

			mutation_cache[mutation] = True;

		
	highest_freq_kmers = []

	for key, val in counts.iteritems():
		if val == highest_count:
			highest_freq_kmers.append( key )

	return prune_kmers_not_in_sequence(seq, highest_freq_kmers);

def prune_kmers_not_in_sequence(seq, highest_freq_kmers):
	freq_kmers_in_sequence = [];
	reverse_compliment_seq = get_reverse_compliment(seq);

	for kmer in highest_freq_kmers:
		if kmer in reverse_compliment_seq or kmer in seq:
			freq_kmers_in_sequence.append(kmer)

	return freq_kmers_in_sequence;


