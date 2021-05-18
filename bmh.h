/************************

    BMP Head build lib
    2020.6.30 from N2Dc
	made by Ark_Noya

*************************/


#include "vec.h"
#include <fstream>


struct BMHead {
protected:
	char BM[2] = { 0x42,0x4d };
	char fileSize[4];
	char keepArea0[2] = { 0x00,0x00 };
	char keepArea1[2] = { 0x00,0x00 };
	char imageDataStart[4] = { 0x36,0x00,0x00,0x00 };
	char headFileSize[4] = { 0x28,0x00,0x00,0x00 };
	char swidth[4];
	char sheight[4];
	char deviceLevel[2] = { 0x01,0x00 };
	char scolorBit[2] = { 0x18,0x00 };
	char Nul[24];
public:
	BMHead(int width, int height) {
		int pn = width * height * 3 + 54;
		for (int i = 0; i < 4; i++) {
			fileSize[i] = pn % 256;
			pn /= 256;
		}
		for (int i = 0; i < 4; i++){
			swidth[i] = width % 256;
			width /= 256;
		}
		for (int i = 0; i < 4; i++) {
			sheight[i] = height % 256;
			height /= 256;
		}
		for (int i = 0; i < 24; i++) {
			Nul[i] = 0x00;
		}
	}
	int size() {
		return 54;
	}
	char operator[](int i) const {
		return BM[i];
	}
};

//ofs<<r<<g<<b
ofstream & BMofs(ofstream & ofs, vec v) {
	ofs << (char)((int)round(v.z)) << (char)((int)round(v.y)) << (char)((int)round(v.x));
	return ofs;
}