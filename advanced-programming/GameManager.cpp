#include "GameManager.h"

GameManager::GameManager()
{
	gameState = MAIN_MENU;
	stage = SURFACE_STAGE;
	playerX = 0;
	playerY = 0;
	zTime = -1.0f;
	moveUp = false;
	moveDown = false;
	moveLeft = false;
	moveRight = false;
	fireWeapon = false;
	stageBoundaries[FIGHTERS_STAGE][0] = 300;
	stageBoundaries[FIGHTERS_STAGE][1] = 150;
	stageBoundaries[SURFACE_STAGE][0] = 400;
	stageBoundaries[SURFACE_STAGE][1] = 150;
	stageBoundaries[TRENCH_STAGE][0] = 200;
	stageBoundaries[TRENCH_STAGE][1] = 50;
	fireDelay = 10;
	lastShot = 0;
	counter = 0;
	SetupButtons();
}
// initial setup of buttons. should only be called once
void GameManager::SetupButtons()
{
	buttons[START_BUTTON].height = 15;
	buttons[START_BUTTON].width = 145;
	buttons[START_BUTTON].x = 55;
	buttons[START_BUTTON].y = 71;
	buttons[START_BUTTON].text = "Assault the death star!";

	buttons[HELP_BUTTON].height = 15;
	buttons[HELP_BUTTON].width = 135;
	buttons[HELP_BUTTON].x = 60;
	buttons[HELP_BUTTON].y = 46;
	buttons[HELP_BUTTON].text = "I need information...";

	buttons[QUIT_BUTTON].height = 15;
	buttons[QUIT_BUTTON].width = 45;
	buttons[QUIT_BUTTON].x = 105;
	buttons[QUIT_BUTTON].y = 22;
	buttons[QUIT_BUTTON].text = "I quit";

	buttons[BACK_BUTTON].height = 15;
	buttons[BACK_BUTTON].width = 35;
	buttons[BACK_BUTTON].x = 110;
	buttons[BACK_BUTTON].y = 10;
	buttons[BACK_BUTTON].text = "back";
}
// main draw statement here - switch based on gamestate and draw elements as needed
void GameManager::DrawVisuals()
{
	CheckFlags();
	switch(gameState)
	{
	case MAIN_MENU:
		#pragma region MainMenuVisuals
		glPushMatrix();
		visText.SetColorFloatRGB(1.0, 0.0, 0.0);
		visText.ReSizeFont(19);
		visText.WriteBitmapString(36, 150, "War of Stars");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(36, 148, "War of Stars");
		visText.ReSizeFont(7);
		// iterate through and draw all buttons
		for (int i = 0; i < BUTTONS_SIZE - 1; i++)
		{
			DrawButton((BUTTONS)i);
		}
		glPopMatrix();
		#pragma endregion MainMenuVisuals
		break;
	case START_GAME:
		break;
	case HELP_SCREEN:
		#pragma region HelpScreenVisuals
		glPushMatrix();
		visText.ReSizeFont(19);
		visText.SetColorFloatRGB(1.0, 0.0, 0.0);
		visText.WriteBitmapString(36, 150, "War of Stars");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(36, 148, "War of Stars");
		visText.ReSizeFont(8);
		visText.SetColorFloatRGB(0.0, 1.0, 0.0);
		visText.WriteBitmapString(63, 110, "Use the mouse to aim");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(63, 109, "Use the mouse to aim");
		visText.SetColorFloatRGB(0.0, 1.0, 0.0);
		visText.WriteBitmapString(54, 97, "and left click to fire");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(54, 96, "and left click to fire");

		visText.SetColorFloatRGB(0.0, 1.0, 0.0);
		visText.WriteBitmapString(40, 84, "Use the arrows to manuever!");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(40, 83, "Use the arrows to manuever!");
		visText.SetColorFloatRGB(0.0, 1.0, 0.0);
		visText.WriteBitmapString(55, 65, "Fly across the surface");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(55, 64, "Fly across the surface");

		visText.SetColorFloatRGB(0.0, 1.0, 0.0);
		visText.WriteBitmapString(55, 53, "and through the trench");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(55, 52, "and through the trench");
		visText.SetColorFloatRGB(0.0, 1.0, 0.0);
		visText.WriteBitmapString(40, 41, "to destroy the exhaust port");
		visText.SetColorFloatRGB(1.0, 1.0, 1.0);
		visText.WriteBitmapString(40, 40, "to destroy the exhaust port");

		DrawButton(BACK_BUTTON);
		glPopMatrix();
		#pragma endregion HelpScreenVisuals
		break;
	case START_WAVE:
		hud.Draw();
		break;
	case PLAY_GAME:
		hud.Draw();
		glPushMatrix();
			objectFactory.DrawTerrain((int)stage);
			glTranslatef(playerX, playerY, -1);
		if (stage != TRENCH_STAGE)
			objectFactory.DrawTerrain((int)stage);
			objectFactory.DrawAll();
		glPopMatrix();
		break;
	case PLAYER_KILLED:
		hud.Draw();
		break;
	case CHANGE_STAGE:
		hud.Draw();
		break;
	case WAVE_OVER:
		break;
	case GAME_OVER:
		break;
	}
}
// check if the wave is over, will move to next stage and return false if it is not
bool GameManager::IsWaveOver()
{
	// if player just completed trench stage, reset to fighters stage and return true as the wave is over
	if (stage == TRENCH_STAGE)
	{
	//	gameState = WAVE_OVER;
		stage = SURFACE_STAGE;
		return true;
	}
	// if the player just beat the fighters, move to surface stage and return false
	else if (stage == FIGHTERS_STAGE)
	{
		stage = SURFACE_STAGE;
		return false;
	}
	// if the player just beat the surface, move to trench and return false
	else if (stage == SURFACE_STAGE)
	{
		stage = TRENCH_STAGE;
		return false;
	}
	return false;
}
// check movement flags and change values as needed
void GameManager::CheckFlags()
{
	if (moveLeft == true && playerX < stageBoundaries[stage][0])
	{
		playerX += 1.0f;
	}
	if (moveRight == true && playerX > 1)
	{
		playerX -= 1.0f;
	}
	if (moveDown == true && playerY < stageBoundaries[stage][1])
	{
		playerY += 1.0f;
	}
	if (moveUp == true && playerY > 1)
	{
		playerY -= 1.0f;
	}
}
// tell object factory to setup next stage
void GameManager::SetupStage(STAGE _nextStage)
{
	switch (_nextStage)
	{
	case FIGHTERS_STAGE:
		objectFactory.SetupFighterStage();
		break;
	case SURFACE_STAGE:
		objectFactory.SetupSurfaceStage();
		break;
	case TRENCH_STAGE:
		objectFactory.SetupTrenchStage();
		break;
	}
	playerX = 100;
	playerY = 50;
}
// increments game logic/state
void GameManager::Update()
{
	switch (gameState)
	{
	case START_GAME:
		SetupStage(stage);
		gameState = PLAY_GAME;
		break;
	case PLAY_GAME:
		counter++;
		objectFactory.Update();
		CheckFlags();
		break;
	}
	zTime++;
	if (zTime > 450)
	{
		IsWaveOver();
		gameState = START_GAME;
		zTime = 0;
	}
}
// handle mouse input, based on gamestate enum
void GameManager::MousePress(float _inX, float _inY)
{
	switch (gameState)
	{
	case MAIN_MENU:
		// menu movement
		for (int i = 0; i < BUTTONS_SIZE - 1; i++)
		{
			if (_inX > buttons[(BUTTONS)i].x && _inX < buttons[(BUTTONS)i].x + buttons[(BUTTONS)i].width)
				if (_inY > buttons[(BUTTONS)i].y && _inY < buttons[(BUTTONS)i].y + buttons[(BUTTONS)i].height)
					switch(BUTTONS(i))
					{
					case START_BUTTON:
						gameState = START_GAME;
						break;
					case HELP_BUTTON:
						gameState = HELP_SCREEN;
						break;
					case QUIT_BUTTON:
						exit(0);
						break;
					}
		}
		break;
	case HELP_SCREEN:
		if (_inX > buttons[BACK_BUTTON].x && _inX < buttons[BACK_BUTTON].x + buttons[BACK_BUTTON].width)
				if (_inY > buttons[BACK_BUTTON].y && _inY < buttons[BACK_BUTTON].y + buttons[BACK_BUTTON].height)
					gameState = MAIN_MENU;
		break;
	case PLAY_GAME:
		if (lastShot + fireDelay < counter)
		{
			hud.FireLaser(_inX, _inY);
			// check hit detection
			lastShot = counter;
		}
		
		break;
	}
}
// draws a button to the screen - expects enum value to access button array
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
	glColor3f(0.0, 0.8, 0.1);
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
{
}


