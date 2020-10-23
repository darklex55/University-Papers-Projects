#include <iostream>
#include <ctime>
#include <cstdlib>

static const int Tests=10;
static const int N=8;

typedef unsigned int Data_t;
typedef Data_t Array[N];

void array_print (Data_t a[], int size) {
	for (int i=0; i < size; ++i){
		std::cout << a[i] << ", ";
	}
}

void run(Data_t A[N], Data_t B[N]){
	
	Data_t a, checksum, sub;
	short int counter;
	
	for (int n=0; n<N; ++n){
		a = A[n];
		checksum = 0;
		sub = 0;
		counter = 0;
		
		while(a>0){
			if ((++counter%2)==0){
				if (2*(a%10)>9){
					sub = 2*(a%10);
					checksum += sub%10 + 1;
				}
				else{
					checksum += 2*(a%10);
				}
			}
			else{
				checksum += a%10;
			}
			a -= (a%10);
			a /= 10;
		}
		
		B[n] = checksum;
	}
	
	
}

int main() {
	Array in_a[Tests], in_b[Tests];
	
	std::srand(std::time(NULL));
	
	for (int i=0; i<Tests; ++i){
		for (int n=0; n<N; ++n){
			if (i<Tests/2){in_a[i][n] = std::rand() % 128;}
			else{in_a[i][n] = std::rand() % 65536;}
		}
		std::cout << "\n\nInput:\n";
		array_print(in_a[i], N);
		
		run(in_a[i],in_b[i]);
		
		std::cout << "\nOutput:\n ";
		
		array_print(in_b[i], N);
	}

	return 0;
}
