#ifdef SOUNDMANAGE_EXPORTS
#define SOUNDMAN_API __declspec(dllexport)
#else
#define SOUNDMAN_API __declspec(dllimport)
#endif


#include "GameSoundMan.h"