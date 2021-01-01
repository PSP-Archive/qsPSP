#ifndef _PNGTEXTURE_H_
#define _PNGTEXTURE_H_

#include <mgftexture.h>
#include <psptypes.h>

class PNGTEXTURE : public MGFTEXTURE {
public:
	PNGTEXTURE(void);
	~PNGTEXTURE(void);

	bool Load(const char *filename, bool swizzle);
};

#endif
