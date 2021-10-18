// AS, CS454, Project MK (SNES version), code snippets.
// Version 2.
// December 2018, unit tested (partial), unrefactored.
// K:\Docs\Lectures\HY454\Content\Project\Project 2018\MK

#include "pch.h"

#include "AnimationFilm.h"
#include "AnimationFilmHolder.h"
#include "BitmapLoader.h"

#include "AudioPlayer.h"

#include "InputController.h"
#include "StateTransitions.h"

#include "Fighter.h"

#include "Sprite.h"

#include "FrameRangeAnimator.h"
#include "FrameRangeAnimation.h"

#include "MovingAnimation.h"
#include "MovingAnimator.h"

#include "AnimatorFunctions.h"
#include "CollisionChecker.h"





mk::Fighter *f0 = nullptr;
mk::Fighter *f1 = nullptr;






std::list<std::string> buttonsList;
std::list<std::string> PressedbuttonsList;


enum GameStates {
	TITLE,
	MAINGAME,

	ENDSCREEN
};


void draw_titlescreen() {

	ALLEGRO_BITMAP* title_screen = NULL;
	title_screen = BitmapLoader::Get()->Load("../smooth.jpg");


	ALLEGRO_FONT *fancy_font19 = al_load_font("../mk1.ttf", 19, 0);
	ALLEGRO_FONT *fancy_font35 = al_load_font("../mk1.ttf", 35, 0);

	al_draw_bitmap(title_screen, 0, 0, 0);

	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 83, 70, 0, "TEAM KOKOLAN");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 83, 100, 0, "University");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 83, 120, 0, "Of Crete");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 83, 150, 0, "Department");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 83, 170, 0, "of Computer");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 83, 190, 0, "Science");


	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 1039, 70, 0, "CS - 454");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 1039, 100, 0, "Development");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 1039, 120, 0, "Of Intelligent");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 1039, 140, 0, "Interfaces");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 1039, 160, 0, "and games");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 1039, 190, 0, "Fall Semester");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 1039, 210, 0, "2018");

	/*
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 55, 280, 0, "TEAM KOKOU");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 55, 310, 0, "University");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 55, 330, 0, "Of Crete");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 55, 360, 0, "Department");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 55, 380, 0, "of Computer");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 55, 400, 0, "Science");


	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 645, 280, 0, "CS - 454");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 645, 310, 0, "Development");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 645, 330, 0, "Of Intelligent");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 645, 350, 0, "Interfaces");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 645, 370, 0, "and games");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 645, 400, 0, "Fall Semester");
	al_draw_text(fancy_font19, al_map_rgb(230, 230, 0), 645, 420, 0, "2018");
	*/

	al_draw_text(fancy_font35, al_map_rgb(0, 230, 0), 513, 420, 0, "PRESS ENTER");

	al_destroy_font(fancy_font19);
	al_destroy_font(fancy_font35);
}

void draw_end_screen() {

	ALLEGRO_BITMAP* title_screen = NULL;
	title_screen = BitmapLoader::Get()->Load("../gameovermk.jpg");
	al_draw_bitmap(title_screen, 0, 0, 0);

}

void drawmedal() {
	ALLEGRO_BITMAP* medal = BitmapLoader::Get()->Load("../../sprites/medal.png");
	ALLEGRO_BITMAP* SUB_WIN = BitmapLoader::Get()->Load("../../sprites/SUBZEROWINS.jpg");
	ALLEGRO_BITMAP* SCORP_WIN = BitmapLoader::Get()->Load("../../sprites/SCORPIONWINS.jpg");



	if (f0->GetWins() == 1)
	{
		al_draw_scaled_bitmap(medal, 0, 0, 163, 12, 520, 61, SCREEN_WIDTH / 3, 12, 0);
	}
	else if (f0->GetWins() >= 2)
	{
		al_draw_scaled_bitmap(medal, 0, 0, 163, 12, 520, 61, SCREEN_WIDTH / 3, 12, 0);
		al_draw_scaled_bitmap(medal, 0, 0, 163, 12, 550, 61, SCREEN_WIDTH / 3, 12, 0);
		al_draw_bitmap(SCORP_WIN, 540, 200, 0);
	}

	if (f1->GetWins() == 1)
	{
		al_draw_scaled_bitmap(medal, 0, 0, 163, 12, 1130, 61, SCREEN_WIDTH / 3, 12, 0);
	}
	else if (f1->GetWins() >= 2)
	{
		al_draw_scaled_bitmap(medal, 0, 0, 163, 12, 1130, 61, SCREEN_WIDTH / 3, 12, 0);
		al_draw_scaled_bitmap(medal, 0, 0, 163, 12, 1160, 61, SCREEN_WIDTH / 3, 12, 0);
		al_draw_bitmap(SUB_WIN, 540, 200, 0);
	}
}


