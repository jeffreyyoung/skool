import sys

class Contigs:
    def get(self):
        return self.result

    def __init__(self, kmers):
        self.g = dict()
        self.counts = dict()
        self.edge_count = 0
        self.kmers = kmers

        for kmer in self.kmers:
            #print kmer
            left = kmer[ : -1 ]
            right = kmer[ 1 : ]
            self.edge_count += 1

            if left in self.g:
                self.g[ left ].append( right )
            else:
                self.g[ left ] = [ right ]

            if left in self.counts:
                self.counts[ left ][ 1 ] += 1
            else:
                self.counts[ left ] = [ 0, 1 ]

            if right in self.counts:
                self.counts[ right ][ 0 ] += 1
            else:
                self.counts[ right ] =[ 1, 0 ]

        non_branching = set()
        contigs = []

        def build_graph( start):
            self.edge_count
            path = [ start ]
            cur_node = start

            while len( cur_node ) > 0:
                next_node = self.g[ cur_node ][ 0 ]
                del self.g[ cur_node ][ 0 ]
                #if len( g[ cur_node ] ) == 0:
                #    del g[ cur_node ]

                self.edge_count -= 1

                #print cur_node
                if next_node in non_branching:
                    #print "non branching:", cur_node
                    path.append( next_node )
                    cur_node = next_node
                    continue
                else:
                    path.append( next_node )
                    break

            return path

        #print g
        #print counts

        def merge_nodes( nodes ):
            contig = nodes[ 0 ]
            for i in range( 1, len( nodes ) ):
                contig += nodes[ i ][ -1 ]
            return contig

        def has_outgoing( node ):
            if len( self.g[ node ] ) > 0:
                return True
            else:
                return False

        for key, item in self.counts.iteritems():
            if item[ 0 ] == 1 and item[ 1 ] == 1:
                non_branching.add( key )

        #print non_branching

        start = self.g.keys()[ 0 ]
        start = 'TG'
        #while( edge_count > 0 ):
        #    print edge_count
        while self.edge_count > 0:
            for i in self.g.keys():
                if i in non_branching or len( self.g[ i ] ) == 0:
                    continue
                start = i
                break
            #print "starting with:", start
            c = build_graph( start )
            contigs.append( c )

        self.result = [merge_nodes(contig) for contig in contigs]
