#include "conditionalCubeAttack.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
#include <string>
using namespace std;

//generate a 64-bit value
UINT64 getRandUINT64(){
	UINT64 t1 = 0, t2 = 0;
	t1 = (rand() << 16) | rand();
	t2 = (rand() << 16) | rand();
	t1 = t1 << 32;
	return t1 | t2;
}

void ConditionalCubeAttack::setConstant(){
	for (int i = 128; i < 1600 - 1024; i++){
		value[i] = rand() & 0x1;
	}
	for (int i = 1600 - 1024; i < 1600; i++){
		value[i] = 0;
	}
	//v[3][1][7] = 0;
	//v[3][2][45] = 0;
	value[getAccuratePos(2, 0, 7)] = value[getAccuratePos(2, 1, 7)];
	value[getAccuratePos(3, 1, 7)] = value[getAccuratePos(4, 0, 6)];

	value[getAccuratePos(2, 0, 45)] = value[getAccuratePos(2, 1, 45)];
	value[getAccuratePos(4, 0, 44)] = 0;

	//v[3][2][46]=0
	value[getAccuratePos(2, 0, 46)] = value[getAccuratePos(2, 1, 46)];
	value[getAccuratePos(4, 0, 45)] = 0;

	//v[3][1][21]=0 (v[3][4][21]=0)
	value[getAccuratePos(2, 0, 21)] = value[getAccuratePos(2, 1, 21)];
	value[getAccuratePos(3, 1, 21)] = 0;
	value[getAccuratePos(4, 0, 20)] = 0;

	//v[3][1][22]=0
	value[getAccuratePos(2, 0, 22)] = value[getAccuratePos(2, 1, 22)];
	value[getAccuratePos(3, 1, 22)] = value[getAccuratePos(4, 0, 21)];

	//v[3][2][4]=0
	value[getAccuratePos(2, 0, 4)] = value[getAccuratePos(2, 1, 4)];
	value[getAccuratePos(4, 0, 3)] = 0;

	//v[3][1][32]=0
	value[getAccuratePos(2, 0, 32)] = value[getAccuratePos(2, 1, 32)];
	value[getAccuratePos(3, 1, 32)] = value[getAccuratePos(4, 0, 31)];

	//v[3][1][49]=0
	value[getAccuratePos(2, 0, 49)] = value[getAccuratePos(2, 1, 49)];
	value[getAccuratePos(3, 1, 49)] = value[getAccuratePos(4, 0, 48)];

	//v[3][1][59]=0
	value[getAccuratePos(2, 0, 59)] = value[getAccuratePos(2, 1, 59)];
	value[getAccuratePos(3, 1, 59)] = value[getAccuratePos(4, 0, 58)];

	//v[3][1][6]=0 v[3][2][44]=0
	value[getAccuratePos(2, 0, 6)] = value[getAccuratePos(2, 1, 6)];
	value[getAccuratePos(3, 1, 6)] = value[getAccuratePos(4, 0, 5)];

	value[getAccuratePos(2, 0, 44)] = value[getAccuratePos(2, 1, 44)];
	value[getAccuratePos(4, 0, 43)] = 0;

	//v[3][3][20]=0,v[3][4][21]=0(satisfied at v[3][1][21]=0)
	value[getAccuratePos(2, 0, 20)] = value[getAccuratePos(2, 1, 20)];
	value[getAccuratePos(4, 0, 19)] = 0;

	//v[3][1][53]=0
	value[getAccuratePos(2, 0, 53)] = value[getAccuratePos(2, 1, 53)];
	value[getAccuratePos(3, 1, 53)] = value[getAccuratePos(4, 0, 52)];
}

/*
state[x,y,z]
z=pos%64;
t=pos/64;
x=t%5;
y=t/5;
*/
void ConditionalCubeAttack::initializeValueByCube(int k){
	//update cube
	for (int i = 0; i < cubeSize; i++){
		for (int j = 0; j < cube[i].length; j++){
			value[cube[i].position[j]] = cube[i].value;
		}
	}

	//v[3][0][61] = v0
	//v[3[[1][61] = v1
	//v[2][0][26]=v[2][1][26]=v1+v0
	value[getAccuratePos(2, 0, 26)] = value[getAccuratePos(3, 0, 61)] ^ value[getAccuratePos(3, 1, 61)];
	value[getAccuratePos(2, 1, 26)] = value[getAccuratePos(2, 0, 26)];
}

ConditionalCubeAttack::ConditionalCubeAttack(){
	value = new bool[N];
}

ConditionalCubeAttack::~ConditionalCubeAttack(){
	delete[]value;
}

void ConditionalCubeAttack::setCubeSize(int size){
	cubeSize = size;
	cout << "CubeSize:" << cubeSize << endl;
}

void ConditionalCubeAttack::getXYZ(int pos){
	int x, y, z;
	z = pos % 64;
	y = (pos / 64) / 5;
	x = (pos / 64) % 5;

	cout <<"A["<<x << "][" << y << "][" << z<<"]";
}

void ConditionalCubeAttack::testPropagation(int x, int y, int z, int len){
	UINT64 tag[25];
	for (int i = 0; i < N; i++){
		value[i] = false;
	}
	key[0] = 0;
	key[1] = 0;

	value[getAccuratePos(x, y, z)] = 1;
	if (len>0)
		value[getAccuratePos(x, y+1, z)] = 1;

	keccakAlgorithm(1, tag);
}

