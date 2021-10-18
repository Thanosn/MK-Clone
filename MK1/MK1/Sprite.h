#pragma once

#include "AnimationFilm.h"
#include "LatelyDestroyable.h"

#include "Fighter.h"




enum TYPE {
	FIGHTER_0,
	FIGHTER_1,
	THROWABLE_0,
	THROWABLE_1,

	DECORATIVES
};
enum STATE {
	HITTING,
	IDLE,
	TRANSPARENT
};

class SpriteWithCollision;

struct CollisionHandler {
	virtual void operator() (SpriteWithCollision* caller, SpriteWithCollision* arg) const = 0;
	virtual CollisionHandler* Clone(void) const = 0;
	virtual ~CollisionHandler() {}
};

class SpriteWithCollision {
protected: 
	typedef std::list<CollisionHandler*> Handlers;
	TYPE type;
	STATE state;
	Handlers handlers;
	void NotifyCollision(SpriteWithCollision* arg) {
		for (auto i = handlers.begin(); i != handlers.end(); i++) {
			(**i)(this, arg);
		}
	}
public:
	TYPE GetType() const { return type; }
	STATE GetState() const { return state; }
	void SetState(STATE _state) { state = _state; }

	void AddColisionHandler(const CollisionHandler& h) {
		handlers.push_back(h.Clone());
	}
	void clearHandlers() {
		for (auto i = handlers.begin(); i != handlers.end(); i++) {
			delete *i;
		}
		handlers.clear();
	}

	SpriteWithCollision(TYPE collisionType, STATE _state = STATE::IDLE) :type(collisionType), state(_state) {}
	~SpriteWithCollision() { clearHandlers(); }
};

#include "Fighter.h"
class Sprite : public SpriteWithCollision, public LatelyDestroyable {
	size_t		frameNo;
	Rect		frameBox;
	int			x, y;
	bool		isVisible;
	AnimationFilm*	currFilm;

	bool  gravityAddicted;
	bool  isFalling;
	bool  looksRight;
	void* owner;
	
	//TileLayer*	myLayer;
public:
	void SetFrame(size_t i);
	size_t GetFrame(void) const;
	void SetVisibility(bool v);
	bool IsVisible(void) const;
	void SetX(int x);
	int GetX() const;
	void SetY(int y);
	int GetY() const;
	void SetLookRight(bool);
	bool GetLooksRight() const;
	void CollisionCheck(Sprite* s);
	void DeCollision();
	void makeDamage(int dmg);
	Rect GetFrameBox()const;
	void* GetOwner()const;


	void Move(int dx, int dy);
	void Display(ALLEGRO_BITMAP* dest);

	Sprite(int _x, int _y, AnimationFilm* film, TYPE collisionType,void* Owner);
	~Sprite();
};


class SpriteHolder {
public:
	typedef std::list<Sprite*> SpriteList;
protected:
	typedef std::map<TYPE, SpriteList> SpritesByType;
	SpritesByType sprites;
public:
	static SpriteHolder* holder;
	void Add(Sprite* s) {
		sprites[s->GetType()].push_back(s);
	}
	void Remove(Sprite* s) {
		sprites[s->GetType()].remove(s);
	}
	void GetSprites(TYPE type, SpriteList* result)const {
		auto i = sprites.find(type);
		if (i != sprites.end())
			*result = i->second;
	}

	static SpriteHolder* Get(void);

};