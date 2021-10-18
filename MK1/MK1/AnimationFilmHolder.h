#pragma once

#include "pch.h"

#include "AnimationFilm.h"
#include "BitmapLoader.h"


class AnimationFilmHolder
{
private:
	typedef std::map<std::string, AnimationFilm*> FILMS;
	FILMS films;
	//BitmapLoader* bitmaps;

	static AnimationFilmHolder* holder; //Our singleton


	AnimationFilmHolder(void);
public:
	~AnimationFilmHolder();
	static AnimationFilmHolder* Get(void);
	void Load(const std::string& catalogue);
	void CleanUp(void);
	AnimationFilm* GetFilm(const std::string id) const;

	void makeSheets(std::string output,ALLEGRO_DISPLAY *display);


};

