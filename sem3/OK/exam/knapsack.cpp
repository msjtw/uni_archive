// The "second" method

#include <iostream>
#include <vector>

using namespace std;

typedef pair<int, int> pii;

const int INF = 1e6;
int UB = 0;

vector <pii> items;

vector <vector<int>> solution;

int main(){
	//read data
	int n, b_size;
	cin >> n >> b_size;
	items.push_back({0,0});
	//size
	for(int i = 0; i < n; i++){
		int a;
		cin >> a;
		items.push_back({a,0});
	}
	//value
	for(int i = 1; i < n+1; i++){
		int a;
		cin >> a;
		items[i].second = a;
		UB += a;
	}
	UB++;

	//initialize
	solution.resize(n+1);
	for(int i = 0; i < n+1; i++){
		solution[i].resize(UB);
	}
	for(int i = 0; i < n+1; i++){
		for(int k = 0; k < UB; k++){
			solution[i][k] = INF;
		}
	}
	for(int i = 0; i < n+1; i++){
		solution[i][0] = 0;
	}
	
	//solve
	int max_sol = 0;
	for(int i = 1; i < n+1; i++){
		for(int k = 1; k < UB; k++){
			int a = solution[i-1][k];
			int b = INF;
			if(k-items[i].second >= 0)
				b = min(b,solution[i-1][k-items[i].second]+items[i].first);
			solution[i][k] = min(a, b);
			if(solution[i][k] > b_size){
				solution[i][k] = INF;
			}
			if(solution[i][k] < INF){
				max_sol = max(max_sol, k);
			}
		}
	}

	//print
	const int n_zero = 3;
	cout << "    ";
	for(int i = 1; i < UB; i++){
		string val = to_string(i);
		cout << string(n_zero - val.length(), ' ') + val << " ";
	}
	cout << endl;
	cout << "    ";
	for(int i = 1; i < UB; i++){
		cout << "____";
	}
	cout << endl;
	for(int i = 1; i < n+1; i++){
		cout << i << " | ";
		for(int k = 1; k < UB; k++){
			if(solution[i][k] == INF){
				cout << " -- ";
			}
			else{
				string val = to_string(solution[i][k]);
				cout << string(n_zero - val.length(), ' ') + val << " ";
			}
		}
		cout << endl;
	}

	cout << "max solution: " << max_sol << endl;
	cout << "       items: " ;
	int curr_item = n;
	while(curr_item){
		if(solution[curr_item][max_sol] == solution[curr_item-1][max_sol]){
			curr_item--;
		}
		else{
			cout << curr_item << " ";
			max_sol -= items[curr_item].second;
			curr_item--;
		}
	}
	cout << endl;

	return 0;
}
