#include <iostream>
#include <float.h>
#include <fstream>

#include "k_means_clustering.h"

int main(){
	
	float val;
	ac_channel<dimension> input_channel;
	ac_channel<cluster> output_channel;
	
	//Read input data from file
	std::ifstream file("data/5d_dimensions.txt");

	while (file){
		file >> val;
		if (file){
			input_channel.write(val);
		}
	}
	
	file.close();
	
	K_means_clustering clusterizer;

	clusterizer.run_k_means(input_channel, output_channel);
	
	//Write labels to file
	std::ofstream ofile;
	ofile.open("data/5d_labels.txt");
	while (output_channel.available(1)){
	    int out = output_channel.read();
		ofile<<(int) out<<std::endl;
	}
	
	return 0;
}
