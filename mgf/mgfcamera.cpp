#include <mgfcamera.h>

MGF *MGFCAMERA2D::mgf = 0;

MGFCAMERA2D::MGFCAMERA2D(float _width, float _height, float _speed) {
	mgf = mgfCreate();
	width = _width;
	height = _height;
	speed = _speed;
	Reset();
	
	mgf->System_Log("Camera: Width=%.2f, Height=%.2f\n", width, height);
}

MGFCAMERA2D::~MGFCAMERA2D(void) {
	mgf->Release();
}

void MGFCAMERA2D::Reset(void) {
  angle = 0.f;
	pos.Set(0.f, 0.f);
	dpos.Set(0.f, 0.f);
}

void MGFCAMERA2D::Set(float _x, float _y) {
	Set(MGFVECTOR2D(_x, _y));
}

void MGFCAMERA2D::Set(MGFVECTOR2D _pos) {
	dpos = _pos;
	if(dpos.x < 0.f) dpos.x = 0.f;
	if(dpos.x > width - SCREEN_WIDTH) dpos.x = width - SCREEN_WIDTH;
	if(dpos.y < 0.f) dpos.y = 0.f;
	if(dpos.y > height - SCREEN_HEIGHT) dpos.y = height - SCREEN_HEIGHT;
	
	angle = pos.AngleTo(dpos);
}

void MGFCAMERA2D::Move(float _x, float _y) {
	Move(MGFVECTOR2D(_x, _y));
}

void MGFCAMERA2D::Move(MGFVECTOR2D _pos) {
	dpos.x += _pos.x;
	dpos.y += _pos.y;
	Set(dpos);
}

void MGFCAMERA2D::Update(float delta) {
	float dist = pos.DistanceTo(dpos);
	if(dist > 1.f) {
		dist = dist * speed * delta;
		pos.Move(dist, angle);
	} else {
		pos = dpos;
	}

	if(pos.x < 0.f) {
    pos.x = 0.f;
    dpos.x = 0.f;
	}
	if(pos.x > width) {
    pos.x = width;
    dpos.x = width;
	}
	if(pos.y < 0.f) {
    pos.y = 0.f;
    dpos.y = 0.f;
	}
	if(pos.y > height) {
    pos.y = height;
    dpos.y = height;
	}
}

void MGFCAMERA2D::Begin(void) {
	sceGumPushMatrix();
	ScePspFVector3 move = { pos.x, pos.y, 0.f };
	sceGumTranslate(&move);
}

void MGFCAMERA2D::End(void) {
	sceGumPopMatrix();
}
