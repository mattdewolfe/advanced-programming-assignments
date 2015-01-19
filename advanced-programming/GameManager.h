#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H
#define GRID_SIZE 16
#define TILE_SIZE 10
#define RICH_MINE_VALUE 100

#include <time.h>
#include "HUD.h"
#include "VisualText.h"

class GameManager
{
public:
	GameManager(void);
	virtual ~GameManager(void);
	// Tracks player score
	int score;
	// Tracks number of scans remaining
	int scansRemaining;
	// Tracks number of extractions remaining
	int extractionsRemaining;
	// Track number of frames we have shown scan results for
	int scanResultTimeOut;
	int scanResultTimer;

	// game state enum
	enum GAMESTATE 
	{ 
		MAIN_MENU,		// main menu
		SCAN_MODE,		// scan the grid 
		EXTRACT_MODE,	// extract resources from grid
		START_GAME,		// load level and transition into game
		SHOW_SCAN,		// show the results of a scan
		GAME_OVER, 
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

	// Information for each tile
	struct TileInfo
	{
		TileInfo() : value(0), isVisible(false), wasMined(false) {}
		int value;
		bool isVisible;
		bool wasMined;
	};

	// X and Y values to offset grid starting position in order to keep it centered
	// on the screen
	int gridOffSetX, gridOffSetY;
	// Store map information
	TileInfo mapArray[GRID_SIZE][GRID_SIZE];
	// set state of state machine
	void SetState(GAMESTATE _newState) { gameState = _newState; }
	GAMESTATE GetState() { return gameState; }	// get current state of state machine
	// Toggle between scan mode and extract mode
	void ToggleMode();
	// Draw visual objects as decided by gamestate
	void DrawVisuals();
	// Increment game state
	void Update();
	// Takes mouse value and performs action as dependent on game state
	void MousePress(float _x, float _y);
	// Display mines in and around target node
	void DisplayData(int _x, int _y);
	
private:
	// Buttons for main menu, setup in constructor
	Button buttons[BUTTONS_SIZE];
	// Draw text to screen
	VisualText visText;
	// Current state of game logic
	GAMESTATE gameState;
	// Heads up display
	HUD hud;
	// Insert a rich mine into the map array at the specified location
	// This will also populate adjacent nodes with lower tiers
	void CreateMine(int _x, int _y);
	// Set information in a given node. Will set value if bool passed in is false
	void CreateNode(int _x, int _y, int _value);
	// Alter node visibilities (after a scan)
	void SetNodeVisibility(int _x, int _y, bool _val);
	// Setup button values
	void SetupButtons();
	// Setup the grid with random resource locations and set value for rich mines
	void SetupGame(int _numberOfRichMines);
	// Draw the grid
	void DrawGrid();
	// Draw a button, as supplied with enum value
	void DrawButton(BUTTONS _button);
	// Check scans remaining and change state when needed
	void CheckGameOver();
	// Switch map visuals back to extract mode
	void SwitchToExtract();
};

#endif