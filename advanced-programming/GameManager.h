#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "ObjectFactory.h"
#include "HUD.h"
#include "Score.h"
#include "VisualText.h"

class GameManager
{
public:
	GameManager(void);
	virtual ~GameManager(void);
	bool moveUp, moveDown, moveLeft, moveRight, fireWeapon;
	float playerX, playerY, zTime; // Co-ordinates of the player and Z position of world
	// tracks player score
	Score score;
	// game state enum
	enum GAMESTATE 
	{ 
		MAIN_MENU,		// main menu
		START_GAME,		// load resources, start a new game
		HELP_SCREEN,	// show how to play and general info
		START_WAVE,		// reset values as needed, start a new wave
		PLAY_GAME,		// majority of gameplay here - input, collision, ai, etc
		PLAYER_KILLED,	// after the player is killed, move here, check lives, respawn if possible
		CHANGE_STAGE,	// after completing the current stage, trigger and move to next stage (if applicable)
		WAVE_OVER,		// show deathstar explosion, inform player next wave is starting
		GAME_OVER,		// display game over screen, return to main on key press
		STATE_SIZE
	};
	// enum used for checking next stage, and loading enemies/visuals
	enum STAGE
	{
		FIGHTERS_STAGE,	// approaching deathstar, dog fight with fighters
		SURFACE_STAGE,	// moving along surface, towers collidable and shoot at player
		TRENCH_STAGE,	// flying through trench towards exhaust port
		STAGE_SIZE
	};
	
	// enum used by button array
	enum BUTTONS
	{
		START_BUTTON,	// start game
		HELP_BUTTON,	// help screen
		QUIT_BUTTON,	// quit game
		BACK_BUTTON,	// exit current screen
		BUTTONS_SIZE
	};
	// used for menu buttons
	struct Button 
	{
		int x;	
		int y;	
		int height;
		int width;
		char* text;
	};
	// set state of state machine
	void SetState(GAMESTATE _newState) { gameState = _newState; }
	GAMESTATE GetState() { return gameState; }	// get current state of state machine
	STAGE GetStage() { return stage; }			// get the current stage player is in

	// draw visual objects as decided by gamestate
	void DrawVisuals();
	// will return true if the wave is over, else will increment to next stage and return false
	bool IsWaveOver();
	// tell object factory to setup the next stage
	void SetupStage(STAGE _nextStage);
	// increment game state
	void Update();
	// takes mouse value and performs action as dependent on game state
	void MousePress(float _x, float _y);
private:
	// variables to track when the player can shoot next
	int lastShot, fireDelay, counter;
	// buttons for main menu, setup in constructor
	Button buttons[BUTTONS_SIZE];
	// draw text to screen
	VisualText visText;
	// integer array storing map boundaries for each stage
	int stageBoundaries[STAGE_SIZE][2];
	// current state of game logic
	GAMESTATE gameState;
	// the current stage the player is on
	STAGE stage;
	// store all objects in the scene
	ObjectFactory objectFactory;
	// heads up display
	HUD hud;
	// check key flags for movement
	void CheckFlags();
	// setup button values
	void SetupButtons();
	// draw a button, as supplied with enum value
	void DrawButton(BUTTONS _button);
};

#endif