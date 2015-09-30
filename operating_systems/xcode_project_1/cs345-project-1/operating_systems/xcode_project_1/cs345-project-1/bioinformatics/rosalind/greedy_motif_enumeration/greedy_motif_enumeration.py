import sys

def greedy_motif_enumeration(dna, k, t):

	def build_profile(kmers):
		counts = {};
		counts['A'] = [0] * len(kmers[0])
		counts['C'] = [0] * len(kmers[0])
		counts['T'] = [0] * len(kmers[0])
		counts['G'] = [0] * len(kmers[0])

		profile = {};
		profile['A'] = [0] * len(kmers[0])
		profile['C'] = [0] * len(kmers[0])
		profile['T'] = [0] * len(kmers[0])
		profile['G'] = [0] * len(kmers[0])

		for kmer in kmers:
			for i in range( len(kmer) ):
				nucleotide = kmer[i];
				counts[nucleotide][i] += 1;
				profile[nucleotide][i] = float(counts[nucleotide][i] / float(len(kmers)))

		return profile;

	def get_kmer_probability(kmer, profile):
		probability = 1;
		for i in range( len(kmer) ):
			nucleotide = kmer[i];
			probability = probability * profile[nucleotide][i]

		return probability;

	def get_most_probable_kmer(line, profile):
		highest_probability = -1;
		highest_probability_kmer = None;
		for i in range(0, len(line) - k + 1):
			kmer = line[i : i + k]
			probability = get_kmer_probability(kmer, profile)
			if highest_probability < probability:
				highest_probability = probability;
				highest_probability_kmer = kmer;
		return highest_probability_kmer;

	def score(motifs):
		motif_length = len( motifs[0] );
		most_frequent_bases = [None] * motif_length;
		counts = [0] * motif_length;
		for pos in range(0, motif_length):
			counts = {
				'A': 0,
				'C': 0,
				'T': 0,
				'G': 0
			}
			for motif in motifs:
				counts[motif[pos]] += 1
			greatest_value = -1;
			base = None;
			for key, value in counts.iteritems():
				if greatest_value < value:
					base = key;
					greatest_value = value
			most_frequent_bases[pos] = base

		score = 0;
		for motif in motifs:
			for i in range( len(motif) ):
				if motif[i] != most_frequent_bases[i]:
					score += 1;
		return score;


	def get_kmers(line, k):
		kmers = []
		for i in range(0, len(line) - k + 1):
			kmers.append( line[i : i + k] )
		return kmers;

	best_motifs = [string[:k] for string in dna]
	
	for kmer in get_kmers(dna[0], k):
		motifs = [kmer]
		for i in range(1, len(dna)):
			line = dna[i]
			profile = build_profile(motifs)
			profile_most_probable_kmer = get_most_probable_kmer(line, profile);
			motifs.append(profile_most_probable_kmer)
			
		if score(motifs) < score(best_motifs):
			best_motifs = motifs;

	return best_motifs;



if __name__ == "__main__":
	with open( sys.argv[ 1 ] ) as fh:
		line = next(fh).strip();
		k, d = [int(x) for x in line.split(' ')]
		dna = []
		for line in fh:
			dna.append(line.strip())

	print "\n".join(greedy_motif_enumeration(dna, k, d))