#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <list>
#include <vector>
#include <tuple>
#include <fstream>
#include <string>
#include <sstream>

#include "mappy_A5.h"

#include "Background.h"
#include "Enemy.h"
#include "Globals.h"
#include "Person.h"
#include "PickUp.h"
#include "PopUp.h"
#include "Projectile.h"
#include "QuickSort.h"


bool keys[] = {false, false, false, false, false};
enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE};
typedef tuple<string, int, string> MyTuple;

//==============================================
//GLOBALS
//==============================================
Person* boy;
Enemy* cancer;
std::list<GameObject *> objects;
std::list<GameObject *>::iterator iter;
std::list<GameObject *>::iterator iter2;
Background *titleScreen;
Background *lostScreen;
PickUp *diamondPickUps;
PopUp *courageScreen;
PopUp *wisdomScreen;
PopUp *honestyScreen;
PopUp *cancerScreen;
vector<MyTuple> HighScoresVector;
string* nameOfPlayer;

void ChangeState(int &state, int newState);
void ChangeChallenge(int &challenge, int newChallenge);
void GetTilesTouchingBounds(Person* myPerson, int xOff);
void CheckForPickUps(Person* myPerson, int& challenge, int xOff);
void RenderPopUpPerChallege(int& challenge);
string getDate();

