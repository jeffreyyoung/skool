if __name__ == "__main__":
    import sys
    with open( sys.argv[ 1 ] ) as fh:
    	text = next(fh).strip()
    	patterns = []
    	for line in fh:
    		patterns.append(line.strip());

    indexes = []
    for p in patterns:
        i = text.index(p);
        print i;
        while i >= 0:
            i = text[i:].index(p)
            print i
            
