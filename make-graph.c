#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "graph.h"

#define NO_EDGE -1

int main (int argc, char *argv[]) {
    int opt, dimension, max_edge_weight, probability_thresh, edge_weight;
    char* output_file_name;

    dimension = 10000; // dimensions of the nxn matrix to create
    probability_thresh = 150; // edge probability threshold
    max_edge_weight = 100; // max edge weight
    output_file_name = "default-graph.dat";

    srand(time(0));

    while((opt = getopt(argc, argv, "n:r:p:o:")) != -1)
    {
         switch(opt)
         {
            case 'n':dimension = atoi(optarg);
            break;

            case 'r':max_edge_weight = atoi(optarg);
            break;

            case 'p':probability_thresh = atoi(optarg);
            break;

            case 'o':output_file_name = strdup(optarg);
            break;
        }
    }

    int (*graph)[dimension] = malloc(dimension * sizeof *graph);

    for(int row = 0; row < dimension; row++)
    {
        for(int col = 0; col < dimension; col++)
        {
            edge_weight = rand() % (probability_thresh + 1);
            if(col == row)
            {
                graph[row][col] = 0;
            }
            else
            {
                graph[row][col] = rand() % (probability_thresh + 1) <= max_edge_weight ? edge_weight : -1;
            }
            //printf("%d\n", graph[row][col]);
        }
    }

    write_graph(dimension, output_file_name, graph);
    
    free(graph);
}
