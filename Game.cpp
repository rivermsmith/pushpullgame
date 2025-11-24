#include "pch.h"
#include "Game.h"
#include <iostream>

//Basic game functions
#pragma region gameFunctions											
void Start()
{
	// initialize game resources here

	Init();
}

void Draw()
{
	ClearBackground();

	// Put your own draw statements here
#pragma region Attila
	DrawPlatforms();
	DrawPushPullRange();
#pragma endregion Attila

#pragma region River
	DrawPlayer();

#pragma endregion River

}

void Update(float elapsedSec)
{
	// process input, do physics 

	// e.g. Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
#pragma region River
	UpdatePlayer(elapsedSec);

#pragma endregion River

}

void End()
{
	// free game resources here
}
#pragma endregion gameFunctions

//Keyboard and mouse input handling
#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key)
{
#pragma region River
	switch (key)
	{
	case SDLK_a:
		g_Player.speed.x = -g_MoveSpeed;
		g_Player.left = true;
		break;
	case SDLK_d:
		g_Player.speed.x = g_MoveSpeed;
		g_Player.right = true;
		break;
	case SDLK_SPACE:
		if (!g_Player.falling && !g_HoldJump)
		{
			g_Player.speed = Add(g_Player.speed, Vector2f{ 0.f, -g_JumpSpeed });
			g_Player.falling = true;
			g_HoldJump = true;
		}
		break;
	}

#pragma endregion River
}

void OnKeyUpEvent(SDL_Keycode key)
{
	//switch (key)
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
#pragma region River
	switch (key)
	{
	case SDLK_a:
		g_Player.left = false;
		if (g_Player.speed.x == -g_MoveSpeed)
		{
			if (g_Player.right)
			{
				g_Player.speed.x = g_MoveSpeed;
			}
			else g_Player.speed.x = 0;
		}
		break;
	case SDLK_d:
		g_Player.right = false;
		if (g_Player.speed.x == g_MoveSpeed)
		{
			if (g_Player.left)
			{
				g_Player.speed.x = -g_MoveSpeed;
			}
			else g_Player.speed.x = 0;
		}
		break;
	case SDLK_SPACE:
		if (g_Player.speed.y < 0)
		{
			g_Player.speed.y = 0;
		}
		g_HoldJump = false;
		break;
	}

#pragma endregion River
}

void OnMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	// SAMPLE CODE: print mouse position
	const float mouseX{ float(e.x) };
	const float mouseY{ float(e.y) };
	//std::cout << "  [" << mouseX << ", " << mouseY << "]\n";
	g_MousePosition = Point2f{ mouseX, mouseY };
}

void OnMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	// SAMPLE CODE: print mouse position
	//const float mouseX{ float(e.x) };
	//const float mouseY{ float(e.y) };
	//std::cout << "  [" << mouseX << ", " << mouseY << "]\n";
}

void OnMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	// SAMPLE CODE: print mouse position
	//const float mouseX{ float(e.x) };
	//const float mouseY{ float(e.y) };
	//std::cout << "  [" << mouseX << ", " << mouseY << "]\n";
	
	// SAMPLE CODE: check which mouse button was pressed
	//switch (e.button)
	//{
	//case SDL_BUTTON_LEFT:
	//	//std::cout << "Left mouse button released\n";
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	//std::cout << "Right mouse button released\n";
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	//std::cout << "Middle mouse button released\n";
	//	break;
	//}
}
#pragma endregion inputHandling

#pragma region ownDefinitions
// Define your own functions here
#pragma region River
void DrawPlayer()
{
	SetColor(1.f, 0.6f, 0.6f);
	FillArc(g_Player.pos, g_Player.circ.radius, g_Player.circ.radius, 0.f, 2 * g_Pi);
	if (g_Debug) {
		SetColor(1.f, 0.f, 0.f);
		DrawVector(g_Player.pos, Scale(g_Player.speed, 0.125f));
	}
}

void UpdatePlayer(float elapsedSec)
{
	g_Player.pos = Point2f{ g_Player.pos.x + (g_Player.speed.x * elapsedSec), g_Player.pos.y + (g_Player.speed.y * elapsedSec) };
	if (g_Player.pos.y >= g_WindowHeight - g_Player.circ.radius && g_Player.falling)
	{
		g_Player.pos.y = g_WindowHeight - g_Player.circ.radius;
		g_Player.speed = Vector2f{ g_Player.speed.x, 0.f };
		g_Player.falling = false;
	}
	if (g_Player.falling)
	{
		g_Player.speed = Add(g_Player.speed, Vector2f{ 0.f, -(g_Gravity * elapsedSec) });
	}
}

#pragma endregion River

#pragma region Attila
void Init() {
	g_Debug = true;

	g_GridSize = 80.f;

	g_GridWidth = static_cast<int>(g_WindowWidth / g_GridSize);
	g_GridHeight = static_cast<int>(g_WindowHeight / g_GridSize);
	g_GridArraySize = g_GridHeight * g_GridWidth;


	
	g_pGridArray = new int[g_GridArraySize] 
		{
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		};
	//for (int col{ 0 }; col < g_GridWidth; ++col) {
	//	int row = g_GridHeight - 1;
	//	g_pGridArray[GetIndex(row, col, g_GridWidth)] = 1;
	//}
	//CreatePlatforms(g_pGridArray, g_GridWidth, 0, g_GridWidth, g_GridHeight - 1, g_GridHeight);
}

void DrawPlatforms() {
	const Color4f 
		green{ 0.f, 1.f, 0.f, 1.f },
		white{ 1.f,1.f,1.f,1.f };

	for (int idx{ 0 }; idx < g_GridArraySize; ++idx) {
		const Rectf grid{
			GetCol(idx, static_cast<int>(g_WindowWidth / g_GridSize))* g_GridSize,
			GetRow(idx, static_cast<int>(g_WindowWidth / g_GridSize))* g_GridSize,
			g_GridSize, g_GridSize
		};
		if(g_pGridArray[idx] == 1){
			utils::SetColor(green);
			utils::FillRect(grid);
		}
		if (g_Debug) {
			utils::SetColor(white);
			utils::DrawRect(grid);
		}
	}
}

void DrawPushPullRange() {
	const Color4f cyan{ 0.f, 1.f, 1.f, 1.f };
	utils::SetColor(cyan);
	utils::DrawLine(g_MousePosition, g_Player.pos);
}

//void CreatePlatforms(int array[], int gridWidth, int startRow, int endRow, int startCol, int endCol) {
//	for (int col{ startCol }; col < endCol; ++col) {
//		for (int row{ startRow }; row < endRow; ++row) {
//			*(array + GetIndex(row, col, gridWidth)) = 1;
//			//array[GetIndex(row, col, gridWidth)] = 1;
//		}
//	}
//}
int GetIndex(int rowIdx, int colIdx, int nrCols) {
	return rowIdx * nrCols + colIdx;
}
int GetRow(int index, int numCols) {
	return index / numCols;
}
int GetCol(int index, int numCols) {
	return index % numCols;
}

#pragma endregion Attila


#pragma endregion ownDefinitions