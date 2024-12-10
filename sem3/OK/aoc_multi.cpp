#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <random>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/wait.h>

using namespace std;

const int MX = 10000;

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

struct POD_ant{
	int path[MX];
	double res;
};

struct ant_msgbuf {
	long mtype;
	POD_ant res;
};

const int MX_IT = 1000;

double A = 1;
double B = 5;
double Evaporation_rate = 0.5;
double Q = 100;
double C = 1;
int iteration_stagnation = 100;
int max_ant_num = 8;

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

int main(){
	//setup queue
	int key = ftok("/home/msjtw/Documents/uni/s3_OK/aoc_multi.cpp", 'b');
	int msqid = msgget(key, 0666 | IPC_CREAT);

	// read data
	cin >> n;
	for(int i = 0; i < n; i++){
		double a, b;
		int idx;
		cin >> idx >> a >> b;
		points.push_back({a, b, idx});
	}

	// initalaize trails
	trails.resize(n);
	for(int i = 0; i < n; i++){
		trails[i].resize(n);
		for(int k = 0; k < n; k++){
			trails[i][k].length = point_distance(i, k);
			trails[i][k].inv_length = 1/trails[i][k].length;
			trails[i][k].feromones = C;
		}
	}
	
	// iterations
	for(int iteration = 0; iteration < MX_IT; iteration++){
		if(iteration - best_iter > iteration_stagnation){
			cout << "No new resaults for " << iteration_stagnation << " iterations. Exiting..." << endl;
			break;
		}
		queue<ant> ants;

		//generate ants
		int ant_n = 0;
		int finished_ants = 0;
		int curr_num_ant = 0;
		while(ant_n < n){
			if(curr_num_ant < max_ant_num){
				/* cout << "new ant" << curr_num_ant << " " << ant_n << endl; */
				curr_num_ant++;
				ant_n++;
				if(!fork()){
					ant curr_ant;
					curr_ant.current_vert = ant_n;
					curr_ant.visited[ant_n] = true;
					curr_ant.path.push_back(ant_n);
					
					while(curr_ant.path.size() < n){
						int next_v = next_vertex(curr_ant);
						curr_ant.visited[next_v] = true;
						curr_ant.path.push_back(next_v);
						curr_ant.length += trails[curr_ant.current_vert][next_v].length;
						curr_ant.current_vert = next_v;
					}
					curr_ant.path.push_back(curr_ant.path[0]);
					curr_ant.length += trails[curr_ant.current_vert][curr_ant.path[0]].length;

					ant_msgbuf buff;
					buff.mtype = 1;
					buff.res.res = curr_ant.length;
					for(int i = 0; i < curr_ant.path.size(); i++){
						buff.res.path[i] = curr_ant.path[i];
					}
					cout << "ant: " << ant_n << " finished" << curr_ant.length << endl;
					if(msgsnd(msqid, &buff, sizeof(buff.res), 0) == -1)
						perror("mgs send");
					exit(0);
				}
			}
			else{
				int status;
				int finished_pid = waitpid(-1, &status, 0);
				if(finished_pid > 0){
					curr_num_ant--;
					ant_msgbuf buff;
					if (msgrcv(msqid, &buff, sizeof(buff.res), 0, 0) == -1) {
						perror("msgrcv");
						exit(1);
					}
					ant fenix;
					fenix.length = buff.res.res;
					cout << fenix.length << endl;
					for(int i = 0; i < n; i++){
						fenix.path.push_back(buff.res.path[i]);
					}
					ants.push(fenix);
				}
			}
		}

		while(ants.size() < n-1){
			int status;
			int finished_pid = waitpid(-1, &status, 0);
			if(finished_pid > 0){
				ant_msgbuf buff;
				if (msgrcv(msqid, &buff, sizeof(buff.res), 0, 0) == -1) {
					perror("msgrcv");
					exit(1);
				}
				ant fenix;
				fenix.length = buff.res.res;
				for(int i = 0; i < n; i++){
					fenix.path.push_back(buff.res.path[i]);
				}
				ants.push(fenix);
			}
		}

		cout << "ok" << endl;

		cout << ants.size() << endl;

		//process resaults
		//evaporate trails
		for(int i = 0; i < n; i++){
			for(int k = 0; k < n; k++){
				trails[i][k].feromones *= Evaporation_rate;
			}
		}
		int i_num = 0;
		while(!ants.empty()){
			i_num++;
			ant curr_ant = ants.front();
			ants.pop();

			//check if new best
			if(curr_ant.length < best_length){
				best_length = curr_ant.length;
				best_path = curr_ant.path;
				cout << "it: " << iteration << ", best: " << best_length << " " << i_num << endl;
			}
			//add new feromones
			for(int i = 0; i < n; i++){
				trails[curr_ant.path[i]][curr_ant.path[i+1]].feromones += Q/curr_ant.length;
			}
		}
	}

	cout << best_length << endl;

	if (msgctl(msqid, IPC_RMID, NULL) == -1) {
		perror("msgctl");
		exit(1);
	}

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
		if(c_ant.visited[i])
			continue;
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
