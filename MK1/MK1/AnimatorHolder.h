#pragma once
#include "Animator.h"

#define DELETE_ANIMATOR(animator)			\
	animator->Destroy();

class AnimatorHolder {
	typedef  std::vector<Animator*> AnimatorList;
	static AnimatorList running, suspended;

	class ProgressFunctor : public std::unary_function<Animator*, void> {
		timestamp_t t;
	public:
		void operator()(Animator* a) const { a->Progress(t); }
		ProgressFunctor(timestamp_t _t) : t(_t) {}
	};

	static void Register(Animator* a);
	static void Cancel(Animator* a);
	static void MarkAsRunning(Animator* a);
	static void MarkAsSuspended(Animator* a);
public:
	static void Create(Animator* a);
	static void Disable(Animator* a);

	static void TimeShift(timestamp_t currTime);
	static void Progress(timestamp_t currTime);
};

