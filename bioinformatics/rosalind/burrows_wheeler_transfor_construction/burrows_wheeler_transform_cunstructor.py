def burrow_wheeler_transfor(text):
    rotations = []
    for i in range( len( text) ):
        rotations.append( text[i:] + text[:i] )
    rotations.sort()
    bwt = "";
    for r in rotations:
        bwt += r[-1]
    return bwt;

if __name__ == "__main__":
    import sys
    with open( sys.argv[ 1 ] ) as fh:
    	text = next(fh).strip()

    print burrow_wheeler_transfor( text ) 
