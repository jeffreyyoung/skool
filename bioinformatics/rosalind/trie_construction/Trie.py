class Node:
    def __init__(self, value_in=None):
        self.children = {}
        self.value = value_in

class Trie:
    def __init__(self):
        self.root = Node();
    def add(self, word):
        cur_node = self.root;
        for char in word:
            if char in cur_node.children:
                cur_node = cur_node.children[char];
            else:
                cur_node.children[char] = Node(char);
                cur_node = cur_node.children[char]
    def to_string(self):
        self.counter = 0;
        for base, child in self.root.children.iteritems():
            self.to_string_recurse(child, 0);
    def to_string_recurse(self, node, parent_count):
        self.counter += 1;
        parent = self.counter;
        print str(parent_count) + "->" + str(self.counter) + ":" + node.value;
        for base, child in node.children.iteritems():
            self.to_string_recurse(child, parent);


if __name__ == "__main__":
    import sys
    with open(sys.argv[1]) as fh:
        patterns = []
        for line in fh:
            patterns.append(line.strip())

    t = Trie();
    for p in patterns:
        t.add(p);
    t.to_string();
