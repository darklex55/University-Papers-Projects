#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

#include <ac_int.h>
#include <ac_channel.h>

typedef ac_int<16,false> Short;
static const int N = 5;
static const int M = 3;

void compute_row_sum (Short a[N][M], Short row_sum[N]){
	for (int i=0; i < N; i++){
		row_sum[i]=0;
		for (int j=0; j < M; j++){
			row_sum[i] += a[i][j];
		}
	}
}

int main(){

}
