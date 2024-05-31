#include <iostream>
#include <windows.h>

int g_cnt = 0;

using namespace std;

void print()
{
	cout << g_cnt++ << ", " << std::hex << print << endl;
	Sleep(1000);
}

int main()
{
	while (true) {
		print();
	}
	return 0;
}