int main(void)
{
	//==============================================
	//SHELL VARIABLES
	//==============================================
	bool done = false;
	bool render = false;	

	int xOff = 0;

	//==============================================
	//PROJECT VARIABLES
	//==============================================
	boy = new Person();

	int state = -1;
	int challenge = -1;

	ALLEGRO_BITMAP *boyImage = NULL;
	ALLEGRO_BITMAP *titleImage = NULL;
	ALLEGRO_BITMAP *lostImage = NULL;
	ALLEGRO_BITMAP *diamondImage = NULL;
	ALLEGRO_BITMAP *courageImage = NULL;
	ALLEGRO_BITMAP *wisdomImage = NULL;
	ALLEGRO_BITMAP *honestyImage = NULL;
	ALLEGRO_BITMAP *cancerImage = NULL;
	ALLEGRO_BITMAP *cancerCharacter = NULL;
	ALLEGRO_BITMAP *heartImage = NULL;

	//==============================================
	//ALLEGRO VARIABLES
	//==============================================
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font18, *font100;

	//==============================================
	//ALLEGRO INIT FUNCTIONS
	//==============================================
	if(!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if(!display)										//test display object
		return -1;

	//==============================================
	//ADDON INSTALL
	//==============================================
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();

	//==============================================
	//PROJECT INIT
	//==============================================
	font18 = al_load_font("arial.ttf", 18, 0);
	font100 = al_load_font("arial.ttf", 24, 0);

	boyImage = al_load_bitmap("boy_sprites.png");
	boy->Init(boyImage);
	objects.push_back(boy);

	titleImage = al_load_bitmap("THON_Title.png");
	lostImage = al_load_bitmap("High_Scores.png");
	diamondImage = al_load_bitmap("diamond_image.png");
	courageImage = al_load_bitmap("courage_image.png");
	wisdomImage = al_load_bitmap("wisdom_image.png");
	honestyImage = al_load_bitmap("honesty_image.png");
	cancerImage = al_load_bitmap("cancer_image.png");
	cancerCharacter = al_load_bitmap("cancer_character.png");
	heartImage = al_load_bitmap("heart.png");

	titleScreen = new Background(titleImage);
	lostScreen = new Background(lostImage);
	diamondPickUps = new PickUp(diamondImage);
	courageScreen = new PopUp(courageImage);
	wisdomScreen = new PopUp(wisdomImage);
	honestyScreen = new PopUp(honestyImage);
	cancerScreen = new PopUp(cancerImage);

	cancer = new Enemy(cancerCharacter);

	objects.push_back(diamondPickUps);

	if(MapLoad("First Test map.fmp", 1))
		return -5;

	ChangeState(state, TITLE);
	ChangeChallenge(challenge, NONE);

	srand(time(NULL));
	//==============================================
	//TIMER INIT AND STARTUP
	//==============================================
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);
	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//==============================================
		//INPUT
		//==============================================
		if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				boy->Jump();
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				if(state == TITLE)
				{
					if (nameOfPlayer->empty())		// Keeps "HighScores.txt" from causing error
						*nameOfPlayer = "Anonymous";
					ChangeState(state, PLAYING);
				}
				else if(state == PLAYING)
				{
					if(challenge == COURAGE)
					{
						diamondPickUps->AddItem();
						ChangeChallenge(challenge, NONE);
					}
					else if (challenge == CANCER)
					{
						ChangeChallenge(challenge, BATTLE);
					}
					else if(challenge == BATTLE)
					{
						Projectile* heart = new Projectile(boy->GetX() + 16, boy->GetY(), heartImage);
						objects.push_back(heart);
					}
				}
				else if(state == GAMEOVER)
					ChangeState(state, TITLE);
				break;
			case CORRECT_WISDOM_ANSWER:
				if (challenge == WISDOM)
				{
					diamondPickUps->AddItem();
					boy->AddPoint();
					ChangeChallenge(challenge, NONE);
				}
				break;
			case CORRECT_HONESTY_ANSWER:
				if (challenge == HONESTY)
				{
					diamondPickUps->AddItem();
					ChangeChallenge(challenge, NONE);
				}
				break;

			case ALLEGRO_KEY_V:
				boy->LoseLife();
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			char unichar = ev.keyboard.unichar;

			if (challenge == WISDOM)
				if (unichar != '4' || unichar != '4')
					ChangeChallenge(challenge, NONE);
			if (challenge == HONESTY)
				if (unichar != 'b' || unichar != 'B')
					ChangeChallenge(challenge, NONE);
			if (state == TITLE)
			{
				if ((unichar >= 32) && (unichar < 256) && (nameOfPlayer->length() < 8))
					*nameOfPlayer += unichar;
				if (unichar == 8 && *nameOfPlayer != "")	// Allows backspace of characters
					nameOfPlayer->pop_back();
			}
		}
		else if(ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;

			xOff += keys[RIGHT] * 4;
			xOff -= keys[LEFT] * 4;

			if(xOff < 0)
				xOff = 0;
			if(xOff > (mapwidth*mapblockwidth - WIDTH))
				xOff = mapwidth*mapblockwidth - WIDTH;

			if(state == PLAYING && (challenge == NONE || challenge == BATTLE))
			{
				if(challenge == BATTLE)
					if(cancer->GetLives() == 0)
						ChangeState(state, GAMEOVER);
				
				if(boy->GetLives() == 0)
					ChangeState(state, GAMEOVER);

				//GET BLOCK FOR EACH FRAME===============
				GetTilesTouchingBounds(boy, xOff);

				CheckForPickUps(boy, challenge, xOff);

				// prevents bouncing jumps
				if(boy->OffGround() || !keys[UP])
					boy->ResetAnimation(1);

				if(keys[LEFT])
					boy->MoveLeft();
				else if(keys[RIGHT])
					boy->MoveRight();
				else
					boy->ResetAnimation(0);


				//UPDATE=======================
				for(iter = objects.begin(); iter != objects.end(); ++iter)
					(*iter)->Update();

				//COLLISIONS====================

				for(iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if( ! (*iter)->Collidable() ) continue;

					for(iter2 = iter; iter2 != objects.end(); ++iter2)
					{
						if( !(*iter2)->Collidable() ) continue;
						if( (*iter)->GetID() == (*iter2)->GetID()) continue;

						if( (*iter)->CheckCollisions( (*iter2)))
						{
							(*iter)->Collided( (*iter2)->GetID() );
							(*iter2)->Collided( (*iter)->GetID() );
						}
					}
				}


				//CULL THE DEAD==================
				for(iter = objects.begin(); iter != objects.end(); )
				{
					if(! (*iter)->GetAlive())
					{
						delete (*iter);
						iter = objects.erase(iter);
					}
					else
						iter++;
				}
			}
		}

		//==============================================
		//RENDER
		//==============================================
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			//BEGIN PROJECT RENDER================
			if(state == TITLE)
			{
				titleScreen->Render();

				al_draw_text(font100, al_map_rgb(255, 255, 255), 275, 265, 0, nameOfPlayer->c_str());
			}
			else if(state == PLAYING)
			{
				MapDrawBG(xOff, 0, 0, 0, WIDTH, HEIGHT);

				al_draw_textf(font18, al_map_rgb(2, 0, 255), 5, 5, 0, "Player has earned $%i", boy->GetScore());

				for(iter = objects.begin(); iter != objects.end(); ++iter)
					(*iter)->Render();

				RenderPopUpPerChallege(challenge);

			}
			else if(state == GAMEOVER)
			{
				//DISPLAY HIGH SCORE INFORMATION================
				lostScreen->Render();
				
				std::ostringstream osPlayerScore;
				osPlayerScore << "$" << boy->GetScore();
				string sPlayerScore = osPlayerScore.str();


				al_draw_textf(font18, al_map_rgb(255, 255, 255), 158, 179, 0, sPlayerScore.c_str());

				string name, date;
				int score;

				int j;
				(HighScoresVector.size() < 10)? (j = HighScoresVector.size()) : j = 10;

				for (int k = 0; k < j; k++)
				{
					name = get<0>(HighScoresVector[k]);
					score = get<1>(HighScoresVector[k]);
					date = get<2>(HighScoresVector[k]);

					std::ostringstream osName, osScore, osDate;
					osName << (k + 1) << ". " << name;
					osScore << "$" << score;
					string sName = osName.str();
					string sScore = osScore.str();

					al_draw_textf(font18, al_map_rgb(255, 255, 255), 325, (90+(20*k)), 0, sName.c_str());
					al_draw_textf(font18, al_map_rgb(255, 255, 255), 450, (90+(20*k)), 0, sScore.c_str());
					al_draw_textf(font18, al_map_rgb(255, 255, 255), 555, (90+(20*k)), 0, date.c_str());
				}
			}

			//FLIP BUFFERS========================
			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}

	MapFreeMem();
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}

