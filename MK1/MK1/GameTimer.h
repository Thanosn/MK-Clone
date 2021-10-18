#pragma once

class GameTimer
{
	static timestamp_t gameTime;

public:

	static timestamp_t GetGameTime(void);

	static timestamp_t GetCurrTime(void);

	static void SetGameTime(timestamp_t time);
};

