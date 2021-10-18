#include "music.h"

ALLEGRO_SAMPLE		*sound = NULL;

if (!al_install_audio()) {
	fprintf(stderr, "failed to initialize audio!\n");
	return -1;
}

if (!al_init_acodec_addon()) {
	fprintf(stderr, "failed to initialize audio codecs!\n");
	return -1;
}

if (!al_reserve_samples(1)) {
	fprintf(stderr, "failed to reserve samples!\n");
	return -1;
}


void playMusic(std::string track) {

	if (track == "menu")
	{
		sound = al_load_sample(music_paths[0]);
		al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	}
	else if (track == "scorpion")
	{
		sound = al_load_sample(music_paths[1]);
		al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
	else
	{
		sound = al_load_sample(music_paths[2]);
		al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
}