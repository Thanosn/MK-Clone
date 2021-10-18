#include "pch.h"
#include "AnimatorHolder.h"
#include "CollisionChecker.h"


AnimatorHolder::AnimatorList AnimatorHolder::running = AnimatorHolder::AnimatorList();
AnimatorHolder::AnimatorList AnimatorHolder::suspended = AnimatorHolder::AnimatorList();

void AnimatorHolder::Register(Animator* a) { suspended.push_back(a); }
void AnimatorHolder::Cancel(Animator* a) {
	auto it = std::find(suspended.begin(), suspended.end(), a);
	if (it == suspended.end())return;

	suspended.erase(it);
}



void AnimatorHolder::MarkAsRunning(Animator* a)
{
	Cancel(a);
	running.push_back(a);
}
void AnimatorHolder::MarkAsSuspended(Animator* a)
{
	auto it = std::find(running.begin(), running.end(), a);
	if (it == running.end())return;
	running.erase(it);
	Register(a);
}

void AnimatorHolder::Create(Animator * a)
{
	Register(a);
	MarkAsRunning(a);
}

void AnimatorHolder::Disable(Animator * a)
{
	MarkAsSuspended(a);
	Cancel(a);
	//CollisionChecker::Get()->Cancel(a->GetSprite());
}


void AnimatorHolder::TimeShift(timestamp_t currTime)
{
	for (auto i = 0; i < running.size(); i++)
		running[i]->TimeShift(currTime);
}

void AnimatorHolder::Progress(timestamp_t currTime) {
	/*
	auto i = running.begin();
	while (i != running.end()) {
		auto tmp = i++;
		bool last = (i == running.end());
		(*tmp)->Progress(currTime);
		if (last)break;
	}*/

	for (auto i = 0; i < running.size(); i++)
		running[i]->Progress(currTime);


}
