#ifndef _MGF_H_
#define _MGF_H_

#ifndef _PSP
#define _PSP
#endif

#ifndef DWORD
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif

#include <malloc.h>
#include <pspkernel.h>
#include <psppower.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>

#include <psptypes.h>
#include <stdlib.h>
#include <psprtc.h>

//#include <libc/time.h>

extern "C" {
	#include <vram.h>
}

#include <mgftexture.h>


#define MGF_VERSION 0x0030

#define SCREEN_WIDTH (480)
#define SCREEN_HEIGHT (272)

#define PIXEL_TYPE u32
#define PIXEL_SIZE 4
#define PIXEL_FORMAT PSP_DISPLAY_PIXEL_FORMAT_8888
#define	BUFFER_FORMAT GU_PSM_8888
#define TEXTURE_FORMAT GU_PSM_8888
#define TEXTURE_COLOR_FORMAT GU_COLOR_8888
/*#define PIXEL_TYPE u16
#define PIXEL_SIZE 2
#define PIXEL_FORMAT PSP_DISPLAY_PIXEL_FORMAT_8888
#define	BUFFER_FORMAT GU_PSM_4444
#define TEXTURE_FORMAT GU_PSM_4444
#define TEXTURE_COLOR_FORMAT GU_COLOR_4444*/

#define	FRAME_BUFFER_WIDTH 512
#define FRAME_BUFFER_SIZE (FRAME_BUFFER_WIDTH * SCREEN_HEIGHT * PIXEL_SIZE)

#define MGF_VERTEX_3D (GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D)
#define MGF_VERTEX_NORMAL_3D (GU_TEXTURE_32BITF|GU_COLOR_8888|GU_NORMAL_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D)
#define MGF_VERTEX_2D (GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D)

#define MGF_MAX_PATH 260

enum {
  MGFBLEND_NONE = 0,
  MGFBLEND_SUB,
  MGFBLEND_ADD,
  MGFBLEND_MULT,
  MGFBLEND_ALPHA
};

enum {
	MGF_PERSPECTIVE = 0,
	MGF_ORTHO
};

enum {
	MGF_APP = 0,
	MGF_APPRESULT,
	MGF_DEBUG,
	MGF_VSYNC,
	MGF_TEXTURE_SWIZZLE,
	MGF_USERDATA
};

class MGF;

class MGFAPP {
public:
	MGFAPP(void);
	~MGFAPP(void);
	
	virtual void Init(void) {};
	virtual void Deinit(void) {};
	virtual bool Update(void) { return false; };
	virtual void Render(void) {};
	
	MGFAPP *_prev;
protected:
	static MGF *mgf;
};

class MGF {
public:
  ~MGF(void);
  
  void Release(void);
  
  // system
  bool System_Init(void);
  int System_Run(MGFAPP *app, bool dialog = false);
  void System_Shutdown(void);
  void System_SetState(int state, bool value);
  void System_SetState(int state, int value);
  void System_SetState(int state, void *value);
  void *System_GetState(int state);
  void System_Log(const char *format, ...);
  
  // timer
  int Timer_GetFps(void);
  double Timer_GetDelta(void);
  
  // input
  bool Input_GetKeyState(int key);
  bool Input_IsKeyClick(int key);
  void Input_GetAnalog(int *x, int *y);
  
  // gfx
  void Gfx_SetClipping(int x = -1, int y = -1, int width = -1, int height = -1);
  void Gfx_SetBlend(int type);
  void Gfx_Clear(int color);
  void Gfx_BeginScene(void);
  void Gfx_EndScene(void);
  void Gfx_SetProjection(int type);
  void Gfx_RenderFps(void);
  void Gfx_RenderDebug(int x, int y, const char *text);
  void Gfx_RenderLine(float x1, float y1, float x2, float y2, int col1, int col2 = -1);
  
  // texture
  MGFTEXTURE *Texture_Add(const char *filename);
  void Texture_Remove(MGFTEXTURE *tex);
  void Texture_Clear(void);
  void Texture_Bind(MGFTEXTURE *tex);
  void Texture_Enable(bool value = true);
  
  // sprite
  
  // random
  void Random_Seed(int seed = 0);
  int Random_Int(int lo, int hi);
  float Random_Float(float lo, float hi);
  
  static MGF *_GetInterface();
  
private:
	MGF(void);
  void _InitGfx(void);

	// debug
	bool isDebug;

	// game
	//MGFGame *Game;
	MGFAPP *currApp;
	int AppResult;
	
	// texture
	bool isTexture;
	
  // timer
  int fps, fpsCount;
  double fpsTime;
  
  double timeDelta;
	u64 timeNow, timeLastAsk;
	u32 tickResolution;
  
  // input
  SceCtrlData Pad;
  u32 PadBtnOld;
  
  // system
  bool isInit;
  //bool isQuit;
  bool isVsync;
  //FILE *logFile;
  int logFile;
  /*u32 mVideoBufferStart;
	u32 mCurrentPointer;
	PIXEL_TYPE *mVRAM;*/
	void *fbp0;
	void *fbp1;
	void *zbp;
  
  u64 fpsTickNow;
  u64 fpsTickLast;
  
  // texture
  bool isSwizzle;
  MGFTEXTURE *TextureList;
  
  // random
  int g_seed;
  
  void *userdata;
};

extern MGF *mgfCreate(void);

#endif
