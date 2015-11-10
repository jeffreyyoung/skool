
import sys#depth first
from SuffixTree import SuffixTree, Node

def get_suffix_array(tree):
    suffix_array = []
    def build_suffix_array(root, depth):
        edges = root.edges.items()
        edges.sort()
        for e in edges:
            n = tree.nodes[ e[1] ]
            child_depth = depth + n.end - n.start
            if len(n.edges) == 0:
                suffix_array.append(n.start - depth)

            build_suffix_array(n, child_depth)

    build_suffix_array(tree.nodes[ tree.root ], 0)

    return suffix_array

if __name__ == "__main__":
    with open(sys.argv[1]) as fh:
        text = next(fh).strip()

    tree = SuffixTree( len( text ) )
    for char in text:
        tree.add_char( char )

    print ", ".join( [str(x) for x in get_suffix_array(tree) ])
