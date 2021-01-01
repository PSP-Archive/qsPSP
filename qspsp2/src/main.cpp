#include "main.h"

#include "app.h"

//PSP_HEAP_SIZE_KB(64);
PSP_MODULE_INFO("qsPSP v.0.9", 0, 1, 1);
//PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | PSP_THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(20000);

MGF *mgf;
CApp *App;

int main(int argc, char **argv) {
	mgf = mgfCreate();
	mgf->System_SetState(MGF_DEBUG, true);
	
	mgf->System_Init();
	mgf->Random_Seed();
	
	App = new CApp();

	mgf->System_Run(App);

	delete App;
	mgf->System_Shutdown();
	delete mgf;
	
	return 0;
}

