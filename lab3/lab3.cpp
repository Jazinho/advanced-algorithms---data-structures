#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <math.h>

using namespace std;

double fun(double x);
double doubleRand();

int main(){

	double dx=0.0000001; 
	int p=1/dx;
	double res[4];
	double sin_res=0;

	double single_start_time = omp_get_wtime();

	for(int i=0;i<p;i++){
		sin_res+=fun(i*dx)*dx;
	}

	double single_end_time = omp_get_wtime();

	cout << "Single thread result x4 =" << sin_res*4 << "\n";
	cout << "Single thread execution took " << single_end_time - single_start_time << "[s]\n";
	
	double multi_start_time = omp_get_wtime();

	#pragma omp parallel
	{
		int k=omp_get_thread_num();
		int n=omp_get_num_threads();
		double w=0;

		//cout << "k: " << k << ", n: " << n << "\n";

		for(int i=k;i<p;i+=4){
			w+=fun(i*dx)*dx;
		}

		//cout << "Adding for " << k << "\n";
		res[k]=w;
	}

	double total=0;
	for(int j=0;j<4;j++){
		//cout << "res[]=" << res[j] << "\n"; 
		total+=res[j];
	}

	double multi_end_time = omp_get_wtime();

	cout << "Total result x4 is: " << total*4 << "\n";
	cout << "Multi thread execution took " << multi_end_time - multi_start_time << "[s]\n";

	// PRAGMA REDUCTION SYNTAX

	double red_res=0;
	#pragma omp parallel for reduction(+:red_res)
	for(int l=0;l<p;l++){
		red_res=red_res+fun(l*dx)*dx;
	}
	
	cout << "Reduction_result=" << red_res*4 << "\n";

	srand(static_cast<unsigned int>(clock()));
	double pts_num = 1000000;
	double inside_pts_num = 0;
	double r=0.5;

	for(int i=0;i<pts_num;i++){
		double x=doubleRand();
		double y=doubleRand();
		double distance = sqrt(pow(r-x,2)+pow(r-y,2));

		if(distance < r){
			inside_pts_num++;
		}
	}

    // Wiemy, ze pole powierzchni kola wpisanego w kwadrat o boku 1
    // wynosi dokladnie pi/4. Stosunek pola tego kola do kwadratu to
    // tez pi/4. A wiec aby uzyskac przyblizenie liczby pi wystarczy
    // policzyc stosunek punktow_w_kole do punktow_w_kwadracie razy 4
	
	double circle_area = inside_pts_num/pts_num; //statistically calculated circe area
	double pi = circle_area * 4; //because circle area is pi*r^2
	//cout << "Inside points: " << inside_pts_num << "\n";
	//cout << "circle_area = " << circle_area << "\n"; 
	cout << "Circle points in/all x4 = " << pi << "\n"; 


	return 0;
}

double fun(double x){
	return 1.0/(1+x*x);
}

double doubleRand() {
  return double(rand()) / (double(RAND_MAX) + 1.0);
}

