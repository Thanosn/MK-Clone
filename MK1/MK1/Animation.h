#pragma once
#include "LatelyDestroyable.h"

typedef signed char	offset_t;
typedef unsigned short	delay_t;
typedef unsigned short	animid_t;
class Animation :public LatelyDestroyable {
	animid_t id;
public:
	animid_t GetId(void) { return id; }
	virtual Animation* Clone(animid_t newId) const = 0;
	Animation(animid_t _id) : id(_id), LatelyDestroyable() {}
	virtual ~Animation() {}
};
