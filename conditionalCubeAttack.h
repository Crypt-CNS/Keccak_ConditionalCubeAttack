#ifndef _CONDITIONAL_CUBE_ATTACK_H
#define _CONDITIONAL_CUBE_ATTACK_H

#include <vector>
#include <string>
using namespace std;

#define N 1600
#define BLOCKSIZE 25
#define ROW 128
#define COL 129
typedef unsigned long long UINT64;
typedef unsigned int UINT32;
typedef unsigned char UINT8;

#define ROL64(a, offset) ((((UINT64)a) << offset) ^ (((UINT64)a) >> (64-offset)))

#define RANDROW() (rand()%5)
#define RANDLANE() (rand()%64)
#define getAccuratePos(x, y, z) ((y * 5 + x) * 64 + z)

#define    MOD5(argValue)    KeccakP1600_Mod5[argValue]
const UINT8 KeccakP1600_Mod5[10] = {
	0, 1, 2, 3, 4, 0, 1, 2, 3, 4
};

#define BIT(x,i) ((x>>i)&0x1)

const UINT8 KeccakP1600_RotationConstants[25] = {
	1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 2, 14, 27, 41, 56, 8, 25, 43, 62, 18, 39, 61, 20, 44
};

const UINT8 KeccakP1600_PiLane[25] = {
	10, 7, 11, 17, 18, 3, 5, 16, 8, 21, 24, 4, 15, 23, 19, 13, 12, 2, 20, 14, 22, 9, 6, 1
};

const UINT64 RC[24] = {
	0x0000000000000001, 0x0000000000008082, 0x800000000000808A, 0x8000000080008000,
	0x000000000000808B, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009,
	0x000000000000008A, 0x0000000000000088, 0x0000000080008009, 0x000000008000000A,
	0x000000008000808B, 0x800000000000008B, 0x8000000000008089, 0x8000000000008003,
	0x8000000000008002, 0x8000000000000080, 0x000000000000800A, 0x800000008000000A,
	0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
};

const UINT64 DI[64] = {
	0x1, 0x2, 0x4, 0x8,
	0x10, 0x20, 0x40, 0x80,
	0x100, 0x200, 0x400, 0x800,
	0x1000, 0x2000, 0x4000, 0x8000,
	0x10000, 0x20000, 0x40000, 0x80000,
	0x100000, 0x200000, 0x400000, 0x800000,
	0x1000000, 0x2000000, 0x4000000, 0x8000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000,
	0x100000000, 0x200000000, 0x400000000, 0x800000000,
	0x1000000000, 0x2000000000, 0x4000000000, 0x8000000000,
	0x10000000000, 0x20000000000, 0x40000000000, 0x80000000000,
	0x100000000000, 0x200000000000, 0x400000000000, 0x800000000000,
	0x1000000000000, 0x2000000000000, 0x4000000000000, 0x8000000000000,
	0x10000000000000, 0x20000000000000, 0x40000000000000, 0x80000000000000,
	0x100000000000000, 0x200000000000000, 0x400000000000000, 0x800000000000000,
	0x1000000000000000, 0x2000000000000000, 0x4000000000000000, 0x8000000000000000
};

struct Cube{
	int position[10];
	int length;
	bool value;
};

class ConditionalCubeAttack{
private:
	Cube cube[32];
	int cubeSize;
	UINT64 state[25];
	bool *value;
	UINT64 key[2];//128-bit key
public:
	ConditionalCubeAttack();
	~ConditionalCubeAttack();
	void setCubeSize(int size);
	void initializeValueByCube(int k);
	void setConstant();
	void getXYZ(int pos);
	void testPropagation(int x, int y, int z,int len);

	void loadCube(string filename);//load cube from file
	void distinguisher(int rounds);//establish distinguisher

	void keccakAlgorithm(int rounds,UINT64 *tag);//Keccak algorithm
	void randomGenerateKey();//generate key

	void outputStateInBit(UINT64 s[]);
	bool checkSum(UINT64 sum[], int len);
};

#endif