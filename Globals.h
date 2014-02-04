#pragma once

const int WIDTH = 640;
const int HEIGHT = 320;

enum ID{PLAYER, ENEMY, PROJECTILE, BORDER, MISC, EXPLOSION};
enum STATE{TITLE, PLAYING, GAMEOVER};
enum CHALLENGE{NONE, COURAGE, WISDOM, HONESTY, CANCER, BATTLE};
enum BLOCK_TYPE{BACKGROUND, PLATFORM, DIAMOND, BRAIN, WALLET, TRIGGER, MONEY};

#define BACKGROUND_TILE		4

#define CORRECT_WISDOM_ANSWER		ALLEGRO_KEY_C
#define CORRECT_HONESTY_ANSWER		ALLEGRO_KEY_B
#define HIGHSCORE_FILE		"HighScores.txt"