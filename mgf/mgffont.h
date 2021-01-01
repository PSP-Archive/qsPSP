#ifndef _MGFFONT_H_
#define _MGFFONT_H_

#include <mgf.h>
#include <mgftexture.h>
#include <mgfgeometry.h>

enum {
	MGFFONT_LEFT = 0,
	MGFFONT_CENTER,
	MGFFONT_RIGHT
};

class MGFFONT {
public:
	MGFFONT(const char *filename);
	~MGFFONT(void);
	
	void SetScale(float scale);
	void SetAngle(float angle);
	void SetColor(int color);
	float GetWidth(const char *text, bool scale = true, const char *end = 0);
	float GetWidthAll(const char *text, bool scale = true, const char *end = 0);
	float GetHeight(bool scale = true);
	int GetLinesCount(const char *text);
	void CreateWidth(float width, char *text);
	void Render(float x, float y, int align, const char *text);
	void RenderInBox(float x, float y, float bx, float by, float bw, float bh, int align, const char *text);
	float RenderChar(float x, float y, int align, unsigned char ch);
	//void RenderBox(float x, float y, float width, float height, int align, const char *text);
	
protected:
	static MGF *mgf;
	bool isLoad;
	MGFTEXTURE *Tex;
	MGFQUAD *Quads[256];
	int Pre[256];
	int Post[256];
	int Height;
	float Scale, Angle;
	int Color;
};

#endif
