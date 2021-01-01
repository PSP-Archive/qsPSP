#ifndef _MGFCAMERA_H_
#define _MGFCAMERA_H_

#include <mgf.h>
#include <mgfvector.h>

class MGFCAMERA2D {
public:
	MGFCAMERA2D(float width, float height, float speed);
	~MGFCAMERA2D(void);

	void Reset(void);
	void Set(float x, float y);
	void Set(MGFVECTOR2D pos);
	void Move(float x, float y);
	void Move(MGFVECTOR2D pos);
	void Update(float delta);
	void Begin(void);
	void End(void);

	MGFVECTOR2D pos, dpos;
	float width, height;
	float speed, angle;
	
protected:
	static MGF *mgf;
};

#endif
