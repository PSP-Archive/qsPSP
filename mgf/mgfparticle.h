#ifndef _MGFPARTICLE_H_
#define _MGFPARTICLE_H_

#include <mgf.h>
#include <mgfsprite.h>
#include <mgfvector.h>
#include <mgfcolor.h>

struct MGFPARTICLE2D {
	bool active;
	MGFVECTOR2D pos;
	float speed, speedSpeed;
	float rotate, rotateSpeed;
	float angle, angleSpeed;
	float scale, scaleSpeed;
	float lifeMax, life;
	MGFCOLOR color, colorStart, colorEnd;
};

class MGFEMITTER2D {
public:
	MGFEMITTER2D(MGFSPRITE *spr, int blend = MGFBLEND_SUB, int max = 100);
	~MGFEMITTER2D(void);
	
	void Add(MGFPARTICLE2D *p);
	int Count(void) { return parCount; };
	virtual void Update(float delta);
	virtual void Render(MGFVECTOR2D scroll);
	
protected:
	static MGF *mgf;
	MGFPARTICLE2D *list;
	MGFSPRITE *sprite;
	int parMax;
	int parCount;
	int blendMode;
};

#endif
