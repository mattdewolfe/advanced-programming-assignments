#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#define GRID_SIZE 16
#define TILE_SIZE 10
#include "HUD.h"
#include "Score.h"
#include "VisualText.h"

class GameManager
{
public:
	GameManager(void);
	virtual ~GameManager(void);
	// tracks player score
	Score score;
	// game state enum
	enum GAMESTATE 
	{ 
		MAIN_MENU,		// main menu
		SCAN_MODE,		// scan the grid 
		EXTRACT_MODE,	// extract resources from grid
		START_GAME,		// load level and transition into game
		STATE_SIZE
	};
		
	// enum used by button array
	enum BUTTONS
	{
		START_BUTTON,	// start game
		TOGGLE_BUTTON,	// toggle between extract and scan
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

	// X and Y values to offset grid starting position in order to keep it centered
	// on the screen
	int gridOffSetX, gridOffSetY;
	
	// Store map information
	int mapArray[GRID_SIZE][GRID_SIZE];

	// set state of state machine
	void SetState(GAMESTATE _newState) { gameState = _newState; }
	GAMESTATE GetState() { return gameState; }	// get current state of state machine
	// Toggle between scan mode and extract mode
	void ToggleMode();
	// draw visual objects as decided by gamestate
	void DrawVisuals();
	// increment game state
	void Update();
	// takes mouse value and performs action as dependent on game state
	void MousePress(float _x, float _y);
	
private:
	// buttons for main menu, setup in constructor
	Button buttons[BUTTONS_SIZE];
	// draw text to screen
	VisualText visText;
	// current state of game logic
	GAMESTATE gameState;
	// heads up display
	HUD hud;
	// setup button values
	void SetupButtons();
	// setup the grid with random resource locations
	void SetupGame();
	// draw the grid
	void DrawGrid();
	// draw a button, as supplied with enum value
	void DrawButton(BUTTONS _button);
};

#endif