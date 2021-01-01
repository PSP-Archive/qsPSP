#include <mgf.h>

#include <mgfcallback.h>
#include <tgatexture.h>
#include <pngtexture.h>
#include <jpgtexture.h>
#include <mgfgeometry.h>

#include <math.h>
#include <pspiofilemgr.h>

static unsigned int __attribute__((aligned(16))) dList[262144];

/*   MGFAPP   */

MGF *MGFAPP::mgf = 0;

MGFAPP::MGFAPP(void) {
	mgf = mgfCreate();
	_prev = 0;
}

MGFAPP::~MGFAPP(void) {
	mgf->Release();
}


/*   MGF   */

int nMgfRef = 0;
MGF *pMgf = 0;

MGF *mgfCreate(void) {
	return (MGF*)MGF::_GetInterface();
}

MGF::MGF(void) {
	currApp = 0;
	isTexture = false;
	fps = 0;
	fpsCount = 0;
	isInit = false;
	//isQuit = false;
	isVsync = false;
	isDebug = false;
	isSwizzle = true;
	
	TextureList = 0;
	
	//logFile = fopen("mgf_log.txt", "rb+");
	//rewind(logFile);
	logFile = sceIoOpen("mgf_log.txt", PSP_O_WRONLY|PSP_O_CREAT|PSP_O_TRUNC, 0777);
}

MGF::~MGF(void) {
	//Game = 0;
	Texture_Clear();
	System_Log("\nEnd.\n\n");
	//fclose(logFile);
	if(logFile) sceIoClose(logFile);
}

void MGF::Release(void) {
	nMgfRef--;
	if(!nMgfRef) {
		delete pMgf;
		pMgf = 0;
	}
}

bool MGF::System_Init(void) {
	SetupCallbacks();
	
	if(isDebug) pspDebugScreenInit();
	
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
		
	// gfx
	_InitGfx();
	
	// timer
	//timeLastAsk = time(NULL);
  sceRtcGetCurrentTick(&timeLastAsk);
	tickResolution = sceRtcGetTickResolution();
	
	Random_Seed(0);
	isInit = true;
	
	System_Log("MGF Engine v.0.3.\n(c) Osokin 'elmortem' Makar, 2006-2009\n\n");
	
	return true;
}

void MGF::System_Shutdown(void) {
	sceGuDisplay(GU_FALSE);
	sceGuTerm();
	sceKernelExitGame();
}

int MGF::System_Run(MGFAPP *app, bool dialog) {
	if(!app) return 0;
	int sec = 0;
	char str[20];
	
	//if(Game) Game->Preinit();
	// 9270 / 8803623762
	
	if(!dialog) {
		app->_prev = currApp;
		currApp = app;
		app->Init();
	}
	
	PadBtnOld = Pad.Buttons;
	bool isQuit = false;
	//int fps = 0, fpsCount = 0;
  //double fpsTime = 0;
	
	while(!isQuit) {
		// считаем время
		sceRtcGetCurrentTick(&timeNow);
		timeDelta = ((double)(timeNow - timeLastAsk) / ((double)tickResolution * 0.001)) * 0.001;	
		timeLastAsk = timeNow;
		// считаем FPS
		fps++;
		fpsTime += timeDelta;
		if(fpsTime >= 1.f)	{
			fpsTime -= 1.f;
			fpsCount = fps;
			fps = 0;
			sec++;
		}
	
		// ввод
		//sceCtrlPeekBufferPositive(&Pad, 1);
		sceCtrlReadBufferPositive(&Pad, 1);
		
		isQuit = app->Update();
		app->Render();
		
		PadBtnOld = Pad.Buttons;
	}
	
	if(!dialog) {
		app->Deinit();
	
		currApp = currApp->_prev;
		app->_prev = 0;
	}
	
	return AppResult;
}

void MGF::System_SetState(int state, bool value) {
	switch(state) {
		case MGF_DEBUG: isDebug = value; break;
		case MGF_VSYNC: isVsync = value; break;
		case MGF_TEXTURE_SWIZZLE: 
			if(!isInit)	isSwizzle = value; 
		break;
	}
}

void MGF::System_SetState(int state, int value) {
	switch(state) {
		case MGF_APPRESULT: AppResult = value;
	}
}

void MGF::System_SetState(int state, void *value) {
	switch(state) {
		case MGF_USERDATA: userdata = value;
		case MGF_APPRESULT: AppResult = (int)value;
	}
}

void *MGF::System_GetState(int state) {
	switch(state) {
		case MGF_APP: return (void*)currApp;
		case MGF_APPRESULT: return (void*)AppResult;
		case MGF_USERDATA: return userdata;
	}
}

