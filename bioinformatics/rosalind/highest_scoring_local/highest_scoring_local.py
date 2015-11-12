import sys
INDEL_PENALTY = 5
LEFT_TYPE = 0
TOP_TYPE = 1
DIAGANOL_TYPE = 2
TAXI_RIDE = 3
class Node:
    def __init__(self, value, direction):
        self.value = value
        self.direction = direction
def print_table(table):
    i = 0
    for j in range( len(table) ):
        string = ''
        row = table[j]
        for n in range( len(table[j]) ):
            if table[j][n] is not None:
                string += str(table[j][n].value)
            else:
                string += '_'
            #string += str( table[n][j].value || 'None' )
            string += " "
        #print "row: " + str(i)
        print string
        #i += 1
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

def highest_local_alignmnet(str1, str2):
    def get_result_strings(table):
        i = len(str1)
        j = len(str2)
        top_string = ''
        left_string = ''
        n = table[i][j]
        finished = False
        while not finished:
            if n.direction is LEFT_TYPE:
                top_string = '-' + top_string
                left_string = str2[j-1] + left_string
                j -= 1;
            elif n.direction is TOP_TYPE:
                top_string = str1[i-1] + top_string
                left_string = '-' + left_string
                i-=1;
            elif n.direction is DIAGANOL_TYPE:
                top_string = str1[i-1] + top_string
                left_string = str2[j-1] + left_string
                i -= 1
                j -= 1
            else:
                break;
            n = table[i][j]
            finished = (i is 0 and j is 0)

        return (left_string, top_string)

    def get_table():
        pam_score = get_alignment_dict()
        table_width = len(str1) + 1
        table_height = len(str2) + 1

        a = [  [None for x in range(table_height)] for x in range(table_width)   ]
        for i in a:
            i[0] = Node(0,4)
        for i in range( len( a[0] ) ):
            a[0][i] = Node(0,4)

        for i in range(1, table_width):
            for j in range(1, table_height):
                values = [None, None, None, 0]
                values[DIAGANOL_TYPE] = a[i - 1][j - 1].value + int( pam_score[str1[i - 1]][str2[j - 1]] )
                values[LEFT_TYPE] = a[i][j-1].value - INDEL_PENALTY
                values[TOP_TYPE] = a[i - 1][j].value - INDEL_PENALTY
                n = Node(-999999999, 9)
                for k in range( len(values) ):
                    if values[k] is not None and values[k] > n.value:
                        n = Node(values[k], k)
                a[i][j] = n
        return a
    table = get_table()
    #print_table( table )
    result = table[len(str1)][len(str2)].value
    strs = get_result_strings( table )
    return (str(result), strs[1], strs[0])


if __name__ == "__main__":
    with open( sys.argv[ 1 ] ) as fh:
    	str1 = next(fh).strip()
        str2 = next(fh).strip()
    print "\n".join(highest_local_alignmnet(str1, str2))
