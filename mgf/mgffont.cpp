#include <mgffont.h>

#include <stdio.h>
#include <math.h>
#include <string.h>

MGF *MGFFONT::mgf = 0;

MGFFONT::MGFFONT(const char *filename) {
	mgf = mgfCreate();
	
	isLoad = false;
	Angle = 0.f;
	Scale = 1.f;
	mgf->System_Log("Loading '%s' font.\n", filename);
	
	char PathDir[MGF_MAX_PATH], TexPath[MGF_MAX_PATH];
	
	unsigned char sig[8];
	int n = 0;
	unsigned char b = 0, c = 0;
	int x, y, w, h;
	
	// find directory
	for(int i = strlen(filename) - 1; i > 0; i--) {
		if(filename[i] == '\\' || filename[i] == '/') {
			n = i + 1;
			break;
		}
	}
	if(n) {
		strncpy(PathDir, filename, n);
		PathDir[n] = '\0';
	} else {
		PathDir[0] = '\0';
	}
	
	mgf->System_Log("Font directory '%s'.\n", PathDir);

	// free data
	Tex = 0;
	for(int i = 0; i < 256; i++) {
		Quads[i] = 0;
		Pre[i] = 0;
		Post[i] = 0;
	}
	Height = 0;
	
	// load font
	FILE *f = fopen(filename, "rb");
	if(!f) {
		mgf->System_Log("FONT error: '%s' not found.\n", filename);
		return;
	}
	fread(sig, 7, 1, f);
	strcpy(TexPath,"");
	while(!feof(f)) {
		fread(&b, 1, 1, f);
		if(b == 10) {
			fread(&b, 1, 1, f);
			fread(TexPath, b, 1, f);
			TexPath[b] = 0;
			strcat(PathDir, TexPath);
			Tex = mgf->Texture_Add(PathDir);
		} else if(b == 20) {
			fread(&c, 1, 1, f);
			
			if(Quads[c]) {
			  fread(&x, 4, 1, f);
				fread(&x, 4, 1, f);
				fread(&x, 4, 1, f);
				fread(&x, 4, 1, f);
				fread(&x, 4, 1, f);
				fread(&x, 4, 1, f);
			  continue;
			}
			
			Quads[c] = new MGFQUAD();
			
			fread(&x, 4, 1, f);
			fread(&y, 4, 1, f);
			fread(&w, 4, 1, f);
			fread(&h, 4, 1, f);
			fread(&Pre[c], 4, 1, f);
			fread(&Post[c], 4, 1, f);
			
			Quads[c]->SetVertex(w, h, false);
			Quads[c]->SetTexCoord((float)x / (float)Tex->Width(), (float)y / (float)Tex->Height(), (float)w / (float)Tex->Width(), (float)h / (float)Tex->Height());
			
			if(h > Height) Height = h;
		}
	}
	
	fclose(f);
	isLoad = true;
	
	mgf->System_Log("Done.\n");
}

MGFFONT::~MGFFONT(void) {
	mgf->Texture_Remove(Tex);
	for(int i = 0; i < 256; i++) {
		if(Quads[i]) delete Quads[i];
		Quads[i] = 0;
	}
	mgf->Release();
}

void MGFFONT::SetScale(float scale) {
  Scale = scale;
}

void MGFFONT::SetAngle(float angle) {
  Angle = angle / 180 * M_PI;
}

void MGFFONT::SetColor(int color) {
  Color = color;
	/*for(int i = 0; i < 256; i++) {
		if(Quads[i]) Quads[i]->SetColor(color);
	}*/
}

float MGFFONT::GetWidth(const char *text, bool scale, const char *end) {
	float w = 0.f;
	unsigned char c = 0;
	
	while(text && *text) 	{
		if(*text == '\n') {
			w = 0.f;
			continue;
		}
		c = (unsigned char)*text;
		if(!Quads[c]) {
		  text++;
		  continue;
		}
		w += Quads[c]->Width + Pre[c] + Post[c];
		text++;
		if(text == end) break;
	}

	if(scale) return w * Scale;
	else return w;
}

float MGFFONT::GetWidthAll(const char *text, bool scale, const char *end) {
	float w1 = 0.f, w2 = 0.f;
	unsigned char c = 0;
	
	while(text && *text) 	{
		c = (unsigned char)*text;
		if(c == '\n') {
		  if(w1 > w2) w2 = w1;
		  w1 = 0.f;
		  continue;
		}
		if(!Quads[c]) {
		  text++;
		  continue;
		}
		w1 += Quads[c]->Width + Pre[c] + Post[c];
		text++;
		if(text == end) break;
	}
	if(w1 > w2) w2 = w1;

	if(scale) return w2 * Scale;
	else return w2;
}

