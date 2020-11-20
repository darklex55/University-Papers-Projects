#ifndef COMPUTE_ROW_SUM_H
#define COMPUTE_ROW_SUM_H

#include <iostream>
#include <cstdlib>
#include <ctime>

#include "mc_scverify.h"

static const int N = 5;
static const int M = 3;

#pragma hls_design top
class ComputeRowSum {
private:
public:
  ComputeRowSum(){};

#pragma hls_design interface
void CCS_BLOCK(compute_row_sum)(short a[N][M], short row_sum[N]) {
  
  ROWS:for (int i=0; i < N; i++) {
    row_sum[i] = 0;
    COLS:for (int j=0; j < M; j++) {
      row_sum[i] += a[i][j];
    }
  }
}

};

#endif


