#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <png.h>
#include "pngtexture.h"

#include <mgf.h>


void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
	MGF *mgf = mgfCreate();
	mgf->System_Log(warning_msg);
	mgf->Release();
}

PNGTEXTURE::PNGTEXTURE(void) {
}

PNGTEXTURE::~PNGTEXTURE(void) {
}

bool PNGTEXTURE::Load(const char *filename, bool swizzle) {
	MGF *mgf = mgfCreate();
	
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 w, h;
	int bit_depth, color_type, interlace_type, x, y;
	u32* line;
	u32* image_u32;
	FILE *fp;

	if((fp = fopen(filename, "rb")) == NULL) {
		mgf->System_Log("PNG error: File '%s' not opened.\n", filename);
		mgf->Release();
		return false;
	}
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(png_ptr == NULL) {
		fclose(fp);
		mgf->System_Log("PNG error: png_create_read_struct\n");
		mgf->Release();
		return false;
	}
	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);
	info_ptr = png_create_info_struct(png_ptr);
	if(info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		mgf->System_Log("PNG error: png_create_info_struct\n");
		mgf->Release();
		return false;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
	if(w > 512 || h > 512) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		mgf->System_Log("PNG error: png_get_IHDR\n");
		mgf->Release();
		return false;
	}
	bits = 4;
	width = GetNextPower2(w);
	height = GetNextPower2(h);
	orig_width = w;
	orig_height = h;
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_gray_1_2_4_to_8(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png_ptr);
	png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
	//image = (unsigned char*)memalign(16, width * height * sizeof(unsigned char) * 4);
	image = (unsigned char*)malloc(width * height * sizeof(unsigned char) * 4);
	image_u32 = (u32*)image;
	if (!image) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		mgf->System_Log("PNG error: memalign\n");
		mgf->Release();
		return false;
	}
	line = (u32*) malloc(width * 4);
	if (!line) {
		free(image);
		fclose(fp);
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		mgf->System_Log("PNG error: malloc\n");
		mgf->Release();
		return false;
	}
	for (y = 0; y < h; y++) {
		png_read_row(png_ptr, (u8*) line, png_bytep_NULL);
		for (x = 0; x < w; x++) {
			u32 color = line[x];
			image_u32[x + y * width] =  color;
		}
	}
	free(line);
	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
	fclose(fp);
	
	if(swizzle) Swizzle();

	mgf->Release();

	return true;
}

