#pragma once
class AudioPlayer
{
public:


private:
	typedef std::map<std::string, ALLEGRO_SAMPLE*> AUDIOS;
	AUDIOS audios;
	ALLEGRO_SAMPLE* GetSample(const std::string path) const;

	static AudioPlayer* loader;
	AudioPlayer();
public:
	ALLEGRO_SAMPLE* Load(const std::string& path);
	void CleanUp(void);
	void Play(const std::string& path,bool loop)const;

	static AudioPlayer* Get();

	~AudioPlayer();
};

