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
	RCkmer = reverseComplement(kmer)
	unique_mutated_kmers = set()

	for mutated_kmer in mutations(kmer, d): # have to do this the generator
		unique_mutated_kmers.add(mutated_kmer)
	
	for unique_kmer in unique_mutated_kmers:
		reverse = reverseComplement(unique_kmer)

		if reverse in counts:
			counts[reverse] += 1
		else:
			counts[reverse] = 1

		if unique_kmer in counts:
			counts[unique_kmer] += 1
		else:
			counts[unique_kmer] = 1

		if highest_kmer_count < counts[unique_kmer]:
			highest_kmer_count = counts[unique_kmer]
		#print str(counts[unique_kmer]) + ' ' + str(highest_kmer_count)


highest_freq_kmers = []

for key, val in counts.iteritems():
    if val == highest_kmer_count:
        highest_freq_kmers.append( key )

print " ".join(highest_freq_kmers) 

print " ".join(most_frequent_kmers)		
