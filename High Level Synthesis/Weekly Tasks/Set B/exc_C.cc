#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

#include <ac_int.h>
#include <ac_channel.h>

static const int Tests=10;

typedef ac_int<4,false> data_t;

void runlength_encode(ac_channel<data_t> &in, ac_channel<data_t> &out){
	if (in.available(10)){
		int out_cnt = 0;
		std::vector<data_t> ls;
		std::vector<data_t> ls_cnt;
		
		for (int i=0;i<10;i++){
			data_t next = in.read();
			bool in_not_exists = true;
			
			if (out_cnt>0){
				for (int o=0;o<out_cnt;o++){
					if (ls[o]==next){
						ls_cnt[o]+=1;
						in_not_exists=false;
					}
				}
			}
			
			if (in_not_exists){
				ls.push_back(next);
				ls_cnt.push_back(1);
			}
			out_cnt+=1;
		}
		
		for (int i=0;i<out_cnt;i++){
			out.write(ls[i]);
			out.write(ls_cnt[i]);
		}
	}
}

int main(){
	std::srand(std::time(NULL));
	
	for (int i=0; i<Tests;i++){
		
		ac_channel<data_t> in_test;
		ac_channel<data_t> out_test;
		
		std::cout<<"\n\nTest "<< i+1 << " input vector: ";
		for (int n=0;n<10;n++){
			int in = std::rand()%9;
			in_test.write(in);
			std::cout<<in<<" ";
		}
		runlength_encode(in_test, out_test);
		
		std::cout<<"\nOutput vector: ";
				
		for (int n=0; n<10; n++){
			data_t out = out_test.read();
			std::cout<<out<<" ";
		}
	}
	
	return 0;
}
