#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "graph.h"

int main (int argc, char *argv[]) {
    int opt, dimension, read_error;
    char* input_file_name;
    FILE* file_p = NULL;

    //dimension = 10; // dimensions of the nxn matrix to create
    input_file_name = "default-graph.dat";

    srand(time(0));

    while((opt = getopt(argc, argv, "i:")) != -1)
    {
         switch(opt)
         {
            case 'i':input_file_name = strdup(optarg);
            break;
        }
    }
    
    dimension = graph_size(input_file_name);
    int (*graph)[dimension] = read_graph(input_file_name);
 
    for(int row = 0; row < dimension + 1; row++)
    {
        if(row == 0 || row == 1)
        {
            printf("   |");
            if(row == 0)
            {
                for(int col = 1; col <= dimension; col++)
                {
                    printf("%5d", col);
                }
            }
            if(row == 1)
            {
                for(int i = 0; i < 5 * dimension; i++)
                {
                    printf("-");
                }
                printf("\n");
                printf("%-3d|", row);
                for(int col = 0; col < dimension; col++)
                {
                    printf("%5d", graph[row - 1][col]);
                }
            }
        }
        else
        {
            printf("%-3d|", row);
            for(int col = 0; col < dimension; col++)
            {
                printf("%5d", graph[row - 1][col]);
            }
        }

        printf("\n");
    }
}
