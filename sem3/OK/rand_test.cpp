#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

int n = 4;

int vprob[] = {1,0,1,2};


int next_vertex(double prob){
	double sum = 0;
	for(int i = 0; i < n; i++){
		sum += vprob[i];
	}
	/* cout << "sum  " << sum << endl; */
	/* cout << prob << " "; */
	prob = (prob*sum)/100.0;
	int vert = 0;
	/* cout << prob << endl; */
	while( vert < n){
		/* cout << "co"; */
		if(prob - vprob[vert] < 0)
			break;
		/* cout << feromone_trail[a][vert] << " -> "; */
		prob -= vprob[vert];
		/* cout << prob << " | "; */
		vert++;
	}
	/* cout << vert << endl;; */

	return vert;
}

int main(){
	srand(time(NULL));
	int count[20] {};
	for(int i = 0; i < 1000; i++){
		int r = rand()%99;
		count[next_vertex(r)]++;
	}
	for(int i = 0; i < n; i++){
		cout << count[i] << endl;
	}
	return 0;
}
