#include <iostream>
#include <random>
#include <set>
#include <ranges>

#define MAX_N 1000
#define MAX_CORD 10000

int main() {
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> distN(1, MAX_N), distCord(1, MAX_CORD);

	int n;
	//std::cout << "Podaj liczbe wierzcholkow (0 jezeli generator ma wybrac losowa liczbe wierzcholkow): ";
	std::cin >> n;
	/*while (n < 0) {
		std::cout << "Podano liczbe mniejsza od 0, sprobuj ponownie: ";
		std::cin >> n;
	}*/
	if (n == 0) {
		n = distN(rng);
	}

	std::set<std::pair<int, int>> points;
	while (points.size() < n) {
		int x = distCord(rng), y = distCord(rng);
		points.insert(std::make_pair(x, y));
	}
	for (const auto& [i, point] : points | std::views::enumerate) {
		std::cout << (i + 1) << ' ' << point.first << ' ' << point.second << std::endl; 
	}
}
