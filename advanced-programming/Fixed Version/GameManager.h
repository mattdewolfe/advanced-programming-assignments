#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <time.h>
#include "HUD.h"
#include "VisualText.h"
#include "Tower.h"
#include "Disc.h"

class GameManager
{
public:
	GameManager(void);
	virtual ~GameManager(void);
	// total number of moves so far
	int moves;
	// the current level
	int currentLevel;
	// Number of towers in this game
	int towerCount;
	// Number of discs in this game
	int discCount;

	// Current disc the player is moving
	int selectedDisc;
	// The tower that the selected disc belongs to
	int selectedDiscOwningTower;
	// Current tower selected to move a disc off of
	int selectedTower;

	// game state enum
	enum GAMESTATE 
	{ 
		MAIN_MENU = 0,	// main menu
		SELECT_DISC,	// selecing a disc to move 
		SELECT_TOWER,	// selecting a tower to move to
		MOVE_DISC,		// animating the disc movement
		LEVEL_CLEAR,	// current level has been cleared
		GAME_OVER, 
		STATE_SIZE
	};
		
	// enum used by button array
	enum BUTTONS
	{
		START_BUTTON,	// start game
		QUIT_BUTTON,	// quit game
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
	// Draw visual objects as decided by gamestate
	void DrawVisuals();
	// Increment game state
	void Update();
	// Takes mouse value and performs action as dependent on game state
	void MousePress(float _x, float _y);
	// Handle key presses, passed in from opengl input functions
	void HandleKeyPress(unsigned char _key);

private:
	// Class arrays for our game objects
	Tower* towers[4];
	Disc* discs[6];
	// Buttons for main menu, setup in constructor
	Button buttons[BUTTONS_SIZE];
	// Draw text to screen
	VisualText visText;
	// Current state of game logic
	GAMESTATE gameState;
	// Heads up display
	HUD hud;
	void DrawObjects();
	// Setup button values
	void SetupButtons();
	// Setup the grid with random resource locations and set value for rich mines
	void SetupGame();
	// Draw a button, as supplied with enum value
	void DrawButton(BUTTONS _button);
	// Check scans remaining and change state when needed
	void CheckVictory();
	// Functions for moving between levels/games
	void ResetGame();
	void StartGame();
	void NextLevel();
};

#endif