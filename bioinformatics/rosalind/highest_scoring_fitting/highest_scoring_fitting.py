import sys
TOP_TYPE = 0
LEFT_TYPE = 1
DIAGANOL_TYPE = 2


def print_table(table):
    for row in table:
        print row

def highest_local_alignment(str1, str2):
    table_width = len(str1) + 1
    table_height = len(str2) + 1
    #score table
    a = [  [0 for x in range(table_height)] for y in range(table_width)   ]
    #pointer table
    p = [  [-1 for x in range(table_height)] for y in range(table_width) ]

    for i in range(1, table_width):
        for j in range(1, table_height):
            values = [None, None, None]
            if str1[i - 1] == str2[j - 1]:
                values[DIAGANOL_TYPE] = a[i - 1][j - 1] + 1
            else:
                values[DIAGANOL_TYPE] = a[i - 1][j - 1] - 1

            values[LEFT_TYPE] = a[i][j-1] - 1
            values[TOP_TYPE] = a[i - 1][j] - 1
            v = -999999;
            pointer_type = -1;
            for k in range( len(values) ):
                if values[k] is not None and values[k] > v:
                    v = values[k]
                    pointer_type = k;
            a[i][j] = v
            p[i][j] = pointer_type

    #find highest point
    # print_table( a )
    # print '\n\n'
    # print_table( p )
    # print '\n\n'
    highest_point = (0,0)
    highest_value_row = 0
    score = 0
    for t in range(len(a[0]), len( a ) ):
        if a[highest_value_row][table_height - 1] <= a[t][table_height - 1]:
            highest_value_row = t
            score = a[t][table_height - 1]

    i = highest_value_row
    j = table_height -1
    top_string = ''
    left_string = ''

    while True:
        if j is 0:
            break
        elif p[i][j] is TOP_TYPE:
            top_string = str1[i-1] + top_string
            left_string = '-' + left_string
            i-=1;
        elif p[i][j] is LEFT_TYPE:
            top_string = '-' + top_string
            left_string = str2[j-1] + left_string
            j -= 1;
        elif p[i][j] is DIAGANOL_TYPE:
            top_string = str1[i-1] + top_string
            left_string = str2[j-1] + left_string
            i -= 1
            j -= 1

    return (str(score), top_string, left_string)




if __name__ == "__main__":
    with open( sys.argv[ 1 ] ) as fh:
    	str1 = next(fh).strip()
        str2 = next(fh).strip()
    print "\n".join(highest_local_alignment( str1, str2 ))
