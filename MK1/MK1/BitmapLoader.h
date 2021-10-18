#pragma once

#include "pch.h"


#include "AnimationFilm.h"


class BitmapLoader
{
private:
	typedef std::map<std::string, ALLEGRO_BITMAP*> BITMAPMAP;
	BITMAPMAP bitmaps;
	ALLEGRO_BITMAP* GetBitmap(const std::string path) const;

	static BitmapLoader* loader;
	BitmapLoader();
public:
	ALLEGRO_BITMAP* Load(const std::string& path);
	void CleanUp(void);

	static BitmapLoader* Get();

	~BitmapLoader();
};

