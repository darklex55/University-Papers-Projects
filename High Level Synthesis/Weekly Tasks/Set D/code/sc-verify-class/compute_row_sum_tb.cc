#include <iostream>
#include <cstdlib>
#include <ctime>

#include "compute_row_sum.h"

//static const int N = 5;
//static const int M = 3;

static const int RUNS = 10;

//#pragma hls_design top
//void CCS_BLOCK(compute_row_sum)(short a[N][M], short row_sum[N]) {
//  
//  ROWS:for (int i=0; i < N; i++) {
//    row_sum[i] = 0;
//    COLS:for (int j=0; j < M; j++) {
//      row_sum[i] += a[i][j];
//    }
//  }
//}

CCS_MAIN(int argc, char* argv[]) {

  short x[N][M];
  short row_sum_of_x[N];
 
  // reseed RNG
  std::srand(std::time(NULL));

  ComputeRowSum rs;
 
  // run RUNS tests
  for (int k=0; k < RUNS; k++) {
    std::cout << "Trial " << k << "\n";

    // fill input matrix with random values
    // for simplicity keep values below 20  
    for (int i=0; i < N; i++) {
      for (int j=0; j < M; j++) {
        x[i][j] = std::rand() % 20;
        std::cout << x[i][j] << "\t";
      }
      std::cout << "\n";
    }

    // call DUT
    rs.compute_row_sum(x, row_sum_of_x);

    // print out the computed sums of each row
    std::cout << "Row Sum = ";
    for (int i=0; i < N; i++) {
      std::cout << row_sum_of_x[i] << " ";
    }
    std::cout << "\n\n";
  }
  
  CCS_RETURN(0);
}

