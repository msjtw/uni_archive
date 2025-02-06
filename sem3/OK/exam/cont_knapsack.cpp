#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

struct item{
	int id;
	double size, value;
	double ratio;
};

bool cmp(item l, item r){
	return l.ratio > r.ratio;
}

int main(){
	int n, b_size;
	cin >> n >> b_size;
	vector<item> items;
	items.push_back({});
	for(int i = 1; i < n+1; i++){
		int a;
		cin >> a;
		items.push_back({.id = i, .size = (double)a});
	}
	for(int i = 1; i < n+1; i++){
		int a;
		cin >> a;
		items[i].value = a;
		items[i].ratio = items[i].value / items[i].size;
		cout << "item " << items[i].id << ": " << items[i].value << "/" << items[i].size << " = " << items[i].ratio << endl;
	}

	sort(items.begin(), items.end(), cmp);

	double res = 0;
	int item_it = 0;
	while(item_it < n and items[item_it].size < b_size){
		cout << items[item_it].id << " ";
		res += items[item_it].value;
		b_size -= items[item_it].size;
		item_it++;
	}

	if(item_it < n){
		cout << items[item_it].id << " ";
		res += items[item_it].ratio * b_size;
	}
	cout << endl;

	cout << res << endl;

	return 0;
}
