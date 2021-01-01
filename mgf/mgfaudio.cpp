#include "mgfaudio.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <mgf.h>

//extern MGF *mgf;
bool pspAudioInited = false;
bool MP3_Inited = false, WAV_Inited = false, MOD_Inited = false;
int mikModThreadID = -1;

int AudioChannelThread(int args, void *argp) {
  while(WAV_Inited) {
    MikMod_Update();
    sceKernelDelayThread(1);
  }
  return 0;
}

//------------------------------------------
MGFAUDIO::MGFAUDIO(int pMaxCount, int pChanel) {
  StopType = 0;
  MaxCount = pMaxCount;
  Count = 0;
  Chanel = pChanel;
  AutoNext = true;
  Playing = false;
  Paused = false;
  CurIndex = -1;
  PlayList = new MGFAUDIO_FILE[MaxCount];
  if(!pspAudioInited){
	  pspAudioInit();
	  pspAudioInited = true;
	}
}

//------------------------------------------
MGFAUDIO::~MGFAUDIO(void) {
  Clear();
  if(WAV_Inited) {
		WAV_Inited = false;
		if(mikModThreadID > 0){
			SceUInt timeout = 10000;
			sceKernelWaitThreadEnd(mikModThreadID, &timeout);
			sceKernelDeleteThread(mikModThreadID);
		}
		
		MikMod_DisableOutput();
		MikMod_Exit();
	}
	   
  delete[] PlayList;
}

//------------------------------------------
int MGFAUDIO::Add(const char *filename) {
  char* suffix = strrchr(filename, '.');
  if(Count + 1 > MaxCount) return -1;
  
	if(suffix) {
		if (!stricmp(suffix, ".AT3") || !stricmp(suffix, ".at3")) PlayList[Count].Type = MGF_AT3;
		else if (!stricmp(suffix, ".MP3") || !stricmp(suffix, ".mp3")) PlayList[Count].Type = MGF_MP3;
		else if (!stricmp(suffix, ".WAV") || !stricmp(suffix, ".wav")) PlayList[Count].Type = MGF_WAV;
		else return -1;
		strcpy(PlayList[Count].Name, filename);
    PlayList[Count].sfx=NULL;
		Count++;
  } else return -1;
}

//------------------------------------------
int MGFAUDIO::Delete(int Index) {
	/*if(Playing)Stop();
	Playing=false;
	if(Index>=Count)return -1;
  return 1; */
}

//------------------------------------------
int MGFAUDIO::PlayFile(const char* FileName) {
	Clear();
	AutoNext = false;
	if(Add(FileName) != -1) Play();
}

int MGFAUDIO::Play(int Index) {
	if(!Paused && Playing) Stop();
  if(Count > 0 && CurIndex == -1) CurIndex = 0;

	if(!Paused)
    switch(PlayList[CurIndex].Type){
      case MGF_MP3: 
  			MP3_Init(Chanel);
   	  	MP3_Inited = true;
        if(Index != -1) CurIndex = Index;
  		  MP3_Load(PlayList[CurIndex].Name); 
			 break;
      case MGF_WAV: 
		    if(!WAV_Inited) {
   				MikMod_RegisterAllDrivers();
   				md_mode = DMODE_16BITS | DMODE_STEREO | DMODE_SOFT_SNDFX | DMODE_SOFT_MUSIC;
   				if(MikMod_Init("")) return -1;
   				MikMod_SetNumVoices(0, 20);
   				MikMod_EnableOutput();
   				WAV_Inited = true;
				  if((mikModThreadID = sceKernelCreateThread("MikMod", (SceKernelThreadEntry)AudioChannelThread, 0x12, 0x10000, 0, NULL)) > 0){
				     sceKernelStartThread(mikModThreadID, 0, NULL);
					} else return -1;
   			}
			break;
    }
  
  switch(PlayList[CurIndex].Type){
    case MGF_MP3: 
			if(!Paused) {
				MP3_Play(); 
			} else {
				MP3_Pause();			    
			}
		break;
		case MGF_WAV: 
			int ind = Index == -1 ? CurIndex : Index;
			if(!PlayList[ind].sfx) PlayList[ind].sfx = Sample_Load(PlayList[ind].Name);
			if(!PlayList[ind].sfx) return -1;
			PlayList[ind].voice = Sample_Play(PlayList[ind].sfx, 0, 0);
			Voice_SetPanning(PlayList[ind].voice, 127);
			Voice_SetVolume(PlayList[ind].voice, 255);
		break;
	}
  
	Playing = true;  
  Paused = false;
  return 1; 
}

//------------------------------------------
int MGFAUDIO::Stop(int Index) {
	switch(PlayList[CurIndex].Type) {
		case MGF_MP3:
			if(Playing && !MP3_EndOfStream())MP3_End();
		break;
		case MGF_WAV:
			int ind=Index==-1?CurIndex:Index;
			if(PlayList[ind].voice != -1 && !Voice_Stopped(PlayList[ind].voice)) Voice_Stop(PlayList[ind].voice);
		break;
	}
	
	StopType = MGF_HAND_STOP;
	
	Playing = false;
	Paused = false;
	return 1;
}

//------------------------------------------
int MGFAUDIO::Pause() {
  switch(PlayList[CurIndex].Type) {
    case MGF_MP3: MP3_Pause();
    break;
    case MGF_WAV: return -1; 
    break;
  }

  Paused = true;
  return 1; 
}

//------------------------------------------
void MGFAUDIO::Update() {
	switch(PlayList[CurIndex].Type) {
		case MGF_MP3:
			if(AutoNext && Playing && MP3_EndOfStream()) {
				if(Playing) MP3_End();
				Playing = false;
				if(Next() == -1) StopType = MGF_END_PLAYLIST;
			} else if(MP3_EndOfStream()) {
				if(Playing) MP3_End();
				Playing = false;
				StopType = MGF_END_PLAYFILE;
			}
		break;
		case MGF_WAV:
		break;
	}
}

//------------------------------------------
int MGFAUDIO::Next() {
	if(Playing) Stop();
	if(CurIndex == Count - 1) return -1;
	CurIndex++;
	Play();
	return 0;
}

//------------------------------------------
int MGFAUDIO::Prev() {
	if(Playing) Stop();
	if(CurIndex == 0) return -1;
	CurIndex--;
	Play();
	return 0;
}

//------------------------------------------
int MGFAUDIO::Clear() {
	if(Playing) Stop();
  for(int i = 0; i < Count; i++) {
		switch(PlayList[CurIndex].Type) {
		  case MGF_MP3: 
				break;
		  case MGF_WAV: 
 	      if(PlayList[i].sfx) Sample_Free(PlayList[i].sfx); 
			break;
		}
		PlayList[i].Name[0] = '\0';
	}
	Count = 0;
	CurIndex = -1;
}