void drawMainScreen() {



	ALLEGRO_BITMAP* bg = BitmapLoader::Get()->Load("../Arena.png");
	ALLEGRO_BITMAP* FULL_BAR = BitmapLoader::Get()->Load("../../sprites/FULL_LIFE_BAR.jpg");
	ALLEGRO_BITMAP* HALF_BAR = BitmapLoader::Get()->Load("../../sprites/HALF_LIFE_BAR.jpg");
	ALLEGRO_BITMAP* QUARTER_BAR = BitmapLoader::Get()->Load("../../sprites/QUARTER_LIFE_BAR.jpg");
	ALLEGRO_BITMAP* THREE_QUARTER_BAR = BitmapLoader::Get()->Load("../../sprites/3QUARTER_LIFE_BAR.jpg");
	ALLEGRO_BITMAP* EMPTY_BAR = BitmapLoader::Get()->Load("../../sprites/EMPTY_LIFE_BAR.jpg");
	ALLEGRO_BITMAP* SubZero = BitmapLoader::Get()->Load("../../sprites/sub_name.png");
	ALLEGRO_BITMAP* Scorpion = BitmapLoader::Get()->Load("../../sprites/Scorpion_name.png");



	al_draw_scaled_bitmap(bg, 0, 0, 400, 254, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	al_draw_scaled_bitmap(Scorpion, -33, -10, 400, 254, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	al_draw_scaled_bitmap(SubZero, -233, -10, 400, 254, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	f0->calculateLook(f1);
	f1->calculateLook(f0);

	f0->getSprite()->Display(NULL);
	f1->getSprite()->Display(NULL);

	if (f0->getCurrentHP() == INITIAL_MAX_FIGHTER_HP)
		al_draw_scaled_bitmap(FULL_BAR, 0, 0, 163, 12, 100, 60, SCREEN_WIDTH / 3, 12, 0);
	else if (f0->getCurrentHP() == INITIAL_MAX_FIGHTER_HP - 25)
		al_draw_scaled_bitmap(THREE_QUARTER_BAR, 0, 0, 163, 12, 100, 60, SCREEN_WIDTH / 3, 12, 0);
	else if (f0->getCurrentHP() == INITIAL_MAX_FIGHTER_HP / 2)
		al_draw_scaled_bitmap(HALF_BAR, 0, 0, 163, 12, 100, 60, SCREEN_WIDTH / 3, 12, 0);
	else if (f0->getCurrentHP() == INITIAL_MAX_FIGHTER_HP / 4)
		al_draw_scaled_bitmap(QUARTER_BAR, 0, 0, 163, 12, 100, 60, SCREEN_WIDTH / 3, 12, 0);
	else if (f0->getCurrentHP() <= 0) {
		al_draw_scaled_bitmap(EMPTY_BAR, 0, 0, 163, 12, 100, 60, SCREEN_WIDTH / 3, 12, 0);
	}

	if (f1->getCurrentHP() == INITIAL_MAX_FIGHTER_HP)
		al_draw_scaled_bitmap(FULL_BAR, 0, 0, 163, 12, 713, 60, SCREEN_WIDTH / 3, 12, ALLEGRO_FLIP_HORIZONTAL);
	else if (f1->getCurrentHP() == 75)
		al_draw_scaled_bitmap(THREE_QUARTER_BAR, 0, 0, 163, 12, 713, 60, SCREEN_WIDTH / 3, 12, ALLEGRO_FLIP_HORIZONTAL);
	else if (f1->getCurrentHP() == INITIAL_MAX_FIGHTER_HP / 2)
		al_draw_scaled_bitmap(HALF_BAR, 0, 0, 163, 12, 713, 60, SCREEN_WIDTH / 3, 12, ALLEGRO_FLIP_HORIZONTAL);
	else if (f1->getCurrentHP() == INITIAL_MAX_FIGHTER_HP / 4)
		al_draw_scaled_bitmap(QUARTER_BAR, 0, 0, 163, 12, 713, 60, SCREEN_WIDTH / 3, 12, ALLEGRO_FLIP_HORIZONTAL);
	else if (f1->getCurrentHP() <= 0) {
		al_draw_scaled_bitmap(EMPTY_BAR, 0, 0, 163, 12, 713, 60, SCREEN_WIDTH / 3, 12, ALLEGRO_FLIP_HORIZONTAL);
	}

	drawmedal();
	CollisionChecker::Get()->Check();

}

//DRAW FUNCTIONS
void draw(GameStates game_state) {
	switch (game_state)
	{
	case TITLE:
		draw_titlescreen();
		break;
	case MAINGAME:
		drawMainScreen();
		break;
	case ENDSCREEN:
		draw_end_screen();
		break;
	default:
		assert(false);
	}
}

int once;
GameStates current_game_state;

void ResetRound() {

	f0->setSprite(100, 450, "standard", TYPE::FIGHTER_0);
	f1->setSprite(300, 450, "standard", TYPE::FIGHTER_1);

	f1->setAnimator(nullptr);
	f0->setAnimator(nullptr);

	f0->setState(mk::READY);
	f1->setState(mk::READY);

	once = 0;

	AnimatorFunctions::makeAnimation(f0);
	AnimatorFunctions::makeAnimation(f1);

	f0->ResetCurrentHP();
	f1->ResetCurrentHP();
}

void GetFatalitied(mk::Fighter* from) {
	auto fatality0 = (f0->getName() == "SCORPION" ? mk::SKELETON_F : mk::BLUE_DESTRUCTION_F);
	auto fatality1 = (f1->getName() == "SUB0" ? mk::BLUE_DESTRUCTION_F : mk::SKELETON_F);

	if (from == f0) {
		f1->setState(fatality0);
		AnimatorFunctions::makeAnimation(f1);
	}
	else if (from == f1) {
		f0->setState(fatality1);
		AnimatorFunctions::makeAnimation(f0);
	}


}

void GameEnd() {
	current_game_state = ENDSCREEN;
}

int main() {


	f0 = new mk::Fighter(0, "SCORPION");
	f1 = new mk::Fighter(1, "SUB0");

	bool finish = false;
	bool redraw = false;
	bool checkInput = false;

	ALLEGRO_DISPLAY		*display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER		*timer = NULL;


	if (!al_init())
		return -1;

	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}

	if (!al_init_acodec_addon()) {
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return -1;
	}

	if (!al_reserve_samples(2)) {
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}



	AudioPlayer::Get()->Play("../../sounds/menusound.ogg", true);

	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!display)
		return -1;

	al_install_keyboard();

	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();




	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//CONFIG TESTS HERE//
	AnimationFilmHolder::Get()->Load("../../sprites/sprite_count_sub0.txt");
	AnimationFilmHolder::Get()->Load("../../sprites/sprite_count_scorpion.txt");

	//AnimationFilmHolder::Get()->makeSheets("../../SHEETS/", display);

	f0->loadKeys("../basic_keys.cfg");
	f0->initInputController();
	f0->initStateTransitions();

	f1->loadKeys("../basic_keys.cfg");
	f1->initInputController();
	f1->initStateTransitions();


	f0->setSprite(100, 450, "standard", TYPE::FIGHTER_0);
	f1->setSprite(300, 450, "standard", TYPE::FIGHTER_1);



	AnimatorFunctions::makeAnimation(f0);
	AnimatorFunctions::makeAnimation(f1);


	//al_play_sample(sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	//
	current_game_state = TITLE;

	//TIMERS
	double currentTime = 0;
	double timeForInput = 0;
	bool paused = false;
	timestamp_t timeBeforePause = 0;
	once = 0;

	PressedbuttonsList.clear();
	buttonsList.clear();
	al_start_timer(timer);
	while (!finish) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//		if(ev.type== ALLEGRO_EVENT_KEY_CHAR)
		//			printf("\n%d",ev.keyboard.unichar);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			//insert the keycode name in the button list

			if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
				if (current_game_state == TITLE)
				{
					al_stop_samples();
					AudioPlayer::Get()->Play("../../sounds/fight.ogg", false);
					current_game_state = MAINGAME;
				}
				else if (current_game_state == MAINGAME)
				{
					current_game_state = ENDSCREEN;
				}
				else
				{
					finish = true;
				}
			}
			else if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				finish = true;
			}
			else if (ev.keyboard.keycode == ALLEGRO_KEY_P) {
				paused = !paused;
				if (paused)
					timeBeforePause = GameTimer::GetGameTime();
				else
					AnimatorHolder::TimeShift(GameTimer::GetGameTime() - timeBeforePause);
			}
			else if (!paused)
				buttonsList.push_back(al_keycode_to_name(ev.keyboard.keycode));
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			PressedbuttonsList.remove(al_keycode_to_name(ev.keyboard.keycode));
		}

		// X to close the programm
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
			finish = true;

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			GameTimer::SetGameTime(GameTimer::GetCurrTime());
			redraw = true;
			/*
						if (f0->isDead() || f1->isDead()) {
							current_game_state = ENDSCREEN;
						}
			*/
			if (current_game_state != TITLE && current_game_state != ENDSCREEN && !paused) {
				AnimatorHolder::Progress(GameTimer::GetGameTime());
				checkInput = true;

				CollisionChecker::Get()->Check();
				DestructionManager::Commit();
			}
		}

		if (checkInput) {
			//if the buttonlist isn't empty
			for (auto i : PressedbuttonsList)
				buttonsList.push_front(i);

			PressedbuttonsList.clear();


			input::InputController::HandleAll();
			//buttonsList.clear();
			for (auto i : buttonsList)
				if (i == f0->translateKey("FORWARD") || i == f0->translateKey("BACK") || i == f0->translateKey("DOWN") || i == f0->translateKey("BLK") ||
					i == f1->translateKey("FORWARD") || i == f1->translateKey("BACK") || i == f1->translateKey("DOWN") || i == f1->translateKey("BLK"))
					PressedbuttonsList.push_back(i);

			buttonsList.clear();

			f0->performTransitions();
			f1->performTransitions();
			checkInput = false;
		}

		if (redraw && al_event_queue_is_empty(event_queue)) {
			redraw = false;

			draw(current_game_state);



			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}

		if (f0->isDead() && once == 0)
		{
			once++;
			f1->AddWin();
			if (f1->getCurrentHP() == 100 && f1->GetWins() == 1)
			{
				AudioPlayer::Get()->Play("../../sounds/flawless_victory.ogg", false);
			}
			if (f1->GetWins() >= 2) {
				AudioPlayer::Get()->Play("../../sounds/sub_zero_wins.ogg", false);
				f1->setState(mk::FATALITY_SUB);
				f0->setState(mk::DIZZY);
			}
			else {
				f0->setState(mk::FALL_DOWN);
				f1->setState(mk::WINNING_POS);

				AnimatorFunctions::makeAnimation(f0);
				AnimatorFunctions::makeAnimation(f1);
			}
		}
		else if (f1->isDead() && once == 0)
		{
			f0->AddWin();
			if (f0->getCurrentHP() == 100 && f0->GetWins() == 1)
			{
				AudioPlayer::Get()->Play("../../sounds/flawless_victory.ogg", false);
			}
			once++;
			if (f0->GetWins() >= 2) {
				AudioPlayer::Get()->Play("../../sounds/scorpion_wins.ogg", false);
				f1->setState(mk::DIZZY);
				f0->setState(mk::FATALITY_SCORPION);
			}
			else {
				f1->setState(mk::FALL_DOWN);
				f0->setState(mk::WINNING_POS);
			}
			AnimatorFunctions::makeAnimation(f1);
			AnimatorFunctions::makeAnimation(f0);
		}

	}
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

	delete f0;
	delete f1;
	DestructionManager::Commit();

	delete AnimationFilmHolder::Get();
	delete CollisionChecker::Get();
	delete SpriteHolder::Get();
	delete BitmapLoader::Get();


	system("PAUSE");

	return 0;
}

//check if for each keycode sent theres a match in the buttonlist
bool input::test_key(const std::string& keyCode) {
	const auto findIter = std::find(buttonsList.begin(), buttonsList.end(), keyCode);
	return findIter != buttonsList.end();
}

