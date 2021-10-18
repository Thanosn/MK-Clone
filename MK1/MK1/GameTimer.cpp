#include "pch.h"
#include "GameTimer.h"

timestamp_t GameTimer::gameTime = 0;

timestamp_t GameTimer::GetGameTime(void) {
	return gameTime;
}

timestamp_t GameTimer::GetCurrTime(void) {
	return al_get_time() * 1000;
}

void GameTimer::SetGameTime(timestamp_t time) {
	gameTime = time;
}