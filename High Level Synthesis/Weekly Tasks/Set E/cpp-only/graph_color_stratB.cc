#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <ac_int.h>

static const int N = 6;
static const int cnt_bit = (log2(N) - (int)log2(N)) ? log2(N)+1 : log2(N);

typedef ac_int<N,false> row_array;
typedef ac_int<cnt_bit,false> count_int;
typedef row_array adj_matrix[N];

void graph_color(adj_matrix Adj_G, count_int &colors_needed){
	colors_needed = 0;
	count_int col_cntr = 0;
	count_int color_array[N];
	bool flag = false;
	bool finish_flag = false;
	
	color_array_init: for (count_int i=0; i<N; i++) color_array[i] = 0;
	count_int edges_checked;
	count_int current_row = 0;
	count_int neighbours_to_visit_cnt = 0;
	row_array neigbhours_to_visit=0;
	
	while (!finish_flag){
		count_int clr = 1;
		row_array current_row_array = Adj_G[current_row];
		row_array neighbours_with_color_checked = 0;
		
		check_neigh_loop:
		for (short int col=0; col<N; col++) if (current_row_array[col]) if (color_array[col]) neighbours_with_color_checked[color_array[col]-1]=1;
		
		bool color_flag = true;
		
		select_color_loop:
		for (short int i=0; i<N; i++){
			if (color_flag and !neighbours_with_color_checked[i]){
				color_flag = false;
				clr = i+1;
			}
		}
		
		color_array[current_row] = clr;
		if (clr>colors_needed) colors_needed=clr;
		
		edges_checked+=1;
		
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
		
		if (edges_checked == N) finish_flag=true;	
	}
	
}

int main() {
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
	  count_int clr_needed;
	  
	  graph_color(test_matrix[test], clr_needed);
	  
	  std::cout<<"Graph "<< test+1 <<" | Minimum Colors: "<<min_color_num[test]
	  <<" | Colors Computed: "<< clr_needed<<"\n";
	}
  return 0;
}

