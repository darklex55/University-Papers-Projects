#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ac_int.h>

#include "mc_scverify.h"

static const int N = 6;

typedef ac_int<N,false> row_array;
typedef row_array adj_matrix[N];

#pragma hls_design top
void CCS_BLOCK(graph_color)(adj_matrix Adj_G, short int &colors_needed){
	colors_needed = 0;
	short int col_cntr = 0;
	short int color_array[N];
	
	color_array_init: for (int i=0; i<N; i++) color_array[i] = 0;
	
	row_loop: for (int row=0;row<N;row++){
		short int clr = 1;
		row_array current_row =  Adj_G[row];
		row_array neighbours_with_color = 0;
		
		check_neigh_loop: 
		for (int col=0; col<N; col++) if (current_row[col]) if (color_array[col]) 
		neighbours_with_color[color_array[col]-1]=1;
		
		bool flag = true;
		
		select_color_loop:for (int i=0; i<N; i++){
			if (flag and !neighbours_with_color[i]){
				flag = false;
				clr = i+1;
			}
		}
		color_array[row] = clr;
		if (clr>colors_needed) colors_needed=clr;
	}
}

CCS_MAIN(int argc, char* argv[]) {
  adj_matrix test_matrix[3];
  test_matrix[0][0]=0b111010;
  test_matrix[0][1]=0b010101;
  test_matrix[0][2]=0b011010;
  test_matrix[0][3]=0b010101;
  test_matrix[0][4]=0b101111;
  test_matrix[0][5]=0b010001;
  
  test_matrix[1][0] = 0b010010;
  test_matrix[1][1] = 0b100101;
  test_matrix[1][2] = 0b011010;
  test_matrix[1][3] = 0b010100;
  test_matrix[1][4] = 0b101101;
  test_matrix[1][5] = 0b010010;
  
  test_matrix[2][0] = 0b000010;
  test_matrix[2][1] = 0b111001;
  test_matrix[2][2] = 0b111000;
  test_matrix[2][3] = 0b000110;
  test_matrix[2][4] = 0b000110;
  test_matrix[2][5] = 0b000110;
  
  int min_color_num[3] = {3,3,2};
  
  for (int test=0;test<3;test++){
	  short int clr_needed;
	  
	  graph_color(test_matrix[test], clr_needed);
	  
	  std::cout<<"Graph "<< test <<" | Minimum Colors: "<<min_color_num[test]
	  <<" | Colors Computed: "<< clr_needed<<"\n";
	}
  CCS_RETURN(0);
}

