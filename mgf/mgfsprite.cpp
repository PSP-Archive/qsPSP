#include <mgfsprite.h>

#include <mgf.h>

MGFSPRITE::MGFSPRITE(MGFTEXTURE *tex, float width, float height, float fps, int start, int end, bool center) {
	Tex = tex;
	Width = width;
	Height = height;
	
	isCenter = center;
	isPlaying = true;
	
	if(fps > 0.f) Speed = 1.f / fps;
	else {
		Speed = 0.f;
		isPlaying = false;
	}
	Time = 0.f;
	Start = start;
	End = end;
	Delta = 1;

	if(Tex) {
		CountX = (int)(Tex->Width() / Width);
		CountY = (int)(Tex->Height() / Height);
	} else {
		CountX = 1;
		CountY = 1;
	}
	Mode = 0;
	Mode = MGFANIM_FWD | MGFANIM_LOOP;
	SetFrame(Start);
	Time = 0.f;
	
	Quad.SetVertex(Width, Height, isCenter);
	SetColor(0xFFFFFFFF);
}

MGFSPRITE::~MGFSPRITE(void) {
	//delete Quad;
}
	
void MGFSPRITE::SetColor(int _color) {
	Color = _color;
	//Quad.SetColor(color);
}

void MGFSPRITE::Update(float delta) {
	if(!isPlaying) return;
	
	Time += delta;
	
	while(Time >= Speed) {
		Time -= Speed;
		
		if(Current + Delta > End || Current + Delta < Start) {
			if(Mode & MGFANIM_LOOP) {
				if(Mode & MGFANIM_PINPONG) {
					Delta = -Delta;
				} else {
					if(Mode & MGFANIM_FWD)
						Current = Start;
					else if(Mode & MGFANIM_REV)
						Current = End;
				}
			} else {
				isPlaying = false;
			}
		}
		if(isPlaying) SetFrame(Current + Delta);
	}

}

void MGFSPRITE::Render(float _x, float _y, float _angle, float _scale) {
	//if(/*Slice && */_x < 0 || _y < 0 || _x > SCREEN_WIDTH || _y > SCREEN_HEIGHT) return;
	
	sceGumPushMatrix();
	
	ScePspFVector3 move = { _x, _y, 0.f };
	ScePspFVector3 scale  = { _scale, _scale, _scale };

	sceGumTranslate(&move);
	sceGumRotateZ(_angle);
	sceGumScale(&scale);

	if(Tex) {
		//sceGuEnable(GU_TEXTURE_2D);
		sceGuTexImage(0, Tex->Width(), Tex->Height(), Tex->Width(), Tex->Image());
	}
	
	MGFQUAD *vtmp = (MGFQUAD*)sceGuGetMemory(sizeof(MGFQUAD));
  memcpy(vtmp, &Quad, sizeof(MGFQUAD));
	vtmp->SetColor(Color);
	
	sceGumDrawArray(GU_TRIANGLE_FAN, MGF_VERTEX_3D, 4, 0, vtmp->v);

	sceGumPopMatrix();
}

void MGFSPRITE::SetFrame(int num) {
	if(CountX == 0 || num > CountX * CountY) return;	
	
	int indY = (int)(num / CountX);
	int indX = (int)(num - indY * CountX);

	float tw = 1.f / Tex->Width() * Width;
	float tx = tw * indX;
	float th = 1.f / Tex->Height() * Height;
	float ty = /*1.f / Tex->Height() - */th * indY;
	
	Quad.SetTexCoord(tx, ty, tw, th);
	Current = num;
}

int MGFSPRITE::GetFrame(void) {
	return Current;
}

float MGFSPRITE::Get(void) {
	return Speed - Time;
}

void MGFSPRITE::SetMode(int mode) {
	Mode = mode;
	if(Mode & MGFANIM_FWD) {
		Delta = 1;
		SetFrame(Start);
	} else if(Mode & MGFANIM_REV) {
		Delta = -1;
		SetFrame(End);
	}
}

void MGFSPRITE::SetVertex(float width, float height, bool center) {
	Quad.SetVertex(width, height, center);
}

void MGFSPRITE::SetTexCoord(float x, float y, float width, float height) {
	Quad.SetTexCoord(x, y, width, height);
}

void MGFSPRITE::Play(bool play) {
	if(Speed <= 0.f) return;
	isPlaying = play;
	if(isPlaying) SetMode(Mode);
}

