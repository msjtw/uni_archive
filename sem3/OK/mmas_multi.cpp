#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <queue>
#include <random>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <cerrno>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <cstring>
#include <chrono>

#define SHM_SIZE 10485760
#define MAX_RETRIES 10
#define NEED_SEMUN
#ifdef NEED_SEMUN
/* Defined in sys/sem.h as required by POSIX now */
union semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
};
#endif

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
	bool visited[1010];
	int path[1010];
	int path_size;
	double length;

	ant(){
		length = 0;
		path_size = 0;
		current_vert = 0;
		memset(visited, 0, sizeof(visited));
        memset(path, 0, sizeof(path)); 
	}
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

int n;
vector<vector<edge>> trails;
vector<vertex> points;

double best_length = numeric_limits<double>::infinity();
vector<int> best_path;
int best_iter = 0;

double point_distance(int, int);
int next_vertex(ant, std::mt19937_64);
void trail_reinitialize();
int initsem(key_t, int, int);

int main(){
	auto start = std::chrono::high_resolution_clock::now();

	int keyE, keyF;
    int shmid;
    ant *ret_queue;
    int mode;
	keyE = ftok("/home/msjtw/Documents/uni/sem3/OK/mmas_multi.cpp", 'E');
	keyF = ftok("/home/msjtw/Documents/uni/sem3/OK/mmas_multi.cpp", 'F');
    shmid = shmget(keyE, sizeof(ant)*51, 0644 | IPC_CREAT);
    ret_queue = (ant *)shmat(shmid, (void *)0 , 0);

    int process_semid, write_semid;

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;  /* set to allocate resource */
    sb.sem_flg = SEM_UNDO;

    if ((process_semid = initsem(keyE, 1, 8)) == -1) {
        perror("initsem process");
        exit(1);
    }
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
		auto now = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = now - start;
		if(elapsed.count() >= 60 * 3){
			cout << "Timer" << endl;
			break;
		}

		//generate ants
		for(int ant_n = 0; ant_n < ant_count; ant_n++){
			if(!fork()){
				sb.sem_op = -1;
				if (semop(process_semid, &sb, 1) == -1) {
					perror("semop");
					exit(1);
				}

				std::random_device rd;
				std::mt19937_64 gen(rd());
				std::uniform_real_distribution<> probability(0.0, 1.0);

				// cout << ant_n << " ";
				ant curr_ant;

				int start = probability(gen)*n;
				curr_ant.current_vert = start;
				curr_ant.visited[start] = true;
				curr_ant.path[curr_ant.path_size] = start;
				curr_ant.path_size++;
				
				bool popsuted = false;
				while(curr_ant.path_size < n){
					int next_v = next_vertex(curr_ant, gen);
					if(next_v == -1){
						popsuted = true;
						break;
					}
					curr_ant.visited[next_v] = true;
					curr_ant.path[curr_ant.path_size] = next_v;
					curr_ant.path_size++;
					curr_ant.length += trails[curr_ant.current_vert][next_v].length;
					curr_ant.current_vert = next_v;
				}
				if(!popsuted){
					curr_ant.path[curr_ant.path_size] = curr_ant.path[0];
					curr_ant.path_size++;
					curr_ant.length += trails[curr_ant.current_vert][curr_ant.path[0]].length;

					ret_queue[ant_n] = curr_ant;
					// cout << ret_queue[ant_n].length << endl;

				}
				sb.sem_op = 1;
				if (semop(process_semid, &sb, 1) == -1) {
					perror("semop");
					exit(1);
				}
				exit(0);
			}
		}

		int wait_for = ant_count;
		while(wait_for > 0) {
			wait(NULL);
			wait_for--;
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

		// cout << "-----" << endl;
		// cout << ret_queue[0].length << endl;
		// cout << ret_queue[1].length << endl;
		// cout << ret_queue[2].length << endl;

		double iteration_best_length = numeric_limits<double>::infinity();
		vector<int> iteration_best_path;
		for(int ant_nn = 0; ant_nn < ant_count; ant_nn++){
			ant curr_ant = ret_queue[ant_nn];

			vector<int> curr_path;
			for(int i = 0; i < curr_ant.path_size; i++){
				curr_path.push_back(curr_ant.path[i]);
			}
			//check if new best
			if(curr_ant.length < best_length){
				best_length = curr_ant.length;
				best_path = curr_path;
				cout << "it: " << iteration << ", best: " << best_length << endl;
			}
			//check if iteration best
			if(curr_ant.length < iteration_best_length){
				iteration_best_length = curr_ant.length;
				iteration_best_path = curr_path;
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

int next_vertex(ant c_ant, std::mt19937_64 gen){
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

	std::uniform_real_distribution<> probability(0.0, 1.0);
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

int initsem(key_t key, int nsems, int processes)  /* key from ftok() */
{
	int i;
	union semun arg;
	struct semid_ds buf;
	struct sembuf sb = { 0 };  /* best to always init structs */
	int semid;

	semid = semget(key, nsems, IPC_CREAT | IPC_EXCL | 0666);

	if (semid >= 0) { /* we got it first */
		sb.sem_op = processes; sb.sem_flg = 0;
		arg.val = 1;

		for(sb.sem_num = 0; sb.sem_num < nsems; sb.sem_num++) { 
			/* do a semop() to "free" the semaphores. */
			/* this sets the sem_otime field, as needed below. */
			if (semop(semid, &sb, 1) == -1) {
				int e = errno;
				semctl(semid, 0, IPC_RMID); /* clean up */
				errno = e;
				return -1; /* error, check errno */
			}
		}
	} else if (errno == EEXIST) { /* someone else got it first */
		int ready = 0;

		semid = semget(key, nsems, 0); /* get the id */
		if (semid < 0) return semid; /* error, check errno */

		/* wait for other process to initialize the semaphore: */
		arg.buf = &buf;
		for(i = 0; i < MAX_RETRIES && !ready; i++) {
			semctl(semid, nsems-1, IPC_STAT, arg);
			if (arg.buf->sem_otime != 0) {
				ready = 1;
			} else {
				sleep(1);
			}
		}
		if (!ready) {
			errno = ETIME;
			return -1;
		}
	} else {
		return semid; /* error, check errno */
	}

	return semid;
}

