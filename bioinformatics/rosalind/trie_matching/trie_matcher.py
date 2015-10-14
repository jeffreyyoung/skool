from Trie import Trie;

class PrefixNotFoundException(Exception):
    def __init__(self):
        self.value = "Prefix not found..."
    def __str__(self):
        return repr(self.value)

def prefix_trie_matching(text, trie):
    i = 0;
    symbol = text[i]
    v = trie.root
    pattern = "";
    for i in range(len(text)):
        symbol = text[i];
        pattern += symbol;
        if v.leaf:
            return pattern
        elif symbol in v.children:
            v = v.children[symbol]
        else:
            raise PrefixNotFoundException()


def trie_matching(text, trie):
    indexes = []
    for i in range(len(text)):
        try:
            prefix_trie_matching(text[i:], trie)
            indexes.append(i)
        except PrefixNotFoundException as e:
            pass
    return indexes

if __name__ == "__main__":
    import sys
    with open( sys.argv[ 1 ] ) as fh:
    	text = next(fh).strip()
    	patterns = []
    	for line in fh:
    		patterns.append(line.strip());

    t = Trie();
    for p in patterns:
        t.add(p);

    print " ".join(str(i) for i in trie_matching(text, t));
