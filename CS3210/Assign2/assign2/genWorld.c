#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    double livePercent;
    int N, i, j;
    FILE* outf;

    if (argc < 4){
        printf("%s <world size> <percentage of live> <output file>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    livePercent = atoi(argv[2]) / (double) 100;

    printf("Generating a %d x %d world with %.3lf live cells\n",
            N, N, livePercent);


    outf = fopen(argv[3], "w");
    fprintf(outf, "%d\n", N);

        
    srand48(time(NULL));
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++){
            if (drand48() < livePercent){
                fprintf(outf, "X");
            } else {
                fprintf(outf, "O");
            }
        }
        fprintf(outf, "\n");
    }
    
    fclose(outf);
    

    return 0;
}
