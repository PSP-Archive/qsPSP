#ifndef _MGFTEXTURE_H_
#define _MGFTEXTURE_H_

#include <malloc.h>
#include <psptypes.h>

class MGFTEXTURE {
public:
	MGFTEXTURE(void);
	~MGFTEXTURE(void);

	void Swizzle(void);
	
	int Width(bool orig = false) { if(orig) return orig_width; else return width; }
	int Height(bool orig = false) { if(orig) return orig_height; else return height; }
	int Bits(void) { return bits; }
	unsigned char* Image(void) { return image; }
	bool Swizzled(void) { return swizzled; }
	
	virtual bool Load(const char *filename, bool swizzle) { return false; };
	void Remove(void);
	
	MGFTEXTURE *next;
	
protected:
	bool swizzled;
	unsigned int bits;
	unsigned int width, orig_width;
	unsigned int height, orig_height;
	unsigned char *image;
};

int GetNextPower2(int width);

#endif
