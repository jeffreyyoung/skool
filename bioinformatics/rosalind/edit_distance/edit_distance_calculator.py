import sys

def inefficient_levenshtein_distance(str1, str2):
    #base case: empty strings
    if len( str1 ) is 0:
        return len( str2)
    elif len( str2 ) is 0:
        return len( str1 )

    cost = 0;
    if str1[-1] == str2[-1]:
        cost = 1
    else:
        cost = 0

    return min(
        inefficient_levenshtein_distance(str1[:-1], str2) + 1,
        inefficient_levenshtein_distance(str1, str1[:-1]) + 1,
        inefficient_levenshtein_distance(str1[:-1], str2[:-1]) + cost
    )

def levenshtein_distance(s, t):
    print "S: ", s
    m = len(s)
    n = len(t)
    d = [[0 for x in range(n)] for t in range(m)]

    for i in range(1, m):
        d[i][0] = i

    for j in range(1, n):
        d[0][j] = j

    for j in range(1, n):
        for i in range(1, m):
            if s[i] == t[j]:
                d[i][j] = d[i - 1][j - 1]
            else:
                d[i][j] = min(
                    d[i - 1, j] + 1, #deletion
                    d[i, j - 1] + 1, #insertion
                    d[i - 1][j - 1] + 1 #substitution
                )
    return d[m][n]

if __name__ == "__main__":
    with open( sys.argv[ 1 ] ) as fh:
    	str1 = next(fh).strip()
        str2 = next(fh).strip()
    print levenshtein_distance(str1, str2)
