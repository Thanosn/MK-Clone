#pragma once
#include "Animation.h"

enum ScrollValue {
	NEGATIVE = -1,
	NONE = 0,
	POSSITIVE = 1
};

struct ScrollEntry {
	ScrollValue	 h;
	ScrollValue  v;
	delay_t     delay;
};

class ScrollAnimation : public Animation {
	std::list<ScrollEntry> scroll;
public:

	const std::list<ScrollEntry>&	GetScroll(void) const;
	void							SetScroll(const std::list<ScrollEntry>& p);

	Animation*						Clone(animid_t newId) const;

	ScrollAnimation(const std::list<ScrollEntry>& _scroll, animid_t id);
};


