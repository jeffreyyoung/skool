
def build_profile(kmers):
	counts = {};
	counts['A'] = [1.0] * len(kmers[0])
	counts['C'] = [1.0] * len(kmers[0])
	counts['T'] = [1.0] * len(kmers[0])
	counts['G'] = [1.0] * len(kmers[0])

	profile = {};
	profile['A'] = [0.0] * len(kmers[0])
	profile['C'] = [0.0] * len(kmers[0])
	profile['T'] = [0.0] * len(kmers[0])
	profile['G'] = [0.0] * len(kmers[0])

	for kmer in kmers:
		for i in range( len(kmer) ):
			nucleotide = kmer[i];
			counts[nucleotide][i] += 1;
			print counts[nucleotide][i]
			print len(kmers) + 1
			print counts[nucleotide][i] / ( len(kmers) + 1 )
			print '-------'
			profile[nucleotide][i] = float(counts[nucleotide][i]) / float(len(kmers) + 4)

	return profile;

kmers = ['ACT', 'ACG']

print build_profile(kmers);