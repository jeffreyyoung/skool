import sys, itertools

def mutations(word, hamming_distance, charset='ATCG'):
    # this enumerates all the positions in word
    for indices in itertools.combinations( range( len( word ) ), hamming_distance ):
        for replacements in itertools.product(charset, repeat=hamming_distance):
            mutation = list(word)
            for index, replacement in zip( indices, replacements ):
                mutation[ index ] = replacement
            yield "".join( mutation )

def reverseComplement(seq):
	newSeq = ''

	for i in range(len(seq)):
		if seq[i] == 'A':
			newSeq += 'T'
		elif seq[i] == 'T':
			newSeq += 'A'
		elif seq[i] == 'G':
			newSeq += 'C'
		else:
			newSeq += 'G'

	return newSeq[::-1]	

def increment_count(kmer):
	global highest_kmer_count;
	if kmer in counts:
		counts[kmer] += 1
	else:
		counts[kmer] = 1
	if counts[kmer] > highest_kmer_count:
		highest_kmer_count = counts[kmer]

#kmer = "ACGT"
#ham_dist = 1

#for i in mutations(kmer, ham_dist):
#	print i
with open(sys.argv[1]) as fh:
	#fh is file handle
	info = fh.readlines()
	seq = info[0].strip()
	#seq = fh.next().strip()
	infostr = info[1].strip()
	intlist = infostr.split()
	k = intlist[0]
	d = intlist[1]
	k = int(k)
	d = int(d)
	print d
	print k		

counts = dict()
highest_kmer_count = 0
most_frequent_kmers = []
# to iterate over all kmers
for i in range(len(seq) - k+1):
	# get the kmer
	kmer = seq[i : i + k] # i to i+k substring
	unique_mutated_kmers = {}
	for mutated_kmer in mutations(kmer, d): # have to do this the generator
		if mutated_kmer not in unique_mutated_kmers:
			increment_count(reverseComplement(mutated_kmer))
			increment_count(mutated_kmer)

		unique_mutated_kmers[mutated_kmer] = True


highest_freq_kmers = []

for key, val in counts.iteritems():
	if val == highest_kmer_count:
		highest_freq_kmers.append( key )

print " ".join(highest_freq_kmers)		
