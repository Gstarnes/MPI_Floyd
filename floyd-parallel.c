#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "graph.h"
#include <math.h>
#include "graph_checkerboard_io.h"
#include <mpi.h>

int main (int argc, char *argv[]) {
        
    clock_t time_start = clock();

    int opt, dimension, temp_path, mpi_op_status;
    int rank, size;
    int col_rank, col_size;
    int row_rank, row_size;
    int cart_rank, cart_size;
    const int keep_col[2] = {1,0};
    const int keep_row[2] = {0,1};
    const int periods[2] = {0, 0};
    const int dims[2];
    char* input_file_name;
    char* output_file_name;
    MPI_Status status;
    MPI_Comm cartesian;
    MPI_Comm row_comm;
    MPI_Comm col_comm;

    input_file_name = "default-graph.dat";
    output_file_name = "default-floyds-graph.dat";

   // debug( "start\n" )

    mpi_op_status = MPI_Init( &argc, &argv);
    mpi_err(mpi_op_status, "MPI_INIT");

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

    MPI_Comm_rank (MPI_COMM_WORLD, &rank); 
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    mpi_op_status = MPI_Dims_create(size, 2, dims);
    mpi_err(mpi_op_status, "MPI_create_dims");

    mpi_op_status = MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cartesian);
    MPI_Comm_rank (cartesian, &cart_rank); 
    MPI_Comm_size (cartesian, &cart_size);
    mpi_err(mpi_op_status, "MPI_Cart_create");
    debug("%d cartesian comm created\n",rank);

    mpi_op_status = MPI_Cart_sub(cartesian, keep_col, &col_comm);
    mpi_err(mpi_op_status, "MPI_Cart_sub2");
    MPI_Comm_rank (col_comm, &col_rank); 
    MPI_Comm_size (col_comm, &col_size);
    debug("%d col comm created\n", rank);

    mpi_op_status = MPI_Cart_sub(cartesian, keep_row, &row_comm);
    mpi_err(mpi_op_status, "MPI_Cart_sub2");
    MPI_Comm_rank (row_comm, &row_rank); 
    MPI_Comm_size (row_comm, &row_size);

    debug("%d: row comm created\n", rank);

    int** graph = NULL;
    int* graph_elements = NULL;
    dimension = 0; // initialize dimension
    int local_size = BLOCK_SIZE(cart_rank, cart_size, dimension);

    debug("%d: %s, %d dimension\n", rank, input_file_name, dimension);

    read_checkerboard_graph (input_file_name, (void***) &graph, (void**) &graph_elements, MPI_INT, &dimension, cartesian);
    
    //print_graph(graph, local_size);
    
    debug("%d:  %d dimension\n", rank, dimension);

    int* temp_vector_row = (int *) malloc(dimension * sizeof(int));
    int* temp_vector_col = (int *) malloc(dimension * sizeof(int));
    int num_procs[2];
    int cords[2];
    MPI_Cart_get(cartesian, 2, num_procs, periods, cords);

    debug("%d: floyd start\n", rank);

    for(int old_row = 0; old_row < dimension; old_row++)
    {
        int owning_task[2];
        owning_task[0] = BLOCK_OWNER(cords[0] , num_procs[0], dimension);
        owning_task[1] = BLOCK_OWNER(cords[1] , num_procs[1], dimension);

        debug("%d: Owner Cords %d,%d\n", rank, cords[1], cords[1]);

        debug("%d: %d local block size\n", rank, local_size);
        
        if(owning_task[0] == cords[0 && owning_task[1] == cords[1]])
        {
            int offset_row = old_row - BLOCK_LOW(cords[0] , num_procs[0], dimension);
            int offset_col = old_row - BLOCK_LOW(cords[1] , num_procs[1], dimension);

            for(int col = 0; col < local_size; col++)
            {
                temp_vector_row[col] = graph[offset_row][col];
            }

            for(int row = 0; row < local_size; row++)
            {
                temp_vector_col[row] = graph[row][offset_col];
            }
        }

        debug("%d: start Bcast\n", rank);

        MPI_Bcast(temp_vector_col, dimension, MPI_INT, owning_task[0], row_comm);
        MPI_Bcast(temp_vector_row, dimension, MPI_INT, owning_task[1], col_comm);

        debug("%d: finish Bcast\n", rank);

        for(int row = 0; row < local_size; row++)
        {
            for(int col = 0; col < local_size; col++)
            {
                temp_path = min(graph[row][col] , temp_vector_row[col] + temp_vector_col[row]);
                graph[row][col] = temp_path;
            }
        }

        debug("%d: floyd iteration finished\n", rank);
    }


    debug("%d: write checkboard graph\n", rank);

    write_checkerboard_graph (output_file_name, (void***) &graph, (void**) &graph_elements, MPI_INT, dimension, cartesian);

    MPI_Finalize();

}