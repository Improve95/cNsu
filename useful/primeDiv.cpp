#include <iostream>
#include <vector>

vector<int> eratosphen(int N) {
	vector<bool> num(N){1};
	vector<int> p;
	num[0]=num[1]=0;
	for(int i = 2; i<N; i++) {
		if(num[i]){
			p.insert(i);
			for(int j = i*i; j<N; j+=i) {
				num[j]=0;
			}
		}
	}
	return p;
}

int main() {
	int x;
	std::cin<<x;
	vector<int>p = eratosphen(x+1);
	vector<int>div(p.size());
	int n=x;
	for(int i = 0; p[i]<=x; i++) {
		while (x%p[i] == 0) {
			x/=p[i];
			div[i]++;
		}
	}
	for(int i = 0; i < p.size(); i++) {
		std::cout<<p[i]<<"^"<<div[i]<<" * ";
	}
	std::cout<<"\n";

	return 0;
}
