#include <iostream>
#include <vector>

using namespace std;

const bool PLUS_ONE_IO = true; // if true, vertices, edges, colours are numbered starting with 1 instead of 0 (affects input and output)
const bool VERTICES_ARE_LETTERS = true; // if true, vertices are numbered by uppercase letters (no bound checking, does not affect input, only output, unaffected by PLUS_ONE_IO)

// prints colour assignment of V vertices as given in col and displays all vertices after insig as "X"
void print_cols(int V, vector<int> col, int insig) {
	for (int i = 0; i < V; i++) {
		if (VERTICES_ARE_LETTERS) { printf(" %c ", i + 'A'); } else { printf("%2d ", i + PLUS_ONE_IO); }
	}
	printf("\n");
	for (int i = 0; i < V; i++) {
		if (i <= insig) {
			printf("%2d ", col[i] + PLUS_ONE_IO);
		} else {
			printf(" x ");
		}
	}
	printf("\n");
}

// returns the vertex it finished at or V is UB was not used
int forward(int start_vertex, int V, int UB, vector<int> &col, vector<vector<int>> graph, vector<vector<bool>> &used_colour) {
	for (int i = start_vertex; i < V; i++) {
		int c = 0;
		while (true) {
			if (c >= UB) { return i; }
			bool feasible = true;
			for (int &v : graph[i]) {
				if (v < i && c == col[v]) { feasible = false; break; }
			}
			if (feasible) {
				col[i] = c;
				used_colour[i][c] = true;
				break;
			}
			c++;
		}
	}
	return V;
}

// returns the vertex it finished at; if 0, that means we're done
int backtrack(int start_vertex, int UB, vector<int> &col, vector<vector<int>> graph, vector<vector<bool>> &used_colour) {
	for (int i = start_vertex; i > 0; i--) {
		for (int c = 0; c < UB; c++) {
			if (used_colour[i][c]) { continue; cout << "b"; }
			bool feasible = true;
			for (int &v : graph[i]) {
				if (v < i && c == col[v]) { feasible = false; break; }
			}
			if (feasible) {
				col[i] = c;
				used_colour[i][c] = true;
				return i;
			}
		}
	}
	return 0; // first vertex reached, stop
}

int main() {
	int V, E;
	printf("Vertices: "); cin >> V;
	printf("Edges: "); cin >> E;
	vector<int> col = vector<int>(V, -1);
	vector<vector<int>> graph = vector<vector<int>>(V, vector<int>());
	vector<vector<bool>> used_colour = vector<vector<bool>>(V, vector<bool>(V, false));
	printf("Edge list:\n");
	int a, b;
	for (int i = 0; i < E; i++) {
		cin >> a >> b;
		a -= PLUS_ONE_IO; b -= PLUS_ONE_IO;
		graph[a].push_back(b);
		graph[b].push_back(a);
	}

	printf("Graph:\n");
	for (int i = 0; i < V; i++) {
		if (VERTICES_ARE_LETTERS) { printf("%c:", 'A' + i); } else { printf("%2d:", i + PLUS_ONE_IO); }
		for (int &v : graph[i]) {
			if (VERTICES_ARE_LETTERS) { printf(" %c", 'A' + v); } else { printf(" %2d", v + PLUS_ONE_IO); }
		}
		printf("\n");
	}

	int UB = V;
	int last_j = 0;
	int j = -1;

	//char c;
	while (true) {
		//c = getchar();
		last_j = j;
		j = forward(j + 1, V, UB, col, graph, used_colour);
		printf("After forward:\n");
		print_cols(V, col, j);

		if (j == V) {
			// pdf case B (last vertex reached)
			UB = 0;
			for (auto &c : col) { UB = max(UB, c); }
			printf("UB = %d\n", UB + PLUS_ONE_IO);
			j = backtrack(V - 2, UB, col, graph, used_colour);
		} else {
			// pdf case A (UB was required midway)
			printf("UB or greater was needed, forward failed.\n");
			j = backtrack(last_j, UB, col, graph, used_colour);
		}
		printf("After backtrack:\n");
		print_cols(V, col, j);
		if (j == 0) { break; }
	}

	printf("Algorithm end. Final assignment:\n");
	print_cols(V, col, V);
	return 0;
}

