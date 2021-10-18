#pragma once

#include "pch.h"
#include "FrameRangeAnimation.h"


//TODO: find a better way to define them (this is temp here)
	struct Point
	{
		int x, y;
	};

	struct Rect
	{
		int x, y, width, height;
	};
//

class AnimationFilm {
	std::vector<Rect>	boxes;
	ALLEGRO_BITMAP*		bitmap;
	std::string	id;
public:
	frame_t				GetTotalFrames(void) 	const;
	ALLEGRO_BITMAP*		GetBitmap(void) const;

	const std::string 	GetId(void) const;
	const Rect			GetFrameBox(size_t frameNo) const;

	void				DisplayFrame(ALLEGRO_BITMAP* d, const Point& at, size_t frameNo) const;

	AnimationFilm(ALLEGRO_BITMAP* bitmap, const std::vector<Rect> boxes, const std::string& id);
};


