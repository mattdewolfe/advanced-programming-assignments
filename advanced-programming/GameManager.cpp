#include "GameManager.h"

GameManager::GameManager() : gameState(MAIN_MENU), 
	score(0), 
	scansRemaining(9), 
	extractionsRemaining(6), 
	scanResultTimeOut(28), 
	scanResultTimer(0)
{
	SetupButtons();
	// Seed random number pool with time
	srand (time(NULL));
}

// Check scans remaining and change state when needed
void GameManager::CheckGameOver()
{
	if (extractionsRemaining <= 0)
		gameState = GAME_OVER;
}

// Toggle between extract and scan mode, also calls toggle in HUD class
void GameManager::ToggleMode()
{
	if (gameState == SCAN_MODE)
	{
		gameState = EXTRACT_MODE;
		hud.ToggleMode();
	}
	else if (gameState == EXTRACT_MODE)
	{
		gameState = SCAN_MODE;
		hud.ToggleMode();
	}

}

// Draw the grid to the screen
void GameManager::DrawGrid()
{
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			glPushMatrix();
			glTranslatef(gridOffSetX + (i * TILE_SIZE), gridOffSetY + (j * TILE_SIZE), 0);
			if (mapArray[i][j].isVisible == true || mapArray[i][j].wasMined == true)
			{
				switch (mapArray[i][j].value) 
				{
					case 0:
						if (mapArray[i][j].wasMined == true)
							glColor3f(0.1, 0.1, 0.1);
						else
							glColor3f(0.3, 0.3, 0.3);
						break;
					case 25:
						glColor3f(1.0, 0.9, 0.0);
						break;
					case 50:
						glColor3f(1.0, 0.5, 0.0);
						break;
					case 100:
						glColor3f(1.0, 0.0, 0.0);
						break;
				}
				glBegin(GL_TRIANGLE_STRIP);
					glVertex3f(0, 0, -4);
					glVertex3f(TILE_SIZE, 0, -4);
					glVertex3f(TILE_SIZE, TILE_SIZE, -4);
					glVertex3f(0, TILE_SIZE, -4);
					glVertex3f(0, 0, -4);
				glEnd();	
			}
			glColor3f(1.0, 1.0, 1.0);
			glLineWidth(2.0);
			glBegin(GL_LINE_STRIP);
				glVertex3f(0, 0, -4);
				glVertex3f(TILE_SIZE, 0, -4);
				glVertex3f(TILE_SIZE, TILE_SIZE, -4);
				glVertex3f(0, TILE_SIZE, -4);
				glVertex3f(0, 0, -4);
			glEnd();
			

			glPopMatrix();
		}
	}
	glLineWidth(1.0);
}

// Initialize the grid, randomize resource spawns
void GameManager::SetupGame(int _numberOfRichMines)
{
	scansRemaining = 9;
	extractionsRemaining = 6;
	
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			mapArray[i][j] = TileInfo();
		}
	}

	for (int i = 0; i < _numberOfRichMines; i++)
	{
		CreateMine(rand()%16, rand()%16);
	}
	gridOffSetX = (GLUT_SCREEN_WIDTH - (GRID_SIZE*TILE_SIZE))/0.8;
	gridOffSetY = (GLUT_SCREEN_HEIGHT - (GRID_SIZE*TILE_SIZE))/2;
}

// Insert a rich mine and populate adjacent areas
void GameManager::CreateMine(int _x, int _y)
{
	mapArray[_x][_y].value = RICH_MINE_VALUE;
	// Insert inner ring
	int temp = RICH_MINE_VALUE/2;
	CreateNode(_x - 1, _y - 1, temp);
	CreateNode(_x, _y - 1, temp);
	CreateNode(_x + 1, _y - 1, temp);
	CreateNode(_x - 1, _y, temp);
	CreateNode(_x + 1, _y, temp);
	CreateNode(_x - 1, _y + 1, temp);
	CreateNode(_x, _y + 1, temp);
	CreateNode(_x + 1, _y + 1, temp);

	// Create outter ring
	temp /= 2;
	// Top line
	CreateNode(_x - 2, _y - 2, temp);
	CreateNode(_x - 1, _y - 2, temp);
	CreateNode(_x, _y - 2, temp);
	CreateNode(_x + 1, _y - 2, temp);
	CreateNode(_x + 2, _y - 1, temp);
	// Second line down
	CreateNode(_x - 2, _y - 1, temp);
	CreateNode(_x + 2, _y - 1, temp);
	// Third line down
	CreateNode(_x - 2, _y, temp);
	CreateNode(_x + 2, _y, temp);
	// Fourth line down
	CreateNode(_x - 2, _y + 1, temp);
	CreateNode(_x + 2, _y + 1, temp);
	// Bottom line
	CreateNode(_x - 2, _y + 2, temp);
	CreateNode(_x - 1, _y + 2, temp);
	CreateNode(_x, _y + 2, temp);
	CreateNode(_x + 1, _y + 2, temp);
	CreateNode(_x + 2, _y + 1, temp);
}

