#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <random>
#include <algorithm>

using namespace std;

typedef pair<int, int> pii;

struct edge{
	double length;
	double inv_length;
	double feromones;
};

struct vertex{
	double x;
	double y;
	int idx;
};

struct ant{
	int current_vert;
	unordered_map<int, bool> visited;
	vector<int> path;
	double length = 0;
};

const int MX = 100;
const int MX_IT = 1e6;

double A = 1;
double B = 2;
double Evaporation_rate = 0.98;
int iteration_stagnation = 10000;
double t_max = 10;
double t_min = 1;
int update_global = 10000;
int ant_count;
double p_best = 0.05;
double p_dec;

std::random_device rd;
std::mt19937_64 gen(rd());
std::uniform_real_distribution<> probability(0.0, 1.0);

int n;
vector<vector<edge>> trails;
vector<vertex> points;

double best_length = numeric_limits<double>::infinity();
vector<int> best_path;
int best_iter = 0;

double point_distance(int, int);
int next_vertex(ant);
void trail_reinitialize();

int main(){
	// read data
	cin >> n;
	for(int i = 0; i < n; i++){
		double a, b;
		int idx;
		cin >> idx >> a >> b;
		points.push_back({a, b, idx});
	}
	ant_count = min(50,n/5);
	p_dec = pow(p_best, 1/n);
	cout << ant_count << endl;

	// initalaize trails
	trails.resize(n);
	for(int i = 0; i < n; i++){
		trails[i].resize(n);
		for(int k = 0; k < n; k++){
			trails[i][k].length = point_distance(i, k);
			trails[i][k].inv_length = 1/trails[i][k].length;
			trails[i][k].feromones = t_max;
		}
	}

	// iterations
	int cat_iteration = 0;
	for(int iteration = 0; iteration < MX_IT; iteration++, cat_iteration++){
		if(iteration - best_iter > iteration_stagnation){
			cout << "No new resaults for " << iteration_stagnation << " iterations. Exiting..." << endl;
			break;
		}
		queue<ant> ants;

		//generate ants
		for(int ant_n = 0; ant_n < ant_count; ant_n++){
			ant curr_ant;
			int start = rand()%n;
			curr_ant.current_vert = start;
			curr_ant.visited[start] = true;
			curr_ant.path.push_back(start);
			
			bool popsuted = false;
			while(curr_ant.path.size() < n){
				int next_v = next_vertex(curr_ant);
				if(next_v == -1){
					popsuted = true;
					break;
				}
				curr_ant.visited[next_v] = true;
				curr_ant.path.push_back(next_v);
				curr_ant.length += trails[curr_ant.current_vert][next_v].length;
				curr_ant.current_vert = next_v;
			}
			if(!popsuted){
				curr_ant.path.push_back(curr_ant.path[0]);
				curr_ant.length += trails[curr_ant.current_vert][curr_ant.path[0]].length;
				ants.push(curr_ant);
			}
		}

		//process resaults
		//evaporate trails
		double feromone_sum = 0;
		for(int i = 0; i < n; i++){
			for(int k = 0; k < n; k++){
				trails[i][k].feromones = min(trails[i][k].feromones, t_max);
				trails[i][k].feromones *= Evaporation_rate;
				trails[i][k].feromones = max(trails[i][k].feromones, t_min);
				feromone_sum += trails[i][k].feromones;
			}
		}
		for(int i = 0; i < best_path.size(); i++){
			feromone_sum -= trails[best_path[i]][best_path[i+1]].feromones;
		}

		// if(feromone_sum < 1.2*t_min*n*(n-1)){
		// 	cout << "mao" << endl;
		// 	trail_reinitialize();
		// 	cat_iteration = 0;
		// }

		double iteration_best_length = numeric_limits<double>::infinity();
		vector<int> iteration_best_path;
		while(!ants.empty()){
			ant curr_ant = ants.front();
			ants.pop();

			//check if new best
			if(curr_ant.length < best_length){
				best_length = curr_ant.length;
				best_path = curr_ant.path;
				cout << "it: " << iteration << ", best: " << best_length << endl;
			}
			//check if iteration best
			if(curr_ant.length < iteration_best_length){
				iteration_best_length = curr_ant.length;
				iteration_best_path = curr_ant.path;
			}
		}
	
		/* cout << iteration; */
		//add feromones
		if(iteration % update_global != 0){
			/* cout << " local" << endl; */
			for(int i = 0; i < n-1; i++){
				trails[iteration_best_path[i]][iteration_best_path[i+1]].feromones += 1/iteration_best_length;
				trails[iteration_best_path[i+1]][iteration_best_path[i]].feromones += 1/iteration_best_length;
				trails[iteration_best_path[i]][iteration_best_path[i+1]].feromones = min(trails[iteration_best_path[i]][iteration_best_path[i+1]].feromones, t_max);
				trails[iteration_best_path[i+1]][iteration_best_path[i]].feromones = min(trails[iteration_best_path[i+1]][iteration_best_path[i]].feromones, t_max);
			}
		}
		else{
			/* cout << " global" << endl; */
			for(int i = 0; i < n-1; i++){
				trails[best_path[i]][best_path[i+1]].feromones += 1/best_length;
				trails[best_path[i+1]][best_path[i]].feromones += 1/best_length;
				trails[best_path[i]][best_path[i+1]].feromones = max(trails[best_path[i]][best_path[i+1]].feromones, t_min);
				trails[best_path[i+1]][best_path[i]].feromones = max(trails[best_path[i+1]][best_path[i]].feromones, t_min);
			}
		}

		//change global update frequency
		if(cat_iteration > 50){
			update_global = 5;
		}
		if(cat_iteration > 150){
			update_global = 3;
		}
		if(cat_iteration > 250){
			update_global = 2;
		}
		if(cat_iteration > 500){
			update_global = 1;
		}

		//update trail limits
		t_max = (1/(1.0-Evaporation_rate))*(1/best_length);
		double avg = n/2.0;
		t_min = min(t_max, (t_max*(1-p_dec))/((avg-1)*p_dec));
		/* cout << t_max << " " << t_min << endl; */
	}

	cout << best_length << endl;
	return 0;
}

double point_distance(int a, int b){
	double x1, x2, y1, y2;
	x1 = points[a].x;
	x2 = points[b].x;
	y1 = points[a].y;
	y2 = points[b].y;
	double x = x1-x2;
	x *= x;
	double y = y1-y2;
	y *= y;
	return sqrt(x+y);
}

int next_vertex(ant c_ant){
	vector<double> probabilites(n);
	double sum_of_prob = 0;

	for(int i = 0; i < n; i++){
		if(c_ant.visited[i]){
			probabilites[i] = 0;
			continue;
		}
		probabilites[i] = pow(trails[c_ant.current_vert][i].feromones, A) * pow(trails[c_ant.current_vert][i].inv_length, B);
		sum_of_prob += probabilites[i];
	}
	double prob = probability(gen)*sum_of_prob;
	for(int i = 0; i < n; i++){
		if(prob - probabilites[i] < 0)
			return i;
		prob -= probabilites[i];
	}
	
	return -1;
}

void trail_reinitialize(){
	for(int i = 0; i < n; i++){
		for(int k = 0; k < n; k++){
			trails[i][k].feromones = t_max;
		}
	}
}
