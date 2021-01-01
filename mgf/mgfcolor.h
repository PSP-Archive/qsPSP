#ifndef _MGFCOLOR_H_
#define _MGFCOLOR_H_

struct MGFCOLOR {
	MGFCOLOR(float _r, float _g, float _b, float _a) {
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	};
	MGFCOLOR(int color) {
		SetColor(color);
	};
	MGFCOLOR(void) {
		r = g = b = a = 0;
	}
	
	MGFCOLOR operator- (const MGFCOLOR &c) const {
		return MGFCOLOR(r - c.r, g - c.g, b - c.b, a - c.a);
	}
	MGFCOLOR operator+ (const MGFCOLOR &c) const {
		return MGFCOLOR(r + c.r, g + c.g, b + c.b, a + c.a);
	}
	MGFCOLOR operator* (const MGFCOLOR &c) const {
		return MGFCOLOR(r * c.r, g * c.g, b * c.b, a * c.a);
	}
	MGFCOLOR& operator-= (const MGFCOLOR &c)	 {
		r -= c.r; 
		g -= c.g;
		b -= c.b;
		a -= c.a;
		return *this;
	}
	MGFCOLOR& operator+= (const MGFCOLOR &c)	 {
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;
		return *this;
	}
	bool operator== (const MGFCOLOR &c) const {
		return (r == c.r && g == c.g && b == c.b && a == c.a);
	}
	bool operator!= (const MGFCOLOR &c) const {
		return (r != c.r || g != c.g || b != c.b || a != c.a);
	}

	MGFCOLOR operator/ (const float f) const {
		return MGFCOLOR( r / f, g / f, b / f, a / f);
	}
	MGFCOLOR operator* (const float f) const {
		return MGFCOLOR(r * f, g * f, b * f, a * f);
	}
	MGFCOLOR& operator*= (const float f) {
		r *= f;
		g *= f;
		b *= f;
		a *= f;
		return *this;
	}
	
	void SetColor(int color) {
	  a = (color >> 24) / 255.f; 
		r = ((color >> 16) & 0xFF) / 255.f; 
		g = ((color >> 8) & 0xFF) / 255.f; 
		b = (color & 0xFF) / 255.f;
	}
	int GetColor(void) const {
		return ((int)(a * 255.f) << 24) + ((int)(r * 255.f) << 16) + ((int)(g * 255.f) << 8) + (int)(b * 255.f);
	}
	
	float r, g, b, a;
};

#endif
