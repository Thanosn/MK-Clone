#include "pch.h"
#include "AnimationFilm.h"



frame_t				AnimationFilm::GetTotalFrames(void) 	const { return boxes.size(); }

ALLEGRO_BITMAP*		AnimationFilm::GetBitmap(void) const { return bitmap; }

const std::string 	AnimationFilm::GetId(void) const { return id; }

const Rect			AnimationFilm::GetFrameBox(size_t frameNo) const
{
	assert(boxes.size() > frameNo);
	return boxes[frameNo];
}

void				AnimationFilm::DisplayFrame(ALLEGRO_BITMAP * d, const Point & at, size_t frameNo) const
{
	//MaskedBlit(bitmap, GetFrameBox(frameNo), dest, at);
}

AnimationFilm::AnimationFilm(ALLEGRO_BITMAP* bitmap, const std::vector<Rect> boxes, const std::string& id):
	bitmap(bitmap),boxes(boxes),id(id)
{
}

