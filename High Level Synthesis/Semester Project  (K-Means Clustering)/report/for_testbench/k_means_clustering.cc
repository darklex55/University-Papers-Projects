#include <iostream>
#include <float.h>
#include <fstream>

#include "k_means_clustering.h"

CCS_MAIN(int argc, char* argv[]){
	
	float val;
	ac_channel<dimension> input_channel;
	ac_channel<cluster> output_channel;
	
	//Read input data from file
	std::ifstream file("data/dimensions_testbench.txt");

	while (file){
		file >> val;
		if (file){
		    std::cout<<val<<"\n";
			input_channel.write(val);
		}
	}
	
	file.close();
	
	K_means_clustering clusterizer;
	clusterizer.run_k_means(input_channel, output_channel);
	
	//Write labels to file
	std::ofstream ofile;
	ofile.open("data/labels_output.txt");
	std::cout<<"OUT\n";
	while (output_channel.available(1)){
	    int out = output_channel.read();
	    std::cout<<out<<"\n";
		ofile<<(int) out<<std::endl;
	}
	
	CCS_RETURN(0);
}
