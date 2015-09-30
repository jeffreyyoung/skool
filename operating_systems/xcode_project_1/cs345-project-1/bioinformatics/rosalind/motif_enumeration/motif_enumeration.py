import sys
import itertools

with open( sys.argv[ 1 ] ) as fh:
	line = next(fh).strip();
	k, d = [int(x) for x in line.split(' ')]
	Dna = []
	for line in fh:
		Dna.append(line.strip())

def mutations(word, hamming_distance, charset='ATCG'):
    # this enumerates all the positions in word
    for indices in itertools.combinations( range( len( word ) ), hamming_distance ):
        for replacements in itertools.product(charset, repeat=hamming_distance):
            mutation = list(word)
            for index, replacement in zip( indices, replacements ):
                mutation[ index ] = replacement
            yield "".join( mutation )

def get_all_kmers(Dna, k, d):
	kmers = set();
	for pattern in Dna:
		for i in range(len(pattern) - k + 1):
			kmer = pattern[i: i + k]
			kmers.add(kmer)
			for mutation in mutations(kmer, d):
				kmers.add(mutation)
	return kmers

def get_all_mutations(kmer, d):
	muts = set()
	muts.add(kmer)
	for m in mutations(kmer, d):
		muts.add(m)
	return muts;

def is_in_all_Dna_patterns(kmers, Dna):

	for pattern in Dna:
		has_one = False
		for kmer in kmers:
			if kmer in pattern:
				has_one = True;
				break;
		if has_one == False:
			return False;
	return True;

def MotifEnumeration(Dna, k, d):
	kmers = get_all_kmers(Dna, k, d)
	patterns = set()
	for kmer in kmers:
		mutations = get_all_mutations(kmer, d)
		if is_in_all_Dna_patterns(mutations, Dna):
			patterns.add(kmer);

	return patterns
			
print " ".join(sorted(MotifEnumeration(Dna, k, d)))



