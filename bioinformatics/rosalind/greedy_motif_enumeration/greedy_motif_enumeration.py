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
				profile[nucleotide][i] = float(counts[nucleotide][i] / len(kmers))

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
			for key, value in counts:
				if greatest_value < value:
					base = key;
			most_frequent_bases[pos] = key

		score = 0;
		for motif in motifs:
			for i in range( len(motif) ):
				if motif[1] != most_frequent_bases[i]:
					score += 1;
		return score;


	def get_kmers(line, k):
		kmers = []
		for i in range(0, len(line) - k + 1):
			kmers.append( line[i : i + k] )
		return kmers;

	best_motifs = [];
	for line in dna:
		best_motifs.append( get_kmers(line, k)[0] );
	
	for kmer in get_kmers(dna[0], k):
		motifs = [kmer]
		for i in range(1, len(dna)):
			line = dna[i]
			profile = build_profile(motifs)
			profile_most_probable_kmer = get_most_probable_kmer(line, profile);
			motifs.append(profile_most_probable_kmer)
			print motifs	
			
		if score(motifs) > score(best_motifs):
			best_motifs = motifs;

	return best_motifs;

if __name__ == "__main__":
	with open( sys.argv[ 1 ] ) as fh:
		line = next(fh).strip();
		k, d = [int(x) for x in line.split(' ')]
		dna = []
		for line in fh:
			dna.append(line.strip())

	print greedy_motif_enumeration(dna, k, d)


		#get most frequent base at each position
		#0, 1, 2

		#count mismatches at each position

		#return count
		# chose most frequent letter in each position
		#score for [GGC, AAG, CAA, CAC, CAA] is 6


	best_motifs = [] #first kmers in each line of dna
					 #[GGC, AAG, CAA, CAC, CAA]

			#make profile from motifs


#loop on kmers in first line
	#loop on lines in dna starting at second


####################
#for each kmer in first dna line;
    #profile 
    #for each line in dna
    	#get_most_probable_kmer(line, profile)





#psuedo counts notes
 #rawr rawr




# possiblePatterns = {}
# finalPatterns = set()

# for line in dna:
# 	linePatterns = set())
# 	for i in range(0, len(line) -k + 1):
# 		for kmer in mutations(line[i : i +k], d):
# 			linePatterns.add(kmer)

# 	for p in linePatterns:
# 		if p in possiblePatterns:
# 			possiblePatterns[p] += 1
# 		else:
# 			possiblePatterns[p] = 1

# for key, val in possiblePatterns.iteritems():
# 	if val >= len(dna):
# 		finalPatterns.add(key)

# print " ".join(finalPatterns);