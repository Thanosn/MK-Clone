#include "pch.h"

#include "BitmapLoader.h"



BitmapLoader* BitmapLoader::loader = 0;
BitmapLoader::BitmapLoader()
{}


BitmapLoader::~BitmapLoader()
{
	CleanUp();
}

ALLEGRO_BITMAP * BitmapLoader::GetBitmap(const std::string path) const
{
	BITMAPMAP::const_iterator i = bitmaps.find(path);
	return i != bitmaps.end() ? i->second : (ALLEGRO_BITMAP *)0;
}

ALLEGRO_BITMAP * BitmapLoader::Load(const std::string & path)
{
	ALLEGRO_BITMAP *b = GetBitmap(path);
	if (!b) {
		b = al_load_bitmap(path.c_str());
		bitmaps[path] = b;
		assert(b);
	}
	return  b;
}

void BitmapLoader::CleanUp(void)
{
	for (BITMAPMAP::iterator i = bitmaps.begin(); i != bitmaps.end(); i++)
		al_destroy_bitmap(i->second);
	bitmaps.clear();
}

BitmapLoader * BitmapLoader::Get()
{
	if (!loader)
		loader = new  BitmapLoader();
	return loader;
}
