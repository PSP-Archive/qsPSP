#include "jpgtexture.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
extern "C" {
#include <jpeglib.h>
#include <jerror.h>
}

#include <mgf.h>

JPGTEXTURE::JPGTEXTURE(void) {
}

JPGTEXTURE::~JPGTEXTURE(void) {
}

bool JPGTEXTURE::Load(const char *filename, bool swizzle) {
	MGF *mgf = mgfCreate();
	
	struct jpeg_decompress_struct dinfo;
	struct jpeg_error_mgr jerr;
	dinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&dinfo);
	FILE* inFile = fopen(filename, "rb");
	if (!inFile) {
		jpeg_destroy_decompress(&dinfo);
		mgf->System_Log("JPG error: File '%s' not opened.\n", filename);
		return false;
	}
	jpeg_stdio_src(&dinfo, inFile);
	jpeg_read_header(&dinfo, TRUE);
	int w = dinfo.image_width;
	int h = dinfo.image_height;
	jpeg_start_decompress(&dinfo);

	bits = 4;
	width = GetNextPower2(w);
	height = GetNextPower2(h);
	orig_width = w;
	orig_height = h;
	//image->data = (Color*) memalign(16, image->textureWidth * image->textureHeight * sizeof(Color));
	image = (unsigned char*)malloc(width * height * sizeof(unsigned char) * 4);
	u32* image_u32 = (u32*)image;
	u8* line = (u8*) malloc(width * 3);
	if (!line) {
		jpeg_destroy_decompress(&dinfo);
		mgf->System_Log("JPG error: line malloc.\n");
		return false;
	}
	if(dinfo.jpeg_color_space == JCS_GRAYSCALE) {
		while(dinfo.output_scanline < dinfo.output_height) {
			int y = dinfo.output_scanline;
			jpeg_read_scanlines(&dinfo, &line, 1);
			for(int x = 0; x < w; x++) { // <-- Line 533
				u32 c = line[x];
				image_u32[x + width * y] = c | (c << 8) | (c << 16) | 0xff000000;;
			}
		}
	} else {
		while (dinfo.output_scanline < dinfo.output_height) {
			int y = dinfo.output_scanline;
			jpeg_read_scanlines(&dinfo, &line, 1);
			u8* linePointer = line;
			for (int x = 0; x < w; x++) { // <-- Line 543
				u32 c = *(linePointer++);
				c |= (*(linePointer++)) << 8;
				c |= (*(linePointer++)) << 16;
				image_u32[x + width * y] = c | 0xff000000;
			}
		}
	}
	jpeg_finish_decompress(&dinfo);
	jpeg_destroy_decompress(&dinfo);
	free(line);
	
	fclose(inFile);
	
	if(swizzle) Swizzle();

	mgf->Release();

	return true;
}

