### seqtk_sdust
  
  1. comile
 
  >gcc -O2   -D_NO_NT4_TBL   seqtk_sdust.c   sdust.c  -o  seqtk_sdust  -lz  kalloc.c
   
  2. interface
      
      $ seqtk_sdust
      Usage: seqtk_sdust [-p 2.0] [-w 64] [-t 20] <in.fa>
      Options:
        -c FLOAT  coverage cutoff for different contigs/reads to filter, defalut [2.0]
        -w INT    window size W, defalut [64]
        -t INT    score threshold T, defalut [20]
        -v        print version number

  
  source code sdust.h sdust.c from： https://github.com/lh3/minimap2
  
  3. example
  
  >seqtk_sdust  -c  0.9   -w 20  -t 20  test.fasta  >/dev/null
  
  Mask the low complexity regin in the fasta sequence and filter the sequence with -c option.
  

