#include <iostream>
#include <cstdlib>
#include <ctime>
#include <ac_int.h>

#include "mc_scverify.h"

static const short int N = 6;
static const short int cnt_bit = 3;

typedef ac_int<N,false> row_array;
typedef ac_int<cnt_bit,false> count_int;
typedef row_array adj_matrix[N];

#pragma hls_design top
void (CCS_BLOCK)(graph_color)(adj_matrix Adj_G, short int &colors_needed){
	colors_needed = 0;
	count_int col_cntr = 0;
	count_int clr;
	count_int color_array[N];
	color_array_init: for (count_int i=0; i<N; i++) color_array[i] = 0;
	count_int current_row = 0;
	row_array current_row_array;
	row_array neighbours_colors_found;
	
	count_int neighbours_to_visit_cnt = 0;
	row_array neigbhours_to_visit=0;
	row_array colors_found;
	
	bool color_flag;
	bool flag = false;
	
	for (short int run = 0; run<N; run++){
			check_neigh_loop:
			for (short int col=0; col<N; col++){
				if (!col){
					current_row_array = Adj_G[current_row];
					neighbours_colors_found = 0;
				}
				if (current_row_array[col]) if (color_array[col]) neighbours_colors_found[color_array[col]-1]=1;;
				
			}

			select_color_loop:for (int i=0; i<N; i++){
				if (!i) color_flag = true;
				if (color_flag and !neighbours_colors_found[i]){
					color_flag = false;
					clr = i+1;
				}
			}
					
			color_array[current_row] = clr;
			if (clr>colors_needed) colors_needed=clr;
			
			if (!flag){
				select_next_row_loop:
				for (short int i=0;i<N;i++){
					if (current_row_array[i] and !color_array[i]){
						neigbhours_to_visit[i]=1;
						neighbours_to_visit_cnt+=1;
						flag=true;
					}
				}
				bool next_flag = true;
				if(!flag){
					select_next_non_neighbour_row_loop:
					for (short int i=0;i<N;i++){
						if (next_flag and !color_array[i]){
							current_row = i;
							next_flag = false;
						}
					}
				}
			}
		
			if (flag){
				bool next_neigbour_flag = true;
				select_next_neighbour_as_row_loop:
				for (short int i=0;i<N;i++){
					if (next_neigbour_flag and neigbhours_to_visit[i]){
						current_row = i;
						neigbhours_to_visit[i] = 0;
						neighbours_to_visit_cnt-=1;
						next_neigbour_flag = false;
						if (!neighbours_to_visit_cnt) flag=false;
					}
				}
			}
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
  
  test_matrix[1][0] = 0b100110;
  test_matrix[1][1] = 0b111101;
  test_matrix[1][2] = 0b001011;
  test_matrix[1][3] = 0b110110;
  test_matrix[1][4] = 0b101010;
  test_matrix[1][5] = 0b011011;
  
  test_matrix[2][0] = 0b000010;
  test_matrix[2][1] = 0b111001;
  test_matrix[2][2] = 0b111000;
  test_matrix[2][3] = 0b000110;
  test_matrix[2][4] = 0b000110;
  test_matrix[2][5] = 0b000110;
  
  int min_color_num[3] = {3,4,2};
  
  for (int test=0;test<3;test++){
	  short int clr_needed;
	  
	  graph_color(test_matrix[test], clr_needed);
	  
	  std::cout<<"Graph "<< test+1 <<" | Minimum Colors: "<<min_color_num[test]
	  <<" | Colors Computed: "<< clr_needed<<"\n";
	}
  CCS_RETURN(0);
}

