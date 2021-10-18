#include "pch.h"
#include "AudioPlayer.h"





AudioPlayer* AudioPlayer::loader = 0;
AudioPlayer::AudioPlayer()
{}


AudioPlayer::~AudioPlayer()
{
	CleanUp();
}

ALLEGRO_SAMPLE * AudioPlayer::GetSample(const std::string path) const
{
	AUDIOS::const_iterator i = audios.find(path);
	return i != audios.end() ? i->second : (ALLEGRO_SAMPLE *)0;
}

ALLEGRO_SAMPLE * AudioPlayer::Load(const std::string & path)
{
	ALLEGRO_SAMPLE *s = GetSample(path);
	if (!s) {
		s = al_load_sample(path.c_str());
		audios[path] = s;
		
		if (!s) printf("Audio clip sample not loaded!\n");
		
		assert(s);
	}
	return  s;
}

void AudioPlayer::CleanUp(void)
{
	for (AUDIOS::iterator i = audios.begin(); i != audios.end(); i++)
		al_destroy_sample(i->second);
	audios.clear();
}

void AudioPlayer::Play(const std::string & path, bool loop) const
{
	al_play_sample(AudioPlayer::Get()->Load(path), 1.0, 0.0, 1.0, (loop ? ALLEGRO_PLAYMODE_LOOP : ALLEGRO_PLAYMODE_ONCE), NULL);
}

AudioPlayer * AudioPlayer::Get()
{
	if (!loader)
		loader = new  AudioPlayer();
	return loader;
}