float MGFFONT::GetHeight(bool scale) {
	if(scale) return Height * Scale;
	else return Height;
}

int MGFFONT::GetLinesCount(const char *text) {
  int cnt = 1;
  unsigned char c = 0;
  
  while(text && *text) 	{
		c = (unsigned char)*text;
		if(c == '\n') cnt++;
		text++;
	}
	return cnt;
}

void MGFFONT::CreateWidth(float width, char *text) {
	char *space = 0;
	unsigned char c = 0;
	float w = 0.f;
	char *line = text;
	int num = 0;
	while(text && *text) {
	  c = (unsigned char)*text;
	  if(c == ' ') space = text;
	  if(c == '\n') {
			text++;
			w = 0.f;
		  num++;
		  continue;
		}
	  if(!Quads[c]) {
		  text++;
		  continue;
		}
	
		w += Quads[c]->Width + Pre[c] + Post[c];
		if(w * Scale > width && space) {
		  space[0] = '\n';
		  line = space + 1;
		  text = space;
		  space = 0;
		  w = 0.f;
		  num++;
		}
		text++;
	}
}
	
void MGFFONT::Render(float x, float y, int align, const char *text) {
	RenderInBox(x, y, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, align, text);
}

void MGFFONT::RenderInBox(float x, float y, float bx, float by, float bw, float bh, int align, const char *text) {
	if(!isLoad) return;

	unsigned char n;
	ScePspFVector3 move = { x, y, 0.f };
	ScePspFVector3 scale = { Scale, Scale, Scale };
	float cx = 0.f, cw = 0.f, ch = 0.f;
	float trnsx = 0.f;

	
	sceGumPushMatrix();
	
	sceGumTranslate(&move);
	sceGumRotateZ(Angle);
	sceGumScale(&scale);
	
	if(align == MGFFONT_RIGHT) move.x = -GetWidth(text, false);
	else if(align == MGFFONT_CENTER) move.x = (float)(int)(-GetWidth(text, false) / 2.f);
	if(align != MGFFONT_LEFT ){
		move.y = 0.f;
		sceGumTranslate(&move);
		trnsx = move.x;
	}
	
	
	mgf->Texture_Bind(Tex);
	
	while(*text) {
		n = (unsigned char)*text;

		if(n == '\n') {
			move.x = -cw;
			ch += Height;
			move.y = Height;
			if(align == MGFFONT_RIGHT) move.x = -GetWidth(text + 1, false) + cx;
			else if(align == MGFFONT_CENTER) move.x -= (float)(int)(GetWidth(text + 1, false) / 2.0f);
			sceGumTranslate(&move);
			cx = 0.f;
			cw = 0.f;
			text++;
			continue;
		}
    
		//if(!Quads[n]) n = '?';
		if(!Quads[n]) {
			text++;
			continue;
		}

		cx += (float)Pre[n];
		cw += cx;
		move.x = cx;
		move.y = 0.f;
		sceGumTranslate(&move);
		
		/*mgf->Gfx_RenderLine(0, 0, 0, Quads[n]->Height, 0x66FFFFFF);
		mgf->Gfx_RenderLine(0, Quads[n]->Height, Quads[n]->Width, Quads[n]->Height, 0x66FFFFFF);
		mgf->Gfx_RenderLine(Quads[n]->Width, Quads[n]->Height, Quads[n]->Width, 0, 0x66FFFFFF);
		mgf->Gfx_RenderLine(Quads[n]->Width, 0, 0, 0, 0x66FFFFFF);*/
		
		//if(x + trnsx + cw < bx || x + trnsx + cw + Quads[n]->Width > bx + bw || y + ch < by || y + ch + Quads[n]->Height > by + bh) {
		if(x + trnsx + cw + Quads[n]->Width < bx || x + trnsx + cw > bx + bw || y + ch + Quads[n]->Height < by || y + ch > by + bh) {
			cx = Quads[n]->Width + (float)Post[n];
			text++;
			continue;
		}
		
		MGFQUAD *vtmp = (MGFQUAD*)sceGuGetMemory(sizeof(MGFQUAD));
    memcpy(vtmp, Quads[n], sizeof(MGFQUAD));
		vtmp->SetColor(Color);
		
		sceGumDrawArray(GU_TRIANGLE_FAN, MGF_VERTEX_3D, 4, 0, vtmp->v);
		cx = Quads[n]->Width + (float)Post[n];
		text++;
	}	

	sceGumPopMatrix();
}

float MGFFONT::RenderChar(float x, float y, int align, unsigned char ch) {
	if(!Quads[ch]) return 0.f;
	char str[2];
	str[0] = ch;
	str[1] = '\0';
	Render(x, y, align, str);
	
	return (Quads[ch]->Width + Pre[ch] + Post[ch]) * Scale;
}
