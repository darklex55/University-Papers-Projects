#include <iostream>
#include <ctime>
#include <cstdlib>

#include <ac_int.h>
#include <ac_fixed.h>

static const int W = 8;
static const int Wout =64;


typedef ac_int<8,false> int_eight;
typedef ac_int<9,true> int_nine_signed;
typedef ac_int<17,true> int_seventeen_signed;
typedef ac_int<18,true> int_eighteen_signed;

typedef ac_fixed<5,2,false> float_five_two;
typedef ac_fixed<5,4,true> float_five_four_signed;
typedef ac_fixed<9,5,true> float_nine_five_signed;
typedef ac_fixed<10,6,true> float_ten_six_ten_signed;

int main(){
	//a
	int_eight a = 255;
	int_eight b = 255;
	int_nine_signed c = 255;
	int_nine_signed d = 255;
	std::cout<<"255*255+255*255 = 130050\n\n";
	
	int_seventeen_signed e = a*b + c*d;
	std::cout<<"16 bit signed out: "<<e<<"\n";
	int_eighteen_signed f = a*b + c*d;
	std::cout<<"17 bit signed out: "<<f<<"\n";
	
	//b
	float_five_two A = 3.875;
	float_five_four_signed B = 7.5;
	
	std::cout<<"\n3.875*7.5 = 29.0625\n\n";
	float_nine_five_signed h = A*B;
	std::cout<<"6,9 float bit signed out: "<<h<<"\n";
	float_ten_six_ten_signed k = A*B;
	std::cout<<"6,10 float bit signed out: "<<k<<"\n";
}
