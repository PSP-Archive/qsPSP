#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "tgatexture.h"

TGATEXTURE::TGATEXTURE(void) {
}

TGATEXTURE::~TGATEXTURE(void) {
}

bool TGATEXTURE::Load(const char *filename, bool swizzle) {
	FILE *pFile = 0;

	unsigned char tempColor;
	unsigned char bitCount;
	int colorMode;
	long tgaSize;
	unsigned char unCompressHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	unsigned char tgaHeader[12];
	unsigned char header[6];

	pFile = fopen(filename, "rb");
	
	if(!pFile) {
		return false;
	}
	
	fread(&tgaHeader, 1, sizeof(tgaHeader), pFile);
	
	if(memcmp(unCompressHeader, tgaHeader, sizeof(unCompressHeader)) != 0) {
		fclose(pFile);
		return false;
	}
	
	fread(header, 1, sizeof(header), pFile);

	orig_width  = header[1] * 256 + header[0];
	orig_height = header[3] * 256 + header[2];
	width = GetNextPower2(orig_width);
	height = GetNextPower2(orig_height);
	bitCount	= header[4];

	colorMode	= bitCount / 8;
	tgaSize		= width * height * colorMode;

	image = (unsigned char*)malloc(tgaSize*sizeof(unsigned char));

	fread(image, sizeof(unsigned char), tgaSize, pFile);

	for(long index = 0; index < tgaSize; index += colorMode) {
		tempColor = image[index];
		image[index] = image[index + 2];
		image[index + 2] = tempColor;
	}
	
	bits = colorMode;

	fclose(pFile);
	
	if(swizzle) Swizzle();

	return true;
}

