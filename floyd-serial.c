#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <string.h>
#include "graph.h"

int main (int argc, char *argv[]) {
    
    clock_t start = clock();

    int opt, dimension;
    char* input_file_name;
    char* output_file_name;

    dimension = 10; // dimensions of the nxn matrix to create
    input_file_name = "default-graph.dat";
    output_file_name = "default-floyds-graph.dat";

    while((opt = getopt(argc, argv, "i:o:")) != -1)
    {
         switch(opt)
         {
            case 'i':input_file_name = strdup(optarg);
            break;

            case 'o':output_file_name = strdup(optarg);
            break;
        }
    }
    

    int (*graph)[dimension] = read_graph(input_file_name);

    printf("floyd start\n");
    
    clock_t floyd_start = clock();
    
    int temp;

    for(int old_row = 0; old_row < dimension; old_row++)
    {
        for(int row = 0; row < dimension; row++)
        {
            for(int col = 0; col < dimension; col++)
            {
                temp = min(graph[row][col] , graph[row][old_row] + graph[old_row][col]);
                graph[row][col] = temp;
                
//                printf("%d ",graph[row][col]);
            }
//            printf("\n");
        }
//        printf("\n");
    }
    
    clock_t floyd_stop = clock();

    printf("floyd finish\n");

    write_graph(dimension, output_file_name, graph);
    
    free(graph);
    
    clock_t end = clock();
    
    printf("floyd time %f\n",(double)(floyd_stop-floyd_start)/CLOCKS_PER_SEC);
    printf("total time %f\n", (double)(end-start)/ CLOCKS_PER_SEC);
}