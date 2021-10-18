#include "pch.h"
#include "Sprite.h"

#define SCALE_FACTOR 1.5
#define COLLISION_OFFSET 0
#define DecollisionNumber 5

#define GROUND_LEVEL 465

void Sprite::SetFrame(size_t i) {
	if (i != frameNo) {
		assert(i < currFilm->GetTotalFrames());
		frameBox = currFilm->GetFrameBox(frameNo = i);
	}
}

void Sprite::Display(ALLEGRO_BITMAP * dest)
{


	//al_draw_bitmap_region(currFilm->GetBitmap(), frameBox.x, frameBox.y, frameBox.width, frameBox.height, (x - frameBox.width), (y - frameBox.height), (looksRight ? 0 : ALLEGRO_FLIP_HORIZONTAL));
		al_draw_scaled_bitmap(currFilm->GetBitmap(), frameBox.x, frameBox.y, frameBox.width, frameBox.height, (x - (frameBox.width * SCALE_FACTOR)/2), (y - frameBox.height*SCALE_FACTOR), frameBox.width*SCALE_FACTOR, frameBox.height*SCALE_FACTOR, !looksRight?(ALLEGRO_FLIP_HORIZONTAL):0);
}

void Sprite::DeCollision() {
	Move(DecollisionNumber*(!looksRight ? 1 : -1), 0);
}

void Sprite::makeDamage(int dmg)
{
	auto Fighter = (mk::Fighter*) owner;
	assert(Fighter);
	assert(dmg >= 0);
	Fighter->damage(dmg);
}



void Sprite::Move(int dx, int dy) {
	x += dx;
	y -= dy;

	if (y > GROUND_LEVEL)
		y = GROUND_LEVEL;

	if ((x - COLLISION_OFFSET * 5) < 0)
		x = COLLISION_OFFSET * 5;
	if (x + frameBox.width * SCALE_FACTOR - COLLISION_OFFSET * 5 >= SCREEN_WIDTH)
		x = SCREEN_WIDTH - frameBox.width*SCALE_FACTOR;

	/*
	 if (gravityAddicted) {
		Check the terrain tiles just below;
		if (over solid terrain) {
			if (isFalling) {
				isFalling = false;
				NotifyStopFalling();
			}
		 }
		 else
		 if (!isFalling) {
		   isFalling = true;
		   NotifyStartFalling();
		}
	}
	*/

}

size_t Sprite::GetFrame(void) const { return frameNo; }

void Sprite::SetVisibility(bool v) { isVisible = v; }

bool Sprite::IsVisible(void) const { return isVisible; }

void Sprite::SetX(int _x)
{
	x = _x;
}

int Sprite::GetX() const
{
	return x;
}

void Sprite::SetY(int _y)
{
	y = _y;
}

int Sprite::GetY() const
{
	return y;
}

void Sprite::SetLookRight(bool _right)
{
	looksRight = _right;
}

bool Sprite::GetLooksRight() const
{
	return looksRight;
}

void Sprite::CollisionCheck(Sprite* s) {
	if (!s)	return;

	const int width = (GetLooksRight() ? 1 : -1) * ((frameBox.width * SCALE_FACTOR) - 2*COLLISION_OFFSET);
	const int height = (frameBox.height * SCALE_FACTOR) - 2*COLLISION_OFFSET;
	const int pos_x = (x + (frameBox.width * SCALE_FACTOR) / 2) - COLLISION_OFFSET;
	const int pos_y = y - COLLISION_OFFSET;

	const int other_width = (s->GetLooksRight() ? 1 : -1) * ((s->GetFrameBox().width * SCALE_FACTOR) - 2*COLLISION_OFFSET);
	const int other_height = (s->GetFrameBox().height * SCALE_FACTOR) - 2*COLLISION_OFFSET;
	const int other_pos_x = (s->GetX() + (s->GetFrameBox().width * SCALE_FACTOR) / 2) - COLLISION_OFFSET;
	const int other_pos_y = s->GetY() - COLLISION_OFFSET;


	const int x1 = pos_x + (GetLooksRight() ? -1 : -0) * (GetFrameBox().width * SCALE_FACTOR);
	const int y1 = pos_y;
	const int x2 = pos_x + width + (GetLooksRight() ? -1 : -0) * (GetFrameBox().width * SCALE_FACTOR);
	const int y2 = pos_y - height;

	const int other_x1 = other_pos_x + (s->GetLooksRight() ? -1 : -0) * (s->GetFrameBox().width * SCALE_FACTOR);
	const int other_y1 = other_pos_y;
	const int other_x2 = other_pos_x + other_width + (s->GetLooksRight() ? -1 : -0) * (s->GetFrameBox().width * SCALE_FACTOR);
	const int other_y2 = other_pos_y - other_height;



#ifdef DEBUG
	al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(0, 0, 0), 6);
	al_draw_rectangle(other_x1, other_y1, other_x2, other_y2, al_map_rgb(0, 0, 0), 6);
#endif // DEBUG


	const bool collide =
		((x1 < other_x1	&&
		x2 > other_x2) ||
		(x1 < other_x2	&&
		x2 > other_x1) ||
		(x2 < other_x1	&&
		x1 > other_x2) ||
		(x2 < other_x2	&&
		x1 > other_x1))
		&&
		y1 > other_y2	&&
		y2 < other_y1;


	if (collide)
		NotifyCollision(s);

}

Rect Sprite::GetFrameBox() const
{
	return frameBox;
}

void * Sprite::GetOwner() const
{
	return owner;
}

Sprite::Sprite(int _x, int _y, AnimationFilm* film, TYPE collisionType, void* Owner) :
	x(_x), y(_y), currFilm(film), isVisible(true), looksRight(false), owner(Owner),
	SpriteWithCollision(collisionType, STATE::IDLE)
{
	frameNo = currFilm->GetTotalFrames();
	SetFrame(0);
	SpriteHolder::Get()->Add(this);
}

Sprite::~Sprite() {
	CollisionChecker::Get()->Cancel(this);
	SpriteHolder::Get()->Remove(this);
}


SpriteHolder* SpriteHolder::holder = 0;

SpriteHolder* SpriteHolder::Get(void)
{
	if (holder == 0)
		holder = new SpriteHolder();
	return holder;
}
