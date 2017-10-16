#include <zlib.h>
#include <stdio.h>
#include <stdint.h>

#include "kseq.h"
KSEQ_INIT(gzFile, gzread)

#define VERSION "0.0.1"

inline void print_seq(const kseq_t *kseq);

int main(int argc, char *argv[]){
    
    int W = 64, T = 20, c;
    float C = 2.0;

    while ((c = getopt(argc, argv, "c:w:t:h")) >= 0) {
        
        if (c == 'w') W = atoi(optarg);
        else if (c == 't') T = atoi(optarg);
        else if (c == 'c') C = atof(optarg);
        else if (c == 'h'){
        	fprintf(stderr, "Version: %s\n",  VERSION);
        	return 0;
        }

    }
    
    if (optind == argc || argc !=  optind + 1 ) {
        fprintf(stderr, "Usage: seqtk_sdust [-p %.1lf] [-w %d] [-t %d] <in.fa>\n", C, W, T);
        fprintf(stderr, "Options:\n");
        fprintf(stderr, "  -c FLOAT  coverage cutoff for different contigs/reads to filter, defalut [2.0]\n");
        fprintf(stderr, "  -w INT    window size W, defalut [64]\n");
        fprintf(stderr, "  -t INT    score threshold T, defalut [20]\n");
        fprintf(stderr, "  -v        print version number\n");
        return 1;    }

    gzFile fp;
    fp = strcmp(argv[optind], "-")? gzopen(argv[optind], "r") : gzdopen(fileno(stdin), "r");

    if(fp){
        
        kseq_t *kseq;
        kseq  = kseq_init(fp);
        while (kseq_read(kseq) >= 0) {
            uint64_t *r;
            int i, n;
            
            r = (uint64_t*) sdust(0, (uint8_t*)kseq->seq.s, -1, T, W, &n);
            
            if( n == 0 ) { print_seq(kseq); continue;}

            int sum = 0;
            for (i = 0; i < n; ++i){
                
                int location[2] = {(int)(r[i]>>32), (int)r[i]};
                sum += location[1] - location[0];
                
                int j;
                for (j = location[0]; j < location[1]; ++j) kseq->seq.s[j] = 'N';
                if( (float)sum / kseq->seq.l < C ) print_seq(kseq);
               
            }
            

            free(r);
        }
        kseq_destroy(kseq);
        gzclose(fp);

    }
    return 0;
}

inline void print_seq(const kseq_t *kseq){

    printf(">%s", kseq->name.s);
    if (kseq->comment.l) printf(" %s", kseq->comment.s);
    printf("\n%s\n", kseq->seq.s); 

}
