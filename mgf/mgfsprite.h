#ifndef _MGFSPRITE_H_
#define _MGFSPRITE_H_

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>

#include <mgftexture.h>
#include <mgfgeometry.h>

enum MGFANIM_MODE {
	MGFANIM_FWD = 2,
	MGFANIM_REV = 4,
	MGFANIM_LOOP = 16,
	MGFANIM_PINPONG = 32
};

class MGFSPRITE {
public:
	MGFSPRITE(MGFTEXTURE *tex, float width, float height, float fps, int start, int end, bool center = true);
	~MGFSPRITE(void);
	
	void SetColor(int color);
	void Update(float delta);
	void Render(float x, float y, float angle = 0.f, float scale = 1.f);
	void SetFrame(int num);
	int GetFrame(void);
	float Get(void);
	void SetMode(int mode);
	void SetVertex(float width, float height, bool center = true);
	void SetTexCoord(float x, float y, float width, float height);
	void Play(bool play);
	
protected:
	MGFTEXTURE *Tex;
	float Width, Height;
	bool isCenter;
	bool isPlaying;
	int Color;
	float Speed;
	int Current, Start, End, Delta, CountX, CountY;
	float Time;
	int Mode;
	MGFQUAD Quad;
};

#endif
