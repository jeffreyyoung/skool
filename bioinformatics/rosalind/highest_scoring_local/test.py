import numpy
import sys
DELETION, INSERTION, MATCH = range(3)

def get_alignment_dict():
    with open("PAM250.txt") as fh:
            first_row = fh.readline().strip().split()
            lines = fh.readlines()
    value_dict = {}
    for line in lines:
            values = line.strip().split()
            line_dict = {}
            amino_acid = values[0]
            for i in range(1, len(values)):
                    line_dict[ first_row[i-1] ] = values[i]
            value_dict[amino_acid] = line_dict
    return value_dict

def smith_waterman(seq1, seq2, insertion_penalty = -5, deletion_penalty = -5):
    """
    Find the optimum local sequence alignment for the sequences `seq1`
    and `seq2` using the Smith-Waterman algorithm. Optional keyword
    arguments give the gap-scoring scheme:

    `insertion_penalty` penalty for an insertion (default: -1)
    `deletion_penalty`  penalty for a deletion (default: -1)
    `mismatch_penalty`  penalty for a mismatch (default: -1)
    `match_score`       score for a match (default: 2)

    See <http://en.wikipedia.org/wiki/Smith-Waterman_algorithm>.

    >>> for s in smith_waterman('AGCAGACT', 'ACACACTA'): print s
    ...
    AGCAGACT-
    A-CACACTA
    """
    m, n = len(seq1), len(seq2)
    score = get_alignment_dict()
    # Construct the similarity matrix in p[i][j], and remember how
    # we constructed it -- insertion, deletion or (mis)match -- in
    # q[i][j].
    p = numpy.zeros((m + 1, n + 1))
    q = numpy.zeros((m + 1, n + 1))
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            deletion = (p[i - 1][j] + deletion_penalty, DELETION)
            insertion = (p[i][j - 1] + insertion_penalty, INSERTION)
            match = (p[i -1][j - 1] + int(score[seq1[i-1]][seq2[j-1]]) , MATCH)

            p[i][j], q[i][j] = max((0, 0), deletion, insertion, match)

    # Yield the aligned sequences one character at a time in reverse
    # order.
    def backtrack():
        i, j = m, n
        while i > 0 or j > 0:
            if not (i >= 0 and j >= 0):
                break
                
            if q[i][j] == MATCH:
                i -= 1
                j -= 1
                yield seq1[i], seq2[j]
            elif q[i][j] == INSERTION:
                j -= 1
                yield '-', seq2[j]
            elif q[i][j] == DELETION:
                i -= 1
                yield seq1[i], '-'
            else:
                break

    return [''.join(reversed(s)) for s in zip(*backtrack())]

if __name__ == "__main__":
    with open( sys.argv[ 1 ] ) as fh:
    	str1 = next(fh).strip()
        str2 = next(fh).strip()
    print smith_waterman(str1, str2)
