#ifndef K_MEANS_CLUSTERING
#define K_MEANS_CLUSTERING

#include <ac_int.h>
#include <ac_fixed.h>
#include <ac_channel.h>
#include "include/ac_sqrt.h"
#include "include/ac_div.h"

#include "mc_scverify.h"

static const int Dimensions = 20;
static const int TotalPoints = 1000;
static const int MaxCenters = 20;

typedef ac_fixed<32,17,true> dimension;
typedef ac_fixed<32,17,false> dimension_unsigned;
typedef ac_fixed<64,34,false> dimension_powered;
typedef ac_int<1,false> control_signal;
typedef ac_int<15,false> cluster;
typedef ac_int<16,false> constants;

// Point Dimension Memory [Dimensions (20) x Total Points (1000)]
struct dim_mem{
	dimension mem[Dimensions*TotalPoints];
};

// K-centers Dimension Memory [Dimensions (20) x Max Centers (20)]
struct k_dim_mem{
	dimension mem[Dimensions*MaxCenters];
};

// Number of Points in Each Cluster Memory [Max Centers (20)]
struct n_points_in_clusters{
	cluster mem[MaxCenters];
};

// Single Point Dimensions Memory [Dimensions (20)]
struct temp_point{
	dimension mem[Dimensions];
};

#pragma hls_design top
class K_means_clustering {
	private:
		dim_mem point_dimensions_mem;				//dimension memory
		k_dim_mem current_center_dimensions_mem;	//center dimensions memory
		k_dim_mem next_center_dimensions_mem;		//new center dimensions memory
		temp_point temp_point_buffer;
		n_points_in_clusters point_cluster_count_buffer; 
		
		control_signal dim_mem_fit;
		control_signal centers_mem_fit;
		control_signal is_final_iter;
		control_signal stop_flag;
		
		cluster cluster_buffer;
		
		constants n_k;
		constants n_points;
		constants n_dimensions;
		constants n_iters;
		
	public:

		K_means_clustering(){
			dim_mem_fit = 1;
			centers_mem_fit = 1;
		}
		
		//Read number of points, dimensions, k-centers and iterations of problem. Return error if not able to process.
		void initialization(ac_channel<dimension> &input){		
			n_points = input.read().to_int();
			n_dimensions = input.read().to_int();
			n_k = input.read().to_int();
			n_iters = input.read().to_int();
			
			if (n_points*n_dimensions > Dimensions*TotalPoints) dim_mem_fit = 0;
			if (n_k*n_dimensions > Dimensions*MaxCenters) centers_mem_fit =0;
			
		}
		
		void calculate_sqrt(dimension_powered in_num, dimension_unsigned &product){
		    ac_math::ac_sqrt(in_num, product);
		}
		
		void divide(dimension in_dividant, dimension in_divisor, dimension &out){
		    ac_math::ac_div(in_dividant, in_divisor, out);
		}
		
		//Save point dimensions to memory. Initialize first centers as k-first points read.
		void input_stream_to_memory(ac_channel<dimension> &input){
IN_TO_MEM:	for (int i=0;i<n_points*n_dimensions;i++){
				dimension input_buffer = input.read();
				point_dimensions_mem.mem[i] = input_buffer;
				if (i<n_k*n_dimensions){
					current_center_dimensions_mem.mem[i] = input_buffer;
				}
			}
		}
		
		void examine_point(cluster current_point, ac_channel<cluster> &output){
			cluster local_cluster_buffer;			
			dimension_unsigned smallest_distance;
			smallest_distance.set_val<AC_VAL_MAX>();
MAIN_K:		for (int k=0; k<n_k; k++){
				dimension_powered distance_buffer_powered = 0;
				dimension_unsigned distance_buffer;
MAIN_DIM:		for (int j=0; j<n_dimensions;j++){
					if (k==0){
						temp_point_buffer.mem[j] = point_dimensions_mem.mem[n_dimensions*current_point + j];
					}
					
					dimension distance_sum_buffer = temp_point_buffer.mem[j]-current_center_dimensions_mem.mem[n_dimensions*k + j];
					distance_buffer_powered += distance_sum_buffer*distance_sum_buffer;
				}
				calculate_sqrt(distance_buffer_powered, distance_buffer);
				
				if (distance_buffer<smallest_distance){
					smallest_distance = distance_buffer;
					local_cluster_buffer = k;
				}
			}
			
			cluster_buffer = local_cluster_buffer;
			
		}
		
		void add_to_mean(){
			dimension point_cluster_count_buffer;
			point_cluster_count_buffer = point_cluster_count_buffer.mem[cluster_buffer] + 1;
			
MAIN_MEAN:	for (int j=0; j<n_dimensions;j++){
				// ALTERNATIVELY: Initalize all of next_center_dimensions_mem as '0'
				
				if (point_cluster_count_buffer ==  1){
					next_center_dimensions_mem.mem[n_dimensions*cluster_buffer+j] = temp_point_buffer.mem[j];
				}
				else{
					dimension mean_buffer=1;
				    divide(temp_point_buffer.mem[j] - next_center_dimensions_mem.mem[n_dimensions*cluster_buffer+j], point_cluster_count_buffer, mean_buffer);
					next_center_dimensions_mem.mem[n_dimensions*cluster_buffer+j] += mean_buffer;
				}
				
			}
			
			point_cluster_count_buffer.mem[cluster_buffer] = point_cluster_count_buffer.to_int();
		}
		
		void iterate(ac_channel<cluster> &output){
			ac::init_array<AC_VAL_0>(point_cluster_count_buffer.mem, MaxCenters);
MAIN_ITER:	for (int p=0; p<n_points; p++){
				examine_point(p,output);
				if (is_final_iter){
				    output.write(cluster_buffer);
			    }
			    else{
				    add_to_mean();
				}
			}
		}
		
		void new_centers(){
			stop_flag = 1;
CP_NEW_K:	for (int i=0; i<n_dimensions*n_k; i++){
				//If any center has changed position, don't raise the stop flag
				if (current_center_dimensions_mem.mem[i] != next_center_dimensions_mem.mem[i]){
					stop_flag = 0;
				}
				current_center_dimensions_mem.mem[i] = next_center_dimensions_mem.mem[i];
			}
		}
		
		
		//Run n_iter times the clustering fucntion for running the k-means algorithm for n_iter iterations
		#pragma hls_design interface
		void CCS_BLOCK(run_k_means)(ac_channel<dimension> &input, ac_channel<cluster> &output){
			constants current_iter = 0;
			stop_flag = 0;
			is_final_iter = 0;
			
			initialization(input);
			input_stream_to_memory(input);

RUN:		    while ((current_iter <= n_iters) && (is_final_iter == 0)){
				if (stop_flag==1 || current_iter == n_iters){
					is_final_iter = 1;
				}
				
				iterate(output);
				
				if (is_final_iter==0){
					new_centers();
				}
				current_iter++;

				
			}
		}
	
};

#endif
