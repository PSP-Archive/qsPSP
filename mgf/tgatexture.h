#ifndef _TGATEXTURE_H_
#define _TGATEXTURE_H_

#include <mgftexture.h>
#include <psptypes.h>

class TGATEXTURE : public MGFTEXTURE {
public:
	TGATEXTURE(void);
	~TGATEXTURE(void);

	bool Load(const char *filename, bool swizzle);
};

#endif
