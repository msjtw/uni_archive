#include <iostream>
#include <vector>
#include <cmath>
#include <bitset>
#include <climits>

constexpr int MX = 10007;
double dist[MX][MX];
std::pair<double, double> pos[MX];
std::bitset<MX> visited;

int main()
{
	/* std::cout << "Podaj n:" << std::endl; */
	int n;
	std::cin >> n;

	if (n > MX)
	{
		std::cout << "Za duze n\n";
		return 1;
	}

	for (int i = 1; i <= n; ++i)
	{
		int tmp;
		/* std::cout << "Podaj wspolrzedne punktu: "; */
		std::cin >> tmp >> pos[i].first >> pos[i].second;
	}
	for (int i = 1; i <= n; ++i)
	{
		for (int j = 1; j <= n; ++j)
		{
			dist[i][j] = sqrt(pow(pos[i].first - pos[j].first, 2.0) + pow(pos[i].second - pos[j].second, 2.0));
		}
	}

	/* std::cout << "Podaj indeks startowego wierzcholka: "; */
	int start = 1;
	//std::cin >> start;
	visited[start] = true;

	std::vector<int> res;
	res.push_back(start);
	double total_dist = 0;

	int poz = start;
	while (res.size() < n)
	{
		double mini = std::numeric_limits<double>::infinity();
		int m_poz;
		for(int i = 1; i <= n ; i++){
			if(dist[poz][i] > 0 and dist[poz][i] < mini and !visited[i]){
				mini = dist[poz][i];
				m_poz = i;
			}
		}
		res.push_back(m_poz);
		total_dist += mini;
		visited[m_poz] = 1;
		poz = m_poz;
	}
	res.push_back(start);
	total_dist += dist[poz][start];

	std::cout << "całkowity dystans: " << total_dist << std::endl;
	std::cout << "kolejność odwiedzania: " << std::endl;
	for(int a : res){
		std::cout << a << " ";
	}
	std::cout << std::endl;
	for(int a : res){
		std::cout << pos[a].first << " " << pos[a].second << ", ";
	}

	return 0;
}
