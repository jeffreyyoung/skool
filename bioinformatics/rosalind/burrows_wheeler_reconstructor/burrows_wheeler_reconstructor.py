def burrow_wheeler_reconstructor(text):
    TEXT_SIZE = len( text )
    rows = [''] * TEXT_SIZE
    for i in range( TEXT_SIZE ):
        for j in range( TEXT_SIZE ):
            rows[j] = text[j] + rows[j]
        rows.sort()
    for t in rows:
        if t[-1] == '$':
            return t;

if __name__ == "__main__":
    import sys
    with open( sys.argv[ 1 ] ) as fh:
    	text = next(fh).strip()

    print burrow_wheeler_reconstructor( text )
