#include "pch.h"

#include "AnimationFilmHolder.h"

AnimationFilmHolder* AnimationFilmHolder::holder = 0;		//Default singleton

AnimationFilmHolder::AnimationFilmHolder()
{
	//bitmaps = BitmapLoader::Get();
}

AnimationFilmHolder::~AnimationFilmHolder()
{
	CleanUp();
}


AnimationFilmHolder* AnimationFilmHolder::Get(void)
{
	if (holder == 0)
		holder = new AnimationFilmHolder();		//Init only one singleton
	return holder;
}


void AnimationFilmHolder::Load(const std::string & catalogue)
{
	ALLEGRO_CONFIG *animationFilms = al_load_config_file(catalogue.c_str());



	std::string path = al_get_config_value(animationFilms, "", "ANIMATION_CATALOGUE");
	ALLEGRO_CONFIG *bitmapRectangles = al_load_config_file(("../../sprites/" + path).c_str());

	std::string bitmap_path = al_get_config_value(bitmapRectangles, "", "BITMAP_NAME");
	ALLEGRO_BITMAP* bmp = BitmapLoader::Get()->Load(("../../sprites/" + bitmap_path).c_str());
	int i = 0;
	const char * id;

	std::map < std::string, bool> list;

	while (id = al_get_config_value(animationFilms, ("ANIMATION " + std::to_string(i)).c_str(), "NAME")) {
		std::string pos = al_get_config_value(animationFilms, ("ANIMATION " + std::to_string(i)).c_str(), "POS");

		std::stringstream test(pos);
		std::string segment;
		std::vector<Rect> rects;

		while (std::getline(test, segment, '.'))
		{
			list[segment] = true;

			if (atoi(segment.c_str()) == 172) {
				int f = 0;
			}

			auto x(al_get_config_value(bitmapRectangles, ("SPRITE " + segment).c_str(), "X"));
			auto y(al_get_config_value(bitmapRectangles, ("SPRITE " + segment).c_str(), "Y"));
			auto w(al_get_config_value(bitmapRectangles, ("SPRITE " + segment).c_str(), "WIDTH"));
			auto h(al_get_config_value(bitmapRectangles, ("SPRITE " + segment).c_str(), "HEIGHT"));

			assert(x);
			assert(y);
			assert(w);
			assert(h);

			rects.push_back(Rect{
				atoi(x),
				atoi(y),
				atoi(w),
				atoi(h)
				});
		}

		assert(!GetFilm(id));
		films[id] = new AnimationFilm(bmp, rects, id);

		i++;
	}





	al_destroy_config(bitmapRectangles);
	al_destroy_config(animationFilms);
	//assert(false);
}

void AnimationFilmHolder::CleanUp(void)
{
	for (FILMS::iterator i = films.begin(); i != films.end(); i++)
		delete(i->second);
	films.clear();
}

AnimationFilm * AnimationFilmHolder::GetFilm(const std::string id) const
{
	FILMS::const_iterator i = films.find(id);
	return i != films.end() ? i->second : (AnimationFilm*)0;
}

void AnimationFilmHolder::makeSheets(std::string output_path, ALLEGRO_DISPLAY * display)
{

	for(auto i2:films) {
		auto name = i2.first;
		auto film = i2.second;

		int maxHeight = 0;
		int sumWidth = 0;
		int i;
		for (i = 0; i < film->GetTotalFrames(); i++) {
			maxHeight = film->GetFrameBox(i).height > maxHeight ? film->GetFrameBox(i).height : maxHeight;
			sumWidth += film->GetFrameBox(i).width;
		}
		
		ALLEGRO_BITMAP* output = al_create_bitmap(sumWidth+(i*5), maxHeight+50);
		al_set_target_bitmap(output);


		int toX = 0;
		for (int i = 0; i < film->GetTotalFrames(); i++) {
			ALLEGRO_BITMAP* sub = al_create_sub_bitmap(film->GetBitmap(), film->GetFrameBox(i).x, film->GetFrameBox(i).y,
				film->GetFrameBox(i).width, film->GetFrameBox(i).height);

			al_draw_bitmap(sub, toX, 25, 0);
			toX += film->GetFrameBox(i).width + 5;

			al_destroy_bitmap(sub);
		}

		//save
		al_save_bitmap((output_path+"/"+name+".png").c_str(), output);

		al_set_target_bitmap(al_get_backbuffer(display));
		al_destroy_bitmap(output);

	}

}



