#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const int TAG_ARRAY = 0;
static const int TAG_LAST = 1;
static const int SLICER = 30000  ;
//static const char FOUND = 't'; 

int main(int argc, char *argv[])
{   
  int myRank;
  int numProc;

  double startTime = 0.0;
  double endTime = 0.0;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);   
  MPI_Comm_size(MPI_COMM_WORLD, &numProc);  

  if (argc < 2){
    if (myRank == 0){
      printf("Invalid arguments, use ./main n\n");
    }
      
    MPI_Finalize();
    return 0;
  }
  
  startTime = MPI_Wtime();  
  
  long n = atol(argv[1]); 
  unsigned long matrix = n * n; 

  unsigned long matrixH = ((matrix - n) / 2) + n; 

  unsigned long coreW[numProc];  
  for (int i = 0; i < numProc; i++){
    coreW[i] = floor((float) matrixH / numProc);
  
    if (i < matrixH % numProc){
      coreW[i] += 1;
    } 
  }

  MPI_Barrier(MPI_COMM_WORLD);

  unsigned long endp = 0;  
  unsigned long counter = 1;
  
  for (int i = 0; i < myRank; i++){
    endp += coreW[i]; 
  }
  

  unsigned long i = 1;
  unsigned long j = 1; 

  while (counter <= endp){
    counter++;
    i++;
    if (i == (n + 1)){
        j++;
        i = j;
    }
  }
  

  unsigned long section = coreW[myRank];  
  unsigned char *productArray = (unsigned char*) calloc(matrix + 1, sizeof(unsigned char));  
  
  if (productArray == NULL){
      printf("Failed to allocate product array\n");
  }

  unsigned long product;
  counter = 0;
  while (counter < section){
    product = i * j;  
    
    
    if (product > matrix) {
	
		product = matrix;
    }
    
    productArray[product] = 't';  
    
    i++;
    counter++;
    if (i == (n + 1)){	
      j++;
      i = j;
    }
  }

	

	unsigned long temp = matrix + 1;	

	unsigned char* slice;
	i = 0;
	j = temp % SLICER;	
	unsigned long k = temp / SLICER;	

	if (myRank != 0) { // Non-root processes
		while (k > 0) {
			slice = &productArray[i];
			MPI_Send(slice, SLICER, MPI_UNSIGNED_CHAR, 0, TAG_ARRAY, MPI_COMM_WORLD);
			i += SLICER;
			k--;
		}
		slice = &productArray[i];
		MPI_Send(slice, j, MPI_UNSIGNED_CHAR, 0, TAG_LAST, MPI_COMM_WORLD); // Send the remaining slice
		free(productArray);
	}

	if (myRank == 0) { // Root process

		unsigned char *recvArray = (unsigned char*) calloc(matrix + 1, sizeof(unsigned char));
		if (recvArray == NULL) {
			printf("Failed to allocate recvArray\n");
		}
		for (int rank = 1; rank < numProc; rank++) {

			i = 0;
			k = temp / SLICER;
			while (k > 0) {
				MPI_Recv(recvArray, SLICER, MPI_UNSIGNED_CHAR, rank, TAG_ARRAY, MPI_COMM_WORLD, NULL);
				for (counter = 0; counter < SLICER; counter++) {
					if (recvArray[counter]) { // No effect if productArray[counter] already == 1
						productArray[i] = 't';
					}
					i++;
				}
				k--;
			}
			MPI_Recv(recvArray, j, MPI_UNSIGNED_CHAR, rank, TAG_LAST, MPI_COMM_WORLD, NULL); // Receive the remaining slice
			for (counter = 0; counter < j; counter++) {
				if (recvArray[counter]) { // No effect if productArray[counter] already == 1
					productArray[i] = 't';
				}
				i++;
			}
		}

		unsigned long totalCount = 0;
		for (counter = 1; counter <= matrix; counter++) {
			if (productArray[counter]) { // Count the total unique numbers
				totalCount++;
			}
		}

		free(recvArray);
		free(productArray);
		printf("Number of Cores: %d\n", numProc);
		printf("Number (N): %lu\n", n);
		printf("Total no. of different elements: %lu\n", totalCount);
		endTime = MPI_Wtime();
		printf("Time elapsed: %.9lf seconds\n", endTime - startTime);
		printf("-------------------------------------------------------------\n" );
	  }
    MPI_Finalize();
    return 0;
}