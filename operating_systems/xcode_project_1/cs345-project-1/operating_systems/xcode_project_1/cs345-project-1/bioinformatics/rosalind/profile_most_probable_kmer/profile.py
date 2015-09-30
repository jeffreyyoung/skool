import sys

with open( sys.argv[ 1 ] ) as fh:
	seq = next(fh).strip()
	k = int( next(fh).strip() )
	profile = {}
	nucleotides = ['A', 'C', 'G', 'T']
	counter = 0;
	for line in fh:
		profile[nucleotides[counter]] = [float(x) for x in line.split(' ')]
		counter += 1

def get_probability(kmer, profile):
	probability = 1;
	for i in range( len(kmer) ):
		probability = probability * profile[kmer[i]][i]
	return probability;

def most_probable_kmer(seq, k, profile):
	highest_probability = 0;
	highest_kmer = None
	for i in range(len(seq) - k + 1):
		kmer = seq[i : i + k]
		p = get_probability(kmer, profile)
		if p > highest_probability:
			highest_kmer = kmer;
			highest_probability = p
	return highest_kmer

print most_probable_kmer(seq, k, profile);