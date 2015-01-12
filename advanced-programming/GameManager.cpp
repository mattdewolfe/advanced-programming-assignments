#include "GameManager.h"

GameManager::GameManager()
{
	gameState = MAIN_MENU;
	SetupButtons();
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
			glColor3f(1.0, 1.0, 1.0);
			glLineWidth(2.0);
		
			glBegin(GL_LINE_STRIP);
				glVertex3f(0, 0, -4);
				glVertex3f(TILE_SIZE, 0, -4);
				glVertex3f(TILE_SIZE, TILE_SIZE, -4);
				glVertex3f(0, TILE_SIZE, -4);
				glVertex3f(0, 0, -4);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glRectf(0, 0, 5, 5);
			
			glPopMatrix();
		}
	}
	glLineWidth(1.0);
}

// Initialize the grid, randomize resource spawns
void GameManager::SetupGame()
{
	for (int i = 0; i < GRID_SIZE; i++)
	{
		for (int j = 0; j < GRID_SIZE; j++)
		{

		}
	}
	gridOffSetX = (GLUT_SCREEN_WIDTH - (GRID_SIZE*TILE_SIZE))/0.85;
	gridOffSetY = (GLUT_SCREEN_HEIGHT - (GRID_SIZE*TILE_SIZE))/2;
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
		hud.Draw();
		break;
	}
}

// Increments game logic/state
void GameManager::Update()
{
	switch (gameState)
	{
	case MAIN_MENU:

		break;

	case START_GAME:
		SetupGame();
		gameState = SCAN_MODE;
		break;

	case EXTRACT_MODE:
		
		break;

	case SCAN_MODE:

		break;
	}
}

// Handle mouse input, based on gamestate enum
void GameManager::MousePress(float _inX, float _inY)
{
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
							gameState = START_GAME;
						else if ((BUTTONS)i == QUIT_BUTTON)
							exit(0);
						break;
					// Scan mode and extract mode button possibitlites are limited to toggling between modes
					case SCAN_MODE:
					case EXTRACT_MODE:
						if ((BUTTONS)i == TOGGLE_BUTTON)
							ToggleMode();
						break;
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


