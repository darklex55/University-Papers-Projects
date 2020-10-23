#include <iostream>
#include <ctime>
#include <cstdlib>

static const int Tests=10;
static const int N=8;

typedef unsigned int Data_t;
typedef Data_t Array[N];

typedef unsigned short int Data_t_s;
typedef Data_t_s Array_s[N];

void array_print (Data_t a[], int size) {
	for (int i=0; i < size; ++i){
		std::cout << a[i] << ", ";
	}
}

void array_print (Data_t_s a[], int size) {
	for (int i=0; i < size; ++i){
		std::cout << a[i] << ", ";
	}
}

void run(Data_t A[N], Data_t_s B[N]){
	
	Data_t a;
	Data_t_s ones_counter;
	
	for (int n=0; n<N; ++n){
		a = A[n];
		ones_counter = 0;
		
		while(a>0){
			if (a%2){++ones_counter;}
			a/=2;
		}
		
		B[n]=ones_counter;
	}
}

int main() {
	Array in_a[Tests];
	Array_s in_b[Tests];
	
	std::srand(std::time(NULL));
	
	for (int i=0; i<Tests; ++i){
		for (int n=0; n<N; ++n){
			if (i<Tests/2){in_a[i][n] = std::rand() % UINT_MAX;}
			else{in_a[i][n] = std::rand() * UINT_MAX;}
		}
		std::cout << "\n\nInput:\n";
		array_print(in_a[i], N);
		
		run(in_a[i],in_b[i]);
		
		std::cout << "\nOutput:\n ";
		
		array_print(in_b[i], N);
	}

	return 0;
}