void GameManager::DisplayData(int _x, int _y)
{
	scansRemaining -= 1;
	gameState = SHOW_SCAN;
	// Middle row
	SetNodeVisibility(_x, _y, true);
	SetNodeVisibility(_x - 1, _y, true);
	SetNodeVisibility(_x + 1, _y, true);
	// Top Row
	SetNodeVisibility(_x - 1, _y - 1, true);
	SetNodeVisibility(_x, _y - 1, true);
	SetNodeVisibility(_x + 1, _y - 1, true);
	// Bottom Row
	SetNodeVisibility(_x - 1, _y + 1, true);
	SetNodeVisibility(_x, _y + 1, true);
	SetNodeVisibility(_x + 1, _y + 1, true);
}

// Set the visibility flag of a specified node
void GameManager::SetNodeVisibility(int _x, int _y, bool _val)
{
	if (_x > -1 && _x < GRID_SIZE)
		if (_y > -1 && _y < GRID_SIZE)
			mapArray[_x][_y].isVisible = _val;
}

// Called by create mine to populate the lower tiers of resource
void GameManager::CreateNode(int _x, int _y, int _value)
{
	// If the target position is legal, and the target value is lower
	// than the current value in that position - set the new value
	if (_x > -1 && _x < GRID_SIZE)
		if (_y > -1 && _y < GRID_SIZE)
			if (mapArray[_x][_y].value < _value)
				mapArray[_x][_y].value = _value;
}

// Initial setup of buttons. should only be called once
void GameManager::SetupButtons()
{
	buttons[START_BUTTON].height = 15;
	buttons[START_BUTTON].width = 105;
	buttons[START_BUTTON].x = 78;
	buttons[START_BUTTON].y = 90;
	buttons[START_BUTTON].text = "Begin Harvesting";

	buttons[TOGGLE_BUTTON].height = 15;
	buttons[TOGGLE_BUTTON].width = 75;
	buttons[TOGGLE_BUTTON].x = 93;
	buttons[TOGGLE_BUTTON].y = 3;
	buttons[TOGGLE_BUTTON].text = "switch mode";
	
	buttons[QUIT_BUTTON].height = 15;
	buttons[QUIT_BUTTON].width = 33;
	buttons[QUIT_BUTTON].x = 112;
	buttons[QUIT_BUTTON].y = 65;
	buttons[QUIT_BUTTON].text = "Exit";
}

// Main draw statement here - switch based on gamestate and draw elements as needed
void GameManager::DrawVisuals()
{
	switch(gameState)
	{
	case MAIN_MENU:
		#pragma region MainMenuVisuals
		glPushMatrix();
		visText.SetColorFloatRGB(0.8, 0.4, 0.2);
		visText.ReSizeFont(19);
		visText.WriteBitmapString(60, 150, "Harvester");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(60, 148, "Harvester");
		visText.ReSizeFont(7);
		// iterate through and draw all buttons
		for (int i = 0; i < BUTTONS_SIZE - 1; i++)
		{
			DrawButton(START_BUTTON);
			DrawButton(QUIT_BUTTON);
		}
		glPopMatrix();
		#pragma endregion MainMenuVisuals
		break;
	case EXTRACT_MODE:
	case SCAN_MODE:
		DrawGrid();
		DrawButton(TOGGLE_BUTTON);
		hud.Draw(score, scansRemaining, extractionsRemaining);
		break;
	case SHOW_SCAN:
		DrawGrid();
		DrawButton(TOGGLE_BUTTON);
		hud.Draw(score, scansRemaining, extractionsRemaining);
		break;
	case GAME_OVER:
		visText.SetColorFloatRGB(0.8, 0.8, 0.8);
		visText.WriteBitmapString(100, 100, "GAME OVER");
		break;
	}
}

// Switches map visuals back to standard mode after a scan
void GameManager::SwitchToExtract()
{
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{
			mapArray[i][j].isVisible = false;
		}
	}
	gameState = SCAN_MODE;
	scanResultTimer = 0;
}

