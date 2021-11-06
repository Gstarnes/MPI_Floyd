//
//  graph.h
//  Prog02
//
//  Created by GS on 11/1/21.
//

#ifndef graph_h
#define graph_h

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NO_EDGE -1
#define INFINITY 99999999

int min(int a,int b);
int graph_size(const char* input_file_name);
void print_graph(int** graph, int dimension);
int** read_graph(const char* input_file_name);
void write_graph(int dimension, const char* output_file_name, int graph[dimension][dimension]);

void mpi_err(const int err, const char* operation);


#endif /* graph_h */
