#pragma once


#include <stdio.h>
#include <assert.h>
#include <string>
#include <list>
#include <functional>
#include <map>
#include <set>
#include <algorithm>

#include <vector>
#include <sstream>


#include <iostream>


#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>


#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>


#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "StateTransitions.h"

#include "Fighter.h"
#include "GameTimer.h"



#define SCREEN_WIDTH 1220	
#define SCREEN_HEIGHT 500

#define	FIGHTER_ACTION_DELAY_MSECS	150

#ifndef DEBUG
	#undef assert
	#define assert(EXPR) ((void*)0)
#endif // DEBUG
