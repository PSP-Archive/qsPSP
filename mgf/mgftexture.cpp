#include <mgftexture.h>
#include <jpgtexture.h>
#include <pngtexture.h>
#include <tgatexture.h>


#include <stdio.h>
#include <string.h>

#include <mgf.h>

MGFTEXTURE::MGFTEXTURE(void) {
	swizzled = false;
	bits = 0;
	width = 0;
	height = 0;
	image = 0;
	
	next = 0;
}

MGFTEXTURE::~MGFTEXTURE(void) {	
	MGF *mgf = mgfCreate();
	if(image) {
		free(image);
		image = 0;
		}
	mgf->Release();
}

void MGFTEXTURE::Swizzle(void) {
	if(!image) return;
	
	unsigned int i,j;
	unsigned int rowblocks = (width*sizeof(u32) / 16);
	long size = width * height * bits;
	 
	unsigned char* out = (unsigned char*)malloc(size*sizeof(unsigned char));

	for(j = 0; j < height; ++j) {
		for(i = 0; i < width*sizeof(u32); ++i) {
			unsigned int blockx = i / 16;
			unsigned int blocky = j / 8;
	 
			unsigned int x = (i - blockx*16);
			unsigned int y = (j - blocky*8);
			unsigned int block_index = blockx + ((blocky) * rowblocks);
			unsigned int block_address = block_index * 16 * 8;
	 
			out[block_address + x + y * 16] = image[i + j * width * sizeof(u32)];
		}
	}
	memcpy(image, out, size);
	swizzled = !swizzled;
	free(out);
}

int GetNextPower2(int width) {
	int b = width;
	int n;
	for (n = 0; b != 0; n++) b >>= 1;
	b = 1 << n;
	if (b == 2 * width) b >>= 1;
	return b;
}
