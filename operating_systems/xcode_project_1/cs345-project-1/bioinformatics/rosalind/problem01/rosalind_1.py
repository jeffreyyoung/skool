import sys

with open( sys.argv[ 1 ] ) as fh:
	seq = fh.next().strip()
	k = int( fh.next().strip() )

counts = dict()
highest_count = 0
most_common_kmer = ""
for i in range( len( seq ) ):
	kmer = seq[ i : i + k ]
	if kmer in counts:
		counts[ kmer ] += 1
	else:
		counts [ kmer ] = 1

	if counts[ kmer ] > highest_count:
		highest_count = counts[ kmer ]
		most_common_kmer = kmer
		
highest_freq_kmers = []

for key, val in counts.iteritems():
	if val == highest_count:
		highest_freq_kmers.append( key )

print " ".join(sorted(  highest_freq_kmers ) )