void MGF::System_Log(const char *format, ...) {
	char msg[2048];
	va_list list;
	
	va_start(list, format);
	vsprintf(msg, format, list);
	va_end(list);
	
	//fprintf(logFile, msg);
	//int c = strlen(msg);
	//fwrite(&c, 4, 1, logFile);
	//fwrite(msg, c, 1, logFile);
	//sceIoWrite(logFile, &c, 4);
	sceIoWrite(logFile, msg, strlen(msg));
	
	if(isDebug) fprintf(stderr, "$>%s", msg);
	
	if(isDebug) {
		if(logFile) sceIoClose(logFile);
		logFile = sceIoOpen("mgf_log.txt", PSP_O_WRONLY|PSP_O_APPEND|PSP_O_CREAT, 0777);
	}
}

// timer

int MGF::Timer_GetFps(void) {
  return fpsCount;
}

double MGF::Timer_GetDelta(void) {
	return timeDelta;
}

// input

bool MGF::Input_GetKeyState(int key) {
  return (Pad.Buttons & key) == key; 
}

bool MGF::Input_IsKeyClick(int key) {
  return (Pad.Buttons & key) == key && (PadBtnOld & key) != key; 
}

void MGF::Input_GetAnalog(int *x, int *y) {
	*x = (int)Pad.Lx - 128;
	*y = (int)Pad.Ly - 128;
}

// graphics

void MGF::Gfx_SetClipping(int x, int y, int width, int height) {
  if(x == -1) sceGuScissor(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  else sceGuScissor(x, y, x+width, y+height);
}

void MGF::Gfx_SetBlend(int type) {
	if(type == MGFBLEND_NONE) {
    sceGuDisable(GU_BLEND);
	} else {
    sceGuEnable(GU_BLEND);
		switch(type) {
			case MGFBLEND_SUB: 
				sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
			break;
			case MGFBLEND_ADD:
				sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_DST_COLOR, 0, 0);
			break;
			case MGFBLEND_MULT: 
				sceGuBlendFunc(GU_ADD, GU_SRC_COLOR, GU_DST_COLOR, 0, 0);
			break;
		}
	}
}

void MGF::Gfx_Clear(int color) {
	sceGuClearDepth(0);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
}

void MGF::Gfx_BeginScene(void) {
	//mCurrentPointer = mVideoBufferStart;
	
	sceGuStart(GU_DIRECT, dList);
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
}

void MGF::Gfx_EndScene(void) {
	sceGuFinish();
	sceGuSync(0, 0);

  //sceKernelDcacheWritebackAll();
	if(isVsync)
		sceDisplayWaitVblankStart();
	
	sceGuSwapBuffers();
	
	//mCurrentPointer = (u32)sceGuSwapBuffers();
}

void MGF::Gfx_SetProjection(int type) {
	if(type == MGF_PERSPECTIVE) {
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective( 75.0f, 16.0f/9.0f, 0.5f, 100.0f);
	
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();
	} else if(type == MGF_ORTHO) {
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumOrtho(0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.f, 0.f, 1.f);
		
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();
	}
}

void MGF::Gfx_RenderFps(void) {
	if(isDebug) {
		char str[20];
		sprintf(str, "fps: %d", fpsCount);
		Gfx_RenderDebug(0, 0, str);
	}
}

void MGF::Gfx_RenderDebug(int x, int y, const char *text) {
	if(isDebug) {
		//pspDebugScreenSetOffset((int)mCurrentPointer);
		pspDebugScreenSetOffset((int)fbp0);
		pspDebugScreenSetXY(x, y);
		pspDebugScreenPrintf(text);
	}
}

void MGF::Gfx_RenderLine(float x1, float y1, float x2, float y2, int col1, int col2) {
	if(col2 == -1) col2 = col1;
	MGFLINE2D *vtmp = (MGFLINE2D*)sceGuGetMemory(sizeof(MGFLINE2D));
	vtmp->Reset();
	vtmp->v[0].color = col1;
	vtmp->v[1].color = col2;
	vtmp->SetVertex(x1, y1, x2, y2);
		
	sceGuDisable(GU_TEXTURE_2D);
	sceGumDrawArray(GU_LINES, MGF_VERTEX_3D, 2, 0, vtmp->v);
	if(isTexture) Texture_Enable(true);
}

