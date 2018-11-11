#include <iostream>
#include <string>
#include "conditionalCubeAttack.h"
using namespace std;

int main(){
	ConditionalCubeAttack test;

	cout << "For a fast verification on PC, we suggest to choose 5 rounds!" << endl;
	cout << "Please choose the rounds to be verified: 0-> 5 rounds; 1-> 6rounds; Else->Exit" << endl;

	int cmd,size,rounds;
	cin >> cmd;

	if (cmd == 0){
		size = 16;
		rounds = 5;
		test.setCubeSize(size);
		test.loadCube("MAC-512-CUBE-DATA.txt");
		test.distinguisher(rounds);
	}
	else if (cmd == 1){
		size = 6;
		rounds = 6;
		test.setCubeSize(size);
		test.loadCube("MAC-512-CUBE-DATA.txt");
		test.distinguisher(rounds);
	}
	cout << "Program over!" << endl;
	system("pause");
	return 0;
}