#include "GameManager.h"

GameManager::GameManager() :
currentLevel(0), 
moves(0),
discCount(4),
towerCount(3)
{
	SetupGame();
	ResetGame();
	SetupButtons();
	// Seed random number pool with time
	srand (time(NULL));
}

// Initialize the grid, randomize resource spawns
void GameManager::SetupGame()
{
	moves = 0;
	for (int i = 0; i < 4; i++)
	{
		towers[i] = new Tower(30 + i * 60, 50);
	}
	for (int i = 0; i < 6; i++)
	{
		discs[i] = new Disc(30, 50 + (i*13));
		discs[i]->SetSize(i);
	}
}

// Initial setup of buttons. should only be called once
void GameManager::SetupButtons()
{
	buttons[START_BUTTON].height = 15;
	buttons[START_BUTTON].width = 72;
	buttons[START_BUTTON].x = 97;
	buttons[START_BUTTON].y = 90;
	buttons[START_BUTTON].text = "Begin Game";
	
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
		glPushMatrix();
		visText.SetColorFloatRGB(0.8, 0.4, 0.2);
		visText.ReSizeFont(19);
		visText.WriteBitmapString(15, 150, "Towers of Hanoi");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(15, 148, "Towers of Hanoi");
		visText.ReSizeFont(7);
		// iterate through and draw all buttons
		for (int i = 0; i < BUTTONS_SIZE - 1; i++)
		{
			DrawButton(START_BUTTON);
			DrawButton(QUIT_BUTTON);
		}
		glPopMatrix();
		break;
	case SELECT_DISC:
		visText.SetColorFloatRGB(0.8, 0.4, 0.2);
		visText.ReSizeFont(5);
		visText.WriteBitmapString(35, 10, "Press the matching number to select the top disc");
		DrawObjects();
		hud.Draw(moves, towerCount, -1);
		break;
	case SELECT_TOWER:
		visText.SetColorFloatRGB(0.8, 0.4, 0.2);
		visText.ReSizeFont(5);
		visText.WriteBitmapString(15, 10, "Press the matching number to place the disc on that tower");
		DrawObjects();
		hud.Draw(moves, towerCount, selectedDisc);
		break;
	case MOVE_DISC:
		DrawObjects();
		hud.Draw(moves, towerCount, -1);
		break;
	case LEVEL_CLEAR:
		visText.SetColorFloatRGB(0.8, 0.4, 0.2);
		visText.ReSizeFont(5);
		visText.WriteBitmapString(50, 10, "Press any key to start the next level");
		DrawObjects();
		hud.Draw(moves, towerCount);
		break;
	case GAME_OVER:
		visText.SetColorFloatRGB(0.8, 0.8, 0.8);
		visText.WriteBitmapString(100, 100, "GAME OVER");
		hud.Draw(moves, towerCount);
		break;
	}
}

// Draw call for actors
void GameManager::DrawObjects()
{
	for (int i = 0; i < towerCount; i++)
	{
		towers[i]->Draw();
	}
	for (int j = 0; j < discCount; j++)
	{
		discs[j]->Draw();
	}
}

// Increments game logic/state
void GameManager::Update()
{
	switch (gameState)
	{
	case MAIN_MENU:

		break;

	case SELECT_DISC:
		CheckVictory();
		break;

	case SELECT_TOWER:
		break;

	case MOVE_DISC:
		discs[selectedDisc]->Update();
		if (discs[selectedDisc]->CheckIsMoving() == false)
		{
			gameState = SELECT_DISC;
		}
		break;

	case GAME_OVER:
		break;
	}
}

void GameManager::HandleKeyPress(unsigned char _key)
{
	int keyVal = -1;
	// Case for selecing a disc to move
	if (gameState == SELECT_DISC)
	{
		switch (_key)
		{
		case '1':
			keyVal = 0;
			break;

		case '2':
			keyVal = 1;
			break;

		case '3':
			keyVal = 2;
			break;

		case '4':
			keyVal = 3;
		}
		if (keyVal != -1 && keyVal < towerCount)
		{
			if (towers[keyVal]->GetTopDisc() > -1)
			{
				selectedDisc = towers[keyVal]->GetTopDisc();
				selectedDiscOwningTower = keyVal;
				gameState = SELECT_TOWER;
			}
		}
	}

	// Case for selecing a tower to place the disc on
	else if (gameState == SELECT_TOWER)
	{
		switch (_key)
		{
		case '1':
			keyVal = 0;
			break;

		case '2':
			keyVal = 1;
			break;

		case '3':
			keyVal = 2;
			break;

		case '4':
			keyVal = 3;
			break;
		}
		// If the currently selected disc can go on this tower
		if (keyVal != -1 
			&& keyVal < towerCount 
			&& towers[keyVal]->PlaceDiscOnTower(selectedDisc) == true)
		{
			// Calc target X position
			float targetX = 30 + keyVal * 60;
		//	// Calc target Y position
			float targetY = 50;
			if (towers[keyVal]->GetDiscCount() > 1)
			{
				targetY += (towers[keyVal]->GetDiscCount()-1) * 12;
			}
			// Pass new target positions to target disc
			discs[selectedDisc]->SetTargetLocation(targetX, targetY);
			// Remove this disc from its old tower
			towers[selectedDiscOwningTower]->RemoveTopDisc();
			// Increment move count for tracking
			moves++;
			// Switch to disc movement state
			gameState = MOVE_DISC;
		}
	}
	else if (gameState == MOVE_DISC)
	{}
	else if (gameState = LEVEL_CLEAR)
	{
		NextLevel();
	}
	else if (gameState == GAME_OVER)
	{
		ResetGame();
	}
}

// Check disc locations to see if player has won
void GameManager::CheckVictory()
{
	// Check if a given tower has every disc on it
	for (int i = 1; i < towerCount; i++)
	{
		if (towers[i]->GetDiscCount() >= discCount)
		{
			gameState = LEVEL_CLEAR;
		}
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
							StartGame();
						}
						else if ((BUTTONS)i == QUIT_BUTTON)
						{
							wasButton = true;
							exit(0);
						}
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

// Resets all key game values for a new game
void GameManager::ResetGame()
{
	gameState = MAIN_MENU;
	moves = 0;
}

// Resets all key game values for a new game
void GameManager::StartGame()
{
	currentLevel = 0;
	NextLevel();
	gameState = SELECT_DISC;
}

void GameManager::NextLevel()
{
	selectedDisc = 0;
	selectedDiscOwningTower = 0;
	selectedTower = 0;
	moves = 0;

	currentLevel += 1;
	switch (currentLevel)
	{
	case 1:
		discCount = 4;
		towerCount = 3;
		break;
	case 2:
		discCount = 5;
		towerCount = 4;
		break;
	case 3:
		discCount = 6;
		towerCount = 3;
		break;
	default:
		discCount = 4;
		towerCount = 3;
		break;
	}
	// Push discs onto leftmost tower
	towers[0]->FillDiscArrayToIndex(discCount);
	for (int i = 1; i < towerCount; i++)
	{
		towers[i]->ClearDiscs();
		towers[i]->SetIsVisible(true);
	}
	for (int i = 0; i < discCount; i++)
	{
		discs[i]->SetIsVisible(true);
	}
	gameState = SELECT_DISC;
}

GameManager::~GameManager()
{}