// texture
MGFTEXTURE *MGF::Texture_Add(const char *filename) {
	MGFTEXTURE* tex = 0;
	System_Log("Loading '%s' texture...\n", filename);

  char* suffix = strrchr(filename, '.');
	  
  if(suffix) {
		if (!stricmp(suffix, ".jpg")|| !stricmp(suffix, ".jpeg") || !stricmp(suffix, ".JPG") || !stricmp(suffix, ".JPEG")) tex = new JPGTEXTURE();
		else if (!stricmp(suffix, ".png") || !stricmp(suffix, ".PNG")) tex = new PNGTEXTURE();
		else if (!stricmp(suffix, ".tga") || !stricmp(suffix, ".TGA")) tex = new TGATEXTURE();
		else return 0;
  } else return 0;
	
/*	if(filename[strlen(filename) - 2] == 'g') tex = new TGATEXTURE();
	else if(filename[strlen(filename) - 2] == 'n') tex = new PNGTEXTURE();
	else if(filename[strlen(filename) - 2] == 'p') tex = new JPGTEXTURE();
	else return 0; */
	
	if(!tex->Load(filename, isSwizzle)) {
		System_Log("Texture '%s' not loaded.\n", filename);
		delete tex;
		return 0;
	}
	
	tex->next = TextureList;
	TextureList = tex;
	
	System_Log("Done.\n");
	
	return tex;
}

void MGF::Texture_Remove(MGFTEXTURE *tex) {
	MGFTEXTURE *t = TextureList, *prev = 0;
	
	while(t) {
		if(t == tex) {
			if(!prev)
	      TextureList = t->next;
	    else
	      prev->next = t->next;

			delete t;
			
			return;
		}
		
		prev = t;
		t = t->next;
	}
}

void MGF::Texture_Clear(void) {
	MGFTEXTURE *t = TextureList, *next;
	
	while(t) {
		next = t->next;
		delete t;
		
		t = next;
	}
	TextureList = 0;
}

void MGF::Texture_Bind(MGFTEXTURE *tex) {
	if(!tex) return;
	//sceGuEnable(GU_TEXTURE_2D);
	sceGuTexImage(0, tex->Width(), tex->Height(), tex->Width(), tex->Image());
}

void MGF::Texture_Enable(bool value) {
	if(value)
		sceGuEnable(GU_TEXTURE_2D);
	else
		sceGuDisable(GU_TEXTURE_2D);
	isTexture = value;
}

// random
void MGF::Random_Seed(int seed) {
	if(!seed) g_seed = timeLastAsk * 1000;
	else g_seed = seed;
}

int MGF::Random_Int(int min, int max) {
	g_seed = 214013 * g_seed + 2531011;
	return min+(g_seed ^ g_seed >> 15) % (max - min);
}

float MGF::Random_Float(float min, float max) {
	g_seed = 214013 * g_seed + 2531011;
	return min+(g_seed >> 16) * (1.0f / 65535.0f) * (max - min);
}


void MGF::_InitGfx(void) {

	fbp0 = getStaticVramBuffer(FRAME_BUFFER_WIDTH, SCREEN_HEIGHT, GU_PSM_8888);
	fbp1 = getStaticVramBuffer(FRAME_BUFFER_WIDTH, SCREEN_HEIGHT, GU_PSM_8888);
	zbp = getStaticVramBuffer(FRAME_BUFFER_WIDTH, SCREEN_HEIGHT, GU_PSM_4444);

	sceGuInit();

	// setup GU
	sceGuStart(GU_DIRECT, dList);

	sceGuDrawBuffer(BUFFER_FORMAT, fbp0, FRAME_BUFFER_WIDTH);
	sceGuDispBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, fbp1, FRAME_BUFFER_WIDTH);
	sceGuDepthBuffer(zbp, FRAME_BUFFER_WIDTH);
	sceGuOffset(2048 - (SCREEN_WIDTH / 2), 2048 - (SCREEN_HEIGHT/2));
	sceGuViewport(2048, 2048, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuDepthRange(65535, 0); 
	//sceGuDepthRange(0xc350, 0x2710);


	Gfx_SetClipping(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);

	//Настройки рендеринга, могут меняться в зависимости от надобностей проекта
	sceGuDepthFunc(GU_GEQUAL);
	//	sceGuFrontFace(GU_CCW);
	//	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_CLIP_PLANES);
	
	
	sceGuTexWrap(GU_REPEAT, GU_REPEAT);
	sceGuTexMode(GU_PSM_8888, 0, 0, isSwizzle);	
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
	sceGuTexFilter(GU_LINEAR, GU_LINEAR);
	//sceGuTexFilter(GU_NEAREST, GU_NEAREST);
  sceGuTexScale(1.0f, 1.0f);
	sceGuTexOffset(0.0f, 0.0f);
	//------------------------------------


	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);
}

MGF *MGF::_GetInterface(void) {
	if(!pMgf) pMgf = new MGF();
	nMgfRef++;
	return pMgf;
}
