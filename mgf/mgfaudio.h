#ifndef _MGFAUDIO_H_
#define _MGFAUDIO_H_

#include <pspaudio.h>
#include <pspaudiolib.h>
#include <mikmod.h>
#include "mp3player.h"

enum {
  MGF_AT3=0,
	MGF_MP3,
	MGF_WAV
};

enum {
  MGF_HAND_STOP=1,
  MGF_END_PLAYFILE,
  MGF_END_PLAYLIST
};


struct MGFAUDIO_FILE {
  char Name[256];
  int Type;
  SAMPLE *sfx; 
  int voice;
};
 

class MGFAUDIO {
public:
	MGFAUDIO(int pMaxCount = 1,int pChanel = 1);
	~MGFAUDIO(void);
  int StopType;
  bool AutoNext;
  bool Playing;
  bool Paused;
  int CurIndex;
	int Count;
	int Add(const char *filename);
	int Delete(int Index);
	int PlayFile(const char *filename);
	int Play(int Index = -1);
	int Stop(int Index = -1);
	int Pause();
	int Next();
	int Prev();
	int Clear();
	void Update();
	
protected:
  int MaxCount;
  MGFAUDIO_FILE *PlayList;
  int Chanel;
};

#endif
