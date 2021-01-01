#ifndef _MGFVECTOR_H_
#define _MGFVECTOR_H_

#define _USE_MATH_DEFINES
#include <math.h>

class MGFVECTOR {
public:
	float x, y, z;
};

class MGFVECTOR2D : public MGFVECTOR {
public:
	MGFVECTOR2D() {
		this->x = 0.f;
		this->y = 0.f;
	}

	MGFVECTOR2D(float x, float y) {
		this->x = x;
		this->y = y;
	}

	MGFVECTOR2D(const MGFVECTOR2D &v) {
		this->x = v.x;
		this->y = v.y;
	}
	
	MGFVECTOR2D	operator- (const MGFVECTOR2D &v) { return MGFVECTOR2D(x-v.x, y-v.y); }
	MGFVECTOR2D	operator+ (const MGFVECTOR2D &v) { return MGFVECTOR2D(x+v.x, y+v.y); }
	MGFVECTOR2D	operator* (float scalar) { return MGFVECTOR2D(x*scalar, y*scalar); }
	MGFVECTOR2D&	operator-= (const MGFVECTOR2D &v) { x-=v.x; y-=v.y; return *this; }
	MGFVECTOR2D&	operator+= (const MGFVECTOR2D &v) { x+=v.x; y+=v.y; return *this; }
	MGFVECTOR2D&	operator*= (float scalar) { x*=scalar; y*=scalar; return *this; }
	MGFVECTOR2D	operator- () { return MGFVECTOR2D(-x, -y); }
	bool		operator== (const MGFVECTOR2D &v) { return (x==v.x && y==v.y); }
	bool		operator!= (const MGFVECTOR2D &v) { return (x!=v.x || y!=v.y); }

	void Set(float x, float y) {
		this->x = x;
		this->y = y;
	}
	
	void Add(float x, float y) {
    this->x += x;
    this->y += y;
  }

	void Move(float dist, float angle) {
		//angle = M_PI / 180 * angle;
		this->x = this->x - sin(angle) * dist;
		this->y = this->y - cos(angle) * dist;
	}
	
	void MoveTo(float dist, MGFVECTOR2D to) {
    float dst = this->DistanceTo(to);
    if(dst > dist) {
      this->Move(dist, this->AngleTo(to));      
    } else {
      this->x = to.x; this->y = to.y;
    }
  }

	void Rotate(MGFVECTOR2D center, float angle) {
		//angle = M_PI / 180 * angle;
		this->x = (this->x - center.x) * cos(angle) - (this->y - center.y) * sin(angle) + center.x;
		this->y = (this->y - center.y) * cos(angle) + (this->x - center.x) * sin(angle) + center.y;
	}

	float AngleTo(MGFVECTOR2D to) {
		if(x == to.x && y == to.y) return -1.f;

		float angle = atan((to.y - y) / (to.x - x)); // * 180 / M_PI;
		if(to.x >= x)
			return M_PI - angle + M_PI_2;
		else
			return M_PI - angle - M_PI_2;
	}

	float Angle(void) {
		if(x == 0 && y == 0) return -1;

		float angle = atan(y / x); // * 180 / M_PI;
		if(0 <= x)
			return M_PI - angle + M_PI_2;
		else
			return M_PI - angle - M_PI_2;		
	}

	float DistanceTo(MGFVECTOR2D to) {
		return sqrt((to.x - this->x) * (to.x - this->x) + (to.y - this->y) * (to.y - this->y));
	}

	float Distance(void) {
		return sqrt(this->x * this->x + this->y * this->y);
	}

	MGFVECTOR2D FromLine(MGFVECTOR2D p1, MGFVECTOR2D p2) {
		float d, d1, d2;
  
		d1 = this->DistanceTo(p1);
		d2 = this->DistanceTo(p2);
  
		d = d1 / ((d1 + d2) / 100);
		d = p1.DistanceTo(p2) / 100 * d;
		p1.Move(d, p1.AngleTo(p2));

		return p1;
	}
	
	void f(MGFVECTOR2D p) {
		this->x = (this->x + p.x) / 2;
		this->y = (this->y + p.y) / 2;
	}
};

#endif
