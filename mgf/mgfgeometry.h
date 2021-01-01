#ifndef _MGFGEOMETRY_H_
#define _MGFGEOMETRY_H_

struct MGFVERTEX {
	float u, v;
	unsigned int color;
	float x, y, z;
};

struct MGFVERTEX3D{
	float u, v;
	unsigned int color;
	float nx, ny, nz;
	float x, y, z;
};

struct MGFLINE2D {
	MGFLINE2D(void) {
		Reset();
	}
	void Reset(void) {
		for(int i = 0; i < 2; i++) {
			v[i].u = v[i].v = 0;
			v[i].color = 0xFFFFFFFF;
			v[i].x = v[i].y = v[i].z = 0;
		}
	}
	void SetVertex(float x1, float y1, float x2, float y2) {		
		v[0].x = x1;
		v[0].y = y1;
		v[1].x = x2;
		v[1].y = y2;
	}
	
	MGFVERTEX v[2];
};

struct MGFTRIANGLE2D {
	MGFTRIANGLE2D(void) {
		Reset();
	}
	void Reset(void) {
		for(int i = 0; i < 3; i++) {
			v[i].u = v[i].v = 0;
			v[i].color = 0xFFFFFFFF;
			v[i].x = v[i].y = v[i].z = 0;
		}
	}
	void SetVertex(float x1, float y1, float x2, float y2, float x3, float y3) {		
		v[0].x = x1;
		v[0].y = y1;
		v[1].x = x2;
		v[1].y = y2;
		v[2].x = x3;
		v[2].y = y3;
	}
	void SetTexCoord(float u1, float v1, float u2, float v2, float u3, float v3) {
		v[0].u = u1;
		v[0].v = v1;
		v[1].u = u2;
		v[1].v = v2;
		v[2].u = u3;
		v[2].v = v3;
	}
	void SetColor(int color) {
		for(int i = 0; i < 3; i++) {
			v[i].color = color;
		}
	}

	MGFVERTEX v[3];
};

struct MGFQUAD {
	MGFQUAD(void) {
		Reset();
	}
	void Reset(void) {
		for(int i = 0; i < 4; i++) {
			v[i].u = v[i].v = 0;
			v[i].color = 0xFFFFFFFF;
			v[i].x = v[i].y = v[i].z = 0;
		}
	}
	void SetVertex(float width, float height, bool center = true) {		
		Width = width;
		Height = height;
		if(center) {
			width = width / 2.f;
			height = height / 2.f;
		
			v[0].x = -width;
			v[0].y = -height;
			v[1].x = -width;
			v[1].y = height;
			v[2].x = width;
			v[2].y = height;
			v[3].x = width;
			v[3].y = -height;
		} else {
			v[0].x = 0;
			v[0].y = 0;
			v[1].x = 0;
			v[1].y = height;
			v[2].x = width;
			v[2].y = height;
			v[3].x = width;
			v[3].y = 0;
		}
	}
	void SetTexCoord(float x, float y, float width, float height) {
		v[0].u = x;
		v[0].v = y;
		v[1].u = x;
		v[1].v = y + height;
		v[2].u = x + width;
		v[2].v = y + height;
		v[3].u = x + width;
		v[3].v = y;
	}
	void SetColor(int color) {
		for(int i = 0; i < 4; i++) {
			v[i].color = color;
		}
	}

	float Width, Height;
	MGFVERTEX v[4];
};

#endif