void ChangeState(int &state, int newState)
{
	state = newState;

	if(state == TITLE)
	{
		nameOfPlayer = new string;
		*nameOfPlayer = "";
	}
	else if(state == PLAYING)
	{
		boy->Init();
		boy->ReadName(*nameOfPlayer);
		delete nameOfPlayer;

	}
	else if(state == GAMEOVER)
	{
		using namespace std;

		ifstream highScoresIn;
		ofstream highScoresOut;
		string playerName;
		int playerScore;
		string date;

		highScoresIn.open("HighScores.txt");
		if (HighScoresVector.empty())
			while(!highScoresIn.eof())
			{
				highScoresIn >> playerName >> playerScore >> date;
				HighScoresVector.push_back(make_tuple(playerName, playerScore, date));
			}
			highScoresIn.close();

			// Add player's score to high scores vector
			HighScoresVector.push_back(make_tuple(boy->GetName(), boy->GetScore(), getDate()));

			// Add player's score to high score file
			highScoresOut.open("HighScores.txt", ios_base::app);
			highScoresOut << boy->GetName() << "\n" << boy->GetScore() << "\n" << getDate() << "\n\n";
			highScoresOut.close();

			QuickSort(HighScoresVector, 0, HighScoresVector.size() - 1);
	}
}

