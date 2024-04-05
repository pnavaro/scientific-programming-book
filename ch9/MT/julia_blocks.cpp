#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <thread>
#include <math.h>
using namespace std;

void outputField(int **data, int ni, int nj);

// support for complex numbers
struct Complex {
	double r;
	double i;
 
	Complex(double a, double b) : r(a), i(b) { }

	double magnitude2() {return r*r + i*i;}
	
	Complex operator*(const Complex& a) {
		return Complex(r*a.r - i*a.i, i*a.r + r*a.i);
	}
	Complex operator+(const Complex& a) {
		return Complex(r + a.r, i + a.i);
	}
 };
    	
 // computes value of julia set at [i,j]*/
 int juliaValue(int i, int j, int ni, int nj) 
 {
	double fi = -1.0 + 2.0*i/ni;	// fi = [-1:1)
	double fj = -1.0 + 2.0*j/nj;	// fj = [-1:1)
	
   Complex c(-0.8, 0.156);	// coefficient for the image
   Complex a(fi, fj);		// pixel pos as a complex number

   int k;					// iteration counter
   for( k = 0; k < 200; k++) {
     a = a * a + c;
     if (a.magnitude2() > 1000) break;	// check for divergence
   }
   return k;				// return 
 }

void calculatePixels(int i_start, int i_end, int ni, int nj, int **julia, int thread_id) {
	for (int i=i_start;i<i_end;i++)
		for (int j=0;j<nj;j++) 
			julia[i][j] = juliaValue(i,j,ni, nj);
		//	julia[i][j] = thread_id;
} 

int main(int num_args, char **args)
{
	const int ni=4000;
	const int nj=4000;

	/*allocate memory for our domain*/
	int **julia=new int*[ni];
	for (int i=0;i<ni;i++) julia[i]=new int[nj];

	// set some initial check values
	for (int i=0;i<ni;i++)
		for (int j=0;j<nj;j++) julia[i][j] = -99;

	int num_threads = thread::hardware_concurrency();  // set some default

	// first argument ([0]) is the path to the executable
	// the first command line argument is our argument 2 - index [1]
	if (num_args>1) {
		num_threads = atoi(args[1]);
	}

	cout<<"Running with "<<num_threads<<" threads"<<endl;

	vector<thread> threads;
	threads.reserve(num_threads);

	/*start timing*/
  	auto clock_start = chrono::high_resolution_clock::now();

	for (int t=0;t<num_threads;t++ ) {

		// with num_threads = 4, we have 4000/4=1000 or 1000 "ni" rows per thread
		// for t=0, i_start=0, t=1, i_start=1000, t=2, i_start=2000, etc...
		int i_start = t*(ni/num_threads);
		int i_end = (t+1)*(ni/num_threads);
		if (t==num_threads-1) i_end=ni;		// correct upper limit on the last thread

		threads.emplace_back(calculatePixels, i_start, i_end, ni, nj, julia,t);
	}

	for (thread &t:threads) t.join();		// blocks until thread "t" finishes its calculatePixels

	/*capture ending time*/
  	auto clock_end = chrono::high_resolution_clock::now();

	outputField(julia,ni,nj);
 	
	std::chrono::duration<float> delta = clock_end-clock_start;
    cout << "Simulation took "<<delta.count()<< "s\n";

	// free memory
	for (int i=0;i<ni;i++) delete[] julia[i];
	delete[] julia;	

	return 0;
}

/*saves output in VTK format*/
void outputField(int **data, int ni, int nj)
{
	stringstream name;
	name<<"julia-blocks.vti";

    /*open output file*/
    ofstream out(name.str());
   	if (!out.is_open()) {cerr<<"Could not open "<<name.str()<<endl;return;}

	/*ImageData is vtk format for structured Cartesian meshes*/
	out<<"<VTKFile type=\"ImageData\">\n";
	out<<"<ImageData Origin=\""<<"0 0 0\" ";
	out<<"Spacing=\"1 1 1\" ";
	out<<"WholeExtent=\"0 "<<ni-1<<" 0 "<<nj-1<<" 0 0\">\n";
	
	/*output data stored on nodes (point data)*/
	out<<"<PointData>\n";
	
	/*potential, scalar*/ 
	out<<"<DataArray Name=\"julia\" NumberOfComponents=\"1\" format=\"ascii\" type=\"Int32\">\n";
	for (int j=0;j<nj;j++)
	{
		for (int i=0;i<ni;i++) out<<data[i][j]<<" ";
		out<<"\n";
	}
	out<<"</DataArray>\n";

	/*close out tags*/
	out<<"</PointData>\n";
	out<<"</ImageData>\n";
	out<<"</VTKFile>\n";
 	out.close();
}
	    
