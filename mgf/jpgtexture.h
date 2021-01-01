#ifndef _JPGTEXTURE_H_
#define _JPGTEXTURE_H_

#include <mgftexture.h>
#include <psptypes.h>

//! JPEG texture class
class JPGTEXTURE : public MGFTEXTURE {
public:
	JPGTEXTURE(void);
	~JPGTEXTURE(void);

	bool Load(const char *filename, bool swizzle);
};

#endif