//construct distinguisher
void ConditionalCubeAttack::distinguisher(int rounds){
	UINT64 *tag;
	UINT64 *sum;
	tag = new UINT64[25];
	sum = new UINT64[25];

	srand(time(NULL));
	//key-dependent bit condition
	//A[2][0][4]=k5+k69+A[0][1][5]+A[2][1][4]+1
	//A[2][0][59]=k60+A[0][1][60]+A[2][1][59]+1
	//k15+A[0][1][15]+A[2][0][14]+A[2][1][14]=1\

	int z = 0;
	int c = 0;
	int aPos = getAccuratePos(0, 1, 5);
	int bPos = getAccuratePos(0, 1, 60);
	int cPos = getAccuratePos(0, 1, 15);

	bool flag = true;
	UINT32 end;
	if (cubeSize == 32){
		end = 0xffffffff;
	}
	else{
		end= (1 << cubeSize)-1;//cubeSize
	}
	cout << "END:" << hex << end << endl;

	for (int i = 0; i < 64; i++){//Test 64 times
		randomGenerateKey();
		cout << "key:" << hex << key[0] << " " << key[1] << endl;

		flag = true;
		setConstant();
		for (unsigned int k = 0; k<8; k++){
			flag = true;

			value[getAccuratePos(0, 1, 5)] = BIT(k, 0);//Guess key bit
			value[getAccuratePos(0, 1, 60)] = BIT(k, 1);
			value[getAccuratePos(0, 1, 15)] = BIT(k, 2);

			for (int index = 0; index < 25; index++){
				sum[index] = 0;
			}

			for (unsigned int v = 0; flag; v++){
				for (int cubeIndex = 0; cubeIndex < cubeSize; cubeIndex++){//update the value of cube
					cube[cubeIndex].value = BIT(v, cubeIndex);
				}

				initializeValueByCube(k);//Guess 
				keccakAlgorithm(rounds, tag);

				for (int index = 0; index < 25; index++){
					sum[index] = tag[index] ^ sum[index];
				}

				if (v == end){
					flag = false;//break
				}
			}

			if (checkSum(sum,25)){
				cout << hex<<k<<" is RIGHT!" << endl;
			}
			/*else{
				cout << hex << k << " is WRONG!" << endl;
			}*/
		}
		cout << endl;
	}
	delete[]tag;
	delete[]sum;
}

//Keccak
void ConditionalCubeAttack::keccakAlgorithm(int rounds, UINT64 *tag){
	UINT64 BC[5];
	UINT64 temp = 0;

	state[0] = key[0];
	state[1] = key[1];

	//Update state[2], state[3], ..., state[24] with value[128~1599]
	UINT64 tmp1 = 0,tmp2=0;
	for (int i = 128; i < N; i += 64){
		tmp2 = 0;
		for (int j = 0; j < 64; j++){
			if (value[i + j]){
				tmp2 = tmp2 | DI[j];
			}
		}

		state[i / 64] = tmp2;
	}
	
	for (int i = 0; i < rounds; i++){
		//Theta
		for (int x = 0; x < 5; x++){
			BC[x] = state[x] ^ state[5 + x] ^ state[10 + x] ^ state[15 + x] ^ state[20 + x];
		}

		for (int x = 0; x < 5; x++){
			temp = BC[MOD5(x + 4)] ^ ROL64(BC[MOD5(x + 1)], 1);
			for (int y = 0; y < 25; y += 5){
				state[y + x] ^= temp;
			}
		}

		//Rho Pi
		temp = state[1];
		for (int x = 0; x < 24; x++){
			BC[0] = state[KeccakP1600_PiLane[x]];
			state[KeccakP1600_PiLane[x]] = ROL64(temp, KeccakP1600_RotationConstants[x]);
			temp = BC[0];
		}

		//Chi
		for (int y = 0; y < 25; y += 5){
			BC[0] = state[y + 0];
			BC[1] = state[y + 1];
			BC[2] = state[y + 2];
			BC[3] = state[y + 3];
			BC[4] = state[y + 4];

			for (int x = 0; x < 5; ++x){
				state[y + x] = BC[x] ^ ((~BC[MOD5(x + 1)]) & BC[MOD5(x + 2)]);
			}
		}

		//Iota
		state[0] ^= RC[i];
	}
	for (int i = 0; i < 25; i++){
		tag[i] = state[i];
	}
}

//生成随机密钥
void ConditionalCubeAttack::randomGenerateKey(){
	key[0] = getRandUINT64();
	key[1] = getRandUINT64();
}

//加载cube变量
void ConditionalCubeAttack::loadCube(string filename){
	ifstream inFile(filename);
	int length = 0;
	int position;
	int x, y, z;
	for (int i = 0; i < cubeSize; i++){
		inFile >> length;
		cube[i].length = length;
		for (int j = 0; j < length; j++){
			inFile >> x>>y>>z;
			position = getAccuratePos(x, y, z);
			cube[i].position[j] = position;
		}
	}
	inFile.close();

	cout << "Cube: " << endl;
	for (int i = 0; i < cubeSize; i++){
		cout << i << ": ";
		for (int j = 0; j < cube[i].length; j++){
			getXYZ(cube[i].position[j]);
			cout<< " ";
		}
		cout << endl;
	}
}

bool ConditionalCubeAttack::checkSum(UINT64 sum[], int len){
	for (int i = 0; i < len; i++){
		if (sum[i] != 0){
			return false;
		}
	}
	return true;
}

void ConditionalCubeAttack::outputStateInBit(UINT64 s[]){
	for (int i = 0; i < 25; i++){
		if (i != 0 && i % 5 == 0){
			cout << endl;
		}
		cout << hex << s[i] << " ";
	}
}
