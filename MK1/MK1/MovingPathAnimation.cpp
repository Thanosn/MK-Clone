#include "pch.h"
#include "MovingPathAnimation.h"


std::vector<PathEntry>* MovingPathAnimation::GetPath(void) const
{
	return path;
}
void MovingPathAnimation::SetPath(std::vector<PathEntry>* p)
{
	path->clear();
	path = p;
}
Animation* MovingPathAnimation::Clone(animid_t newId) const
{
	return new MovingPathAnimation(new std::vector<PathEntry>(*path), newId);
}
MovingPathAnimation::MovingPathAnimation( std::vector<PathEntry>* _path, animid_t id) :
	path(_path), Animation(id) {}

MovingPathAnimation::~MovingPathAnimation()
{
	path->clear();
	delete path;
	//delete path;
}