// Increments game logic/state
void GameManager::Update()
{
	switch (gameState)
	{
	case MAIN_MENU:

		break;

	case START_GAME:
		SetupGame(4);
		gameState = SCAN_MODE;
		break;

	case EXTRACT_MODE:
		CheckGameOver();
		break;

	case SCAN_MODE:
		CheckGameOver();
		break;

	case SHOW_SCAN:
		scanResultTimer += 1;
		if (scanResultTimer > scanResultTimeOut)
		{
			SwitchToExtract();
		}
		break;
	case GAME_OVER:
		break;
	}
}

// Handle mouse input, based on gamestate enum
void GameManager::MousePress(float _inX, float _inY)
{
	// We use this bool to avoid doing position checks after iterating
	// through all the buttons. It will only be true if we hit a button
	bool wasButton = false;
	// Iterate through each button
	for (int i = 0; i < BUTTONS_SIZE; i++)
	{
		// Check if the mouse click was within a specific buttons boundary
		if (_inX > buttons[(BUTTONS)i].x && _inX < buttons[(BUTTONS)i].x + buttons[(BUTTONS)i].width)
		{
			if (_inY > buttons[(BUTTONS)i].y && _inY < buttons[(BUTTONS)i].y + buttons[(BUTTONS)i].height)
			{
				// If so, switch through gamestate for the action to take upon pressing that button
				switch (gameState)
				{	
					// Main menu possibilities are to start the game, or quit the program
					case MAIN_MENU:
						if ((BUTTONS)i == START_BUTTON)
						{
							wasButton = true;
							gameState = START_GAME;
						}
						else if ((BUTTONS)i == QUIT_BUTTON)
						{
							wasButton = true;
							exit(0);
						}
						break;
					// Scan mode and extract mode button possibitlites are limited to toggling between modes
					case SCAN_MODE:
					case EXTRACT_MODE:
						if ((BUTTONS)i == TOGGLE_BUTTON)
						{
							wasButton = true;
							ToggleMode();
						}
						break;
				}
			}
		}
	}
	// Check X and Y for click, move from there
	if (wasButton == false)
	{
		_inX -= gridOffSetX;
		_inY -= gridOffSetY;
		// Ensure the value, after subtracting offset, is still on the grid somewhere
		if (_inX >= 0 && _inY >= 0)
		{
			// Divide X by tile size and see if it falls on grid
			int xPos = _inX / TILE_SIZE;
			if (xPos < GRID_SIZE)
			{
				// Divide Y by tile size, and see if it falls on grid
				int yPos = _inY / TILE_SIZE;
				if (yPos < GRID_SIZE)
				{
					// Depending on game state, take input position and
					// act on the tile that was clicked. Displaying it or mining it
					switch (gameState)
					{
					case SCAN_MODE:
						if (scansRemaining > 0)
						{
							if (mapArray[xPos][yPos].wasMined == false)
							{
								DisplayData(xPos, yPos);
							}
						}
						break;
					case EXTRACT_MODE:
						if (extractionsRemaining > 0)
						{
							if (mapArray[xPos][yPos].value > 0 && mapArray[xPos][yPos].wasMined == false)
							{
								score += mapArray[xPos][yPos].value;
								mapArray[xPos][yPos].wasMined = true;
								extractionsRemaining -= 1;
							}
						}
						break;
					}
				}
			}
		}
	}
}

// Draws a button to the screen - expects enum value to access button array
void GameManager::DrawButton(BUTTONS _bval)
{
	// ensure value is within bounds of button array
	if (_bval > BUTTONS_SIZE || _bval < 0)
		return;

	glPushMatrix();
		visText.SetColorFloatRGB(0.8, 0.8, 0.8);
		visText.WriteBitmapString(buttons[_bval].x + 5, buttons[_bval].y + 3, buttons[_bval].text);
		visText.SetColorFloatRGB(1.0, 0.0, 0.0);
		visText.WriteBitmapString(buttons[_bval].x + 5, buttons[_bval].y + 3, buttons[_bval].text);
		glColor3f(0.0, 0.5, 0.5);
		glLineWidth(2.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(buttons[_bval].x, buttons[_bval].y, -4);
			glVertex3f(buttons[_bval].x + buttons[_bval].width, buttons[_bval].y, -4);
			glVertex3f(buttons[_bval].x + buttons[_bval].width, buttons[_bval].y + buttons[_bval].height, -4);
			glVertex3f(buttons[_bval].x, buttons[_bval].y + buttons[_bval].height, -4);
			glVertex3f(buttons[_bval].x, buttons[_bval].y, -4);
		glEnd();
		glLineWidth(1.0);
	glPopMatrix();
}

GameManager::~GameManager()
{}


