#include <iostream>
#include <ctime>
#include <cstdlib>

#include <ac_int.h>

static const int W = 8;
static const int Wout =64;


typedef ac_int<W,false> data_t;
typedef ac_int<W,true> data_t_s;
typedef ac_int<Wout,true> data_t_mul;


struct csd {
	data_t xP;
	data_t xN;
	ac_int<1,true> s;
};

void csd_encode(data_t_s &num, csd &num_csd){
	csd c;
	c.xP=0;
	c.xN=0;
	c.s=num[W-1];
	
	int i = 0;
	bool carry = false;
	ac_int<W,false> x = num;
	x[W-1]=0;
	
	num_csd.xP = 0;
	num_csd.xN = 0;
	
	
	while (i<W-1) {
		if (carry){
			if(x[i]==1){
				carry=true;
			}
			else{
				if(x[i+1]==0){
					c.xP[i]=1;
					carry=false;
				}
				else{
					c.xN[i]=1;
					carry=true;
				}
			}
		}
		else{
			if(x[i]==0){
				carry=false;
			}
			else{
				if (x[i+1]==0){
					c.xP[i]=1;
					carry=false;
				}
				else{
					c.xN[i]=1;
					carry=true;
				}
			}
		}
		i++;
	}
	
	if (carry) {c.xP[W-1] = 1;}
	else {c.xP[W-1] = 0;}
	
	num_csd = c;
	
}

data_t_mul csd_mult (data_t_s &in, const csd &cons_csd){
	data_t_mul out = 0;
	
	for (int i=0;i<W;++i){
		out+= (in<<i)*(cons_csd.xP[i] - cons_csd.xN[i]);
	}
	
	return out;
}

int main(){
	
	data_t_s a[] = {13, 18, 36, 65, 92, 6};
	
	std::cout << 'CSD encoder testing' << '\n';
	
	for (int i=0;i<6;i++){
		csd o;
		csd_encode(a[i],o);

		std::cout<<a[i]<<" = "<<o.xP<<" - "<<o.xN<<'\n';
	}
	
	data_t_s con = 6;
	csd c_csd;
	csd_encode(con,c_csd);
	
	std::cout << "\nCSD mul testing" << '\n';
	
	for (int i=0;i<6;i++){
		data_t_mul mul_prod = csd_mult(a[i],c_csd);
		std::cout<<con<< " x " << a[i] << " = "<< mul_prod <<'\n';
	}
}