void ChangeChallenge(int &challenge, int newChallenge)
{
	challenge = newChallenge;

	if(challenge == NONE)
	{}
	else if(challenge == COURAGE)
	{}
	else if(challenge == WISDOM)
	{}
	else if(challenge == HONESTY)
	{}
	else if(challenge == CANCER)
	{}
	else if(challenge == BATTLE)
	{
		objects.push_back(cancer);
	}
}

void GetTilesTouchingBounds(Person* myPerson, int xOff)
{
	myPerson->topLeftBlock = MapGetBlockInPixels(myPerson->GetX() + xOff, myPerson->GetY());
	myPerson->topRightBlock = MapGetBlockInPixels(myPerson->GetRightOfBoundX() + xOff, myPerson->GetY());
	myPerson->bottomLeftBlock = MapGetBlockInPixels(myPerson->GetX() + xOff, myPerson->GetBottomOfBoundY());
	myPerson->bottomRightBlock = MapGetBlockInPixels(myPerson->GetRightOfBoundX()+ xOff, myPerson->GetBottomOfBoundY());
}

void CheckForPickUps(Person* myPerson, int& challenge, int xOff)
{
	std::list<BLKSTR *>::iterator blockIter;
	int corner = 0;
	enum CORNERS{TL, TR, BL, BR};

	myPerson->GetCornerBlocks();

	// Loops through list and checks if myPerson is touching a pickup item, responds accordingly
	for (blockIter = myPerson->cornerBlocks.begin(); blockIter != myPerson->cornerBlocks.end(); blockIter++)
	{
		if (((*blockIter)->user1 >= 2) && ((*blockIter)->user1 <= 6))
		{
			switch((*blockIter)->user1)
			{
			case DIAMOND:
				ChangeChallenge(challenge, COURAGE);
				break;
			case BRAIN:
				ChangeChallenge(challenge, WISDOM);
				break;
			case WALLET:
				ChangeChallenge(challenge, HONESTY);
				break;
			case TRIGGER:
				ChangeChallenge(challenge, CANCER);
				break;
			case MONEY:
				myPerson->AddPoint();
				break;
			}

			// Each case changes the block to an background block, then sets block for myPerson to new one
			switch (corner)
			{
			case TL:
				MapSetBlockInPixels(myPerson->GetX() + xOff, myPerson->GetY(), BACKGROUND_TILE);
				(*blockIter) = MapGetBlockInPixels(myPerson->GetX() + xOff, myPerson->GetY());
				break;
			case TR:
				MapSetBlockInPixels(myPerson->GetRightOfBoundX()+ xOff, myPerson->GetY(), BACKGROUND_TILE);
				(*blockIter) = MapGetBlockInPixels(myPerson->GetRightOfBoundX()+ xOff, myPerson->GetY());
				break;
			case BL:
				MapSetBlockInPixels(myPerson->GetX() + xOff, myPerson->GetBottomOfBoundY(), BACKGROUND_TILE);
				(*blockIter) = MapGetBlockInPixels(myPerson->GetX() + xOff, myPerson->GetBottomOfBoundY());
				break;
			case BR:
				MapSetBlockInPixels(myPerson->GetRightOfBoundX()+ xOff, myPerson->GetBottomOfBoundY(), BACKGROUND_TILE);
				(*blockIter) = MapGetBlockInPixels(myPerson->GetRightOfBoundX()+ xOff, myPerson->GetBottomOfBoundY());
				break;
			}				
		}

		corner++;
	}
}

void RenderPopUpPerChallege(int& challenge)
{
	switch(challenge)
	{
	case COURAGE:
		courageScreen->Render();
		break;
	case WISDOM:
		wisdomScreen->Render();
		break;
	case HONESTY:
		honestyScreen->Render();
		break;
	case CANCER:
		cancerScreen->Render();
		break;
	}
}

string getDate()
{
	std::ostringstream os;
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	os << (now->tm_mon + 1) << '/' 
		<< now->tm_mday  << '/'
		<<  (now->tm_year - 100);

	return os.str();
}