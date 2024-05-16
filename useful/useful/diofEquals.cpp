#include <iostream>

int ext_gcd(int a, int b, int& x, int& y) {
	if (a == 0) {
		x = 0;
		y = 1;
		return b;
	}
	int x1, y1;
	int g = ext_gcd(b%a, a, x1, y1);
	x = y1 - (b/a)*x1;
	y = x1;
	return g;
}

int main() {
	int a, b, c;
	std::cin>>a>>b>>c;
	int x, y;
	int g = ext_gcd(a, b, x, y);
	if (c%g != 0) {
		std::cout<<"no solutions";
		return 0;
	}
	x*=c/g;
	y*=c/g;
	std::cout<<"x = "<<x<<" + k*("<<b<<"/"<<g*g<<")\n";
	std::cout<<"y = "<<y<<" - k*("<<a<<"/"<<g*g<<")\n";
	return 0;
}
