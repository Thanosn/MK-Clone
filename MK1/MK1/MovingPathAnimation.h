#pragma once
#include "Animation.h"

typedef unsigned char frame_t;
struct PathEntry {
	offset_t 	dx, dy;
	frame_t 	frame;
	delay_t	delay;
	PathEntry(void) : dx(0), dy(0), frame(0xff), delay(0) {}
	PathEntry(const PathEntry& p) :
		dx(p.dx), dy(p.dy), frame(p.frame), delay(p.delay) {}

	PathEntry(int _dx, int _dy, frame_t _frame, delay_t _delay) :
		dx(_dx), dy(_dy), frame(_frame), delay(_delay) {}
};

class MovingPathAnimation : public Animation {
	std::vector<PathEntry>* path;
public:
	std::vector<PathEntry>* GetPath(void) const;
	void SetPath(std::vector<PathEntry>* p);
	Animation* Clone(animid_t newId) const;
	MovingPathAnimation(std::vector<PathEntry>*, animid_t);
	~MovingPathAnimation();
};

