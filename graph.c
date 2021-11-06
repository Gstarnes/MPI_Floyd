//
//  graph.c
//  Prog02
//
//  Created by GS on 11/1/21.
//

#include "graph.h"


int min(int a,int b)
{
    if(a == NO_EDGE && b == NO_EDGE)
    {
        return INFINITY;
    }
    else if(a == NO_EDGE && !(b == NO_EDGE))
    {
        return b;
    }
    else if(!(a == NO_EDGE) && b == NO_EDGE)
    {
        return a;
    }
    else
    {
        if(a <= b)
        {
            return a;
        }
    }

    return b;
}

int** read_graph(const char* input_file_name)
{
    int dimension = 0;
    unsigned long read_error = 0;
    FILE* file_p = NULL;
    
    
    file_p = fopen(input_file_name, "r");
    
    if(file_p == NULL)
    {
        printf("Could not open file\n");
        exit(EXIT_FAILURE);
    }

    read_error = fread(&dimension, sizeof(int), 1, file_p);
    
    if(read_error != 1)
    {
        printf("Could not read all values\n");
        printf("%lu out of %d read\n",read_error,dimension);
        exit(EXIT_FAILURE);
    }

    printf("read in graph\n");

    int (*graph)[dimension] = malloc(dimension * sizeof *graph);

    for(int row = 0; row < dimension; row++)
    {
        for(int col = 0; col < dimension; col++)
        {
            read_error += fread(&graph[row][col], sizeof(int), 1, file_p);
        }
    }
    
    if(read_error != dimension * dimension + 1)
    {
        printf("Could not read all values\n");
        printf("%lu out of %d read\n",read_error,dimension * dimension + 1);
        exit(EXIT_FAILURE);
    }

    printf("graph created\n");

    fclose(file_p);
    
    return graph;
}

void write_graph(int dimension, const char* output_file_name, int graph[dimension][dimension])
{
    unsigned long write_error;
    FILE* file_p = NULL;
    
    file_p = fopen(output_file_name, "w");

    printf("write out file\n");

    write_error = fwrite(&dimension, sizeof(int), 1, file_p);
    
    if(write_error != 1)
    {
        printf("Could not write all values\n");
        printf("%lu out of %d written\n",write_error,dimension);
        exit(EXIT_FAILURE);
    }
    
    for(int row = 0; row < dimension; row++)
    {
        for(int col = 0; col < dimension; col++)
        {
        //printf("%d\n",graph[row][1]);
            write_error += fwrite(&graph[row][col], sizeof(int), 1, file_p);

        }
    }
    
    if(write_error != dimension * dimension + 1)
    {
        printf("Could not write all values\n");
        printf("%lu out of %d written\n",write_error,dimension * dimension + 1);
        exit(EXIT_FAILURE);
    }
    
//    for(int n = 0; n < tI; n++)
//    {
//      fwrite(heat[n], sizeof(float),sections, ofp);
//    }

    printf("finish");

    fclose(file_p);
}

int graph_size(const char* input_file_name)
{
    int dimension = 0;
    unsigned long read_error = 0;
    FILE* file_p = NULL;
    
    
    file_p = fopen(input_file_name, "r");
    
    if(file_p == NULL)
    {
        printf("Could not open file\n");
        exit(EXIT_FAILURE);
    }

    read_error = fread(&dimension, sizeof(int), 1, file_p);
    
    if(read_error != 1)
    {
        printf("Could not read all values\n");
        printf("%lu out of %d read\n",read_error,dimension);
        exit(EXIT_FAILURE);
    }

    return dimension;
}

void mpi_err(int err, const char* operation)
{
    if(err != MPI_SUCCESS)
    {
        printf("MPI Failed %s\n", operation);
        exit(EXIT_FAILURE);
    }
}

void print_graph (int** graph, int dimension)
{
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