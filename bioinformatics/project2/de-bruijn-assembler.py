import sys


def get_reads_from_fasta(fastaFileName):
    with open(fastaFileName) as fh:
        seq = ''
        reads = []
        for line in fh:
            if line[0] == '>':
                seq_header = line.strip()
            else:
                reads.append(line.strip())
        return reads

def get_seq_from_fasta(fastaFileName):
    with open(fastaFileName) as fh:
        seq = ''
        for line in fh:
            if line[0] == '>':
                seq_header = line.strip()
            else:
                seq += line.strip()
        return seq

print get_reads_from_fasta(sys.argv[1])
