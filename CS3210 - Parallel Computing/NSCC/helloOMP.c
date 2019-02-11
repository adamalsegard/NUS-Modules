#include <stdio.h>

int main()
{

    #pragma omp parallel num_threads(4)
    {
        printf("Hello World!\n");
    }

    printf("End of program\n");

    return 0;
}
