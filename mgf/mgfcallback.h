#ifndef _MGFCALLBACK_H_
#define _MGFCALLBACK_H_

#include <malloc.h>
#include <pspkernel.h>
#include <pspdisplay.h>
#include <pspdebug.h>
#include <stdio.h>

#include <pspgu.h>
#include <pspgum.h>
#include <pspctrl.h>

#include <psptypes.h>
#include <stdlib.h>
#include <psprtc.h>

int exit_callback(int arg1, int arg2, void *common);
int CallbackThread(SceSize args, void *argp);
int SetupCallbacks(void);

#endif
