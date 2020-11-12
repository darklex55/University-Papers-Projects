#include <iostream>
#include <ctime>
#include <cstdlib>
#include <vector>

#include <ac_int.h>
#include <ac_channel.h>

static const int Tests=10;

typedef ac_int<4,false> data_t;

class RunLengthEncoder{
	private:
		std::vector<data_t> mem;
	public:
		RunLengthEncoder(){
			mem = std::vector<data_t>(16,0);
		}
		
		void run(ac_channel<data_t> &in, ac_channel<data_t> &out){
			if (in.available(1)){
				data_t next = in.read();
				mem[next]+=1;
				run(in, out);
			}
			for (int i=0;i<16;i++){
				if (mem[i]!=0){
					out.write(i);
					out.write(mem[i]);
					mem[i]=0;
				}
			}
		}
};

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
		
		RunLengthEncoder encoder;
		
		encoder.run(in_test, out_test);
		
		std::cout<<"\nOutput vector: ";
				
		for (int n=0; n<10; n++){
			data_t out = out_test.read();
			std::cout<<out<<" ";
		}
	}
	
	return 0;
}
