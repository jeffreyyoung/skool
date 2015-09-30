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


kmers = ['GGC', 'AAG']

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

print build_profile(kmers);	