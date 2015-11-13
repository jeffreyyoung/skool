import sys
INDEL_PENALTY = 5
LEFT_TYPE = 0
TOP_TYPE = 1
DIAGANOL_TYPE = 2
TAXI_RIDE = 3

def print_table(table):
    for row in table:
        print row

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

def highest_local_alignment(str1, str2):
    pam_score = get_alignment_dict()
    table_width = len(str1) + 1
    table_height = len(str2) + 1

    a = [  [0 for x in range(table_height)] for y in range(table_width)   ]
    p = [  [4 for x in range(table_height)] for y in range(table_width) ]

    for i in range(1, table_width):
        for j in range(1, table_height):
            values = [None, None, None, 0]
            values[DIAGANOL_TYPE] = a[i - 1][j - 1] + int( pam_score[str1[i - 1]][str2[j - 1]] )
            values[LEFT_TYPE] = a[i][j-1] - INDEL_PENALTY
            values[TOP_TYPE] = a[i - 1][j] - INDEL_PENALTY
            v = 0;
            pointer_type = 4;
            for k in range( len(values) ):
                if values[k] is not None and values[k] > v:
                    v = values[k]
                    pointer_type = k;
            a[i][j] = v
            p[i][j] = pointer_type

    #find highest point
    highest_point = (0,0)
    for i in range(table_width):
        for j in range(table_height):
            if a[i][j] > a[highest_point[0]][highest_point[1]]:
                highest_point = (i, j)

    i = highest_point[0]
    j = highest_point[1]

    score = a[i][j]

    top_string = ''
    left_string = ''
    while 1:
        if a[i][j] is 0:
            break
        if p[i][j] is LEFT_TYPE:
            top_string = '-' + top_string
            left_string = str2[j-1] + left_string
            j -= 1;
        elif p[i][j] is TOP_TYPE:
            top_string = str1[i-1] + top_string
            left_string = '-' + left_string
            i-=1;
        elif p[i][j] is DIAGANOL_TYPE:
            top_string = str1[i-1] + top_string
            left_string = str2[j-1] + left_string
            i -= 1
            j -= 1

    return (str(score), left_string, top_string)




if __name__ == "__main__":
    with open( sys.argv[ 1 ] ) as fh:
    	str1 = next(fh).strip()
        str2 = next(fh).strip()
    print "\n".join(highest_local_alignment(str2, str1))
