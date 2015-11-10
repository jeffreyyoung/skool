import sys
from SuffixTree import SuffixTree, Node
from suffix_array import get_suffix_array


def get_partial_suffix_array(text, k):
    tree = SuffixTree( len( text ) )
    for char in text:
        tree.add_char( char )
    suffix_array = get_suffix_array( tree )
    partial_suffix_array = []
    for i in range( len(suffix_array) ):
        if suffix_array[i] % k == 0:
            partial_suffix_array.append( (i, suffix_array[i]) )
    return partial_suffix_array




if __name__ == "__main__":
    import sys
    with open( sys.argv[ 1 ] ) as fh:
    	text = next(fh).strip()
        patterns = []
        for line in fh:
            patterns.append(line.strip())
    for x in get_partial_suffix_array( text, 5 ):
        print str(x[0]) + ", " + str(x[1])
