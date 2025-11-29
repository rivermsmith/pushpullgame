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
		g_Player.moveState = lastPressed::left;
		break;
	case SDLK_d:
		g_Player.speed.x = g_MoveSpeed;
		g_Player.right = true;
		g_Player.moveState = lastPressed::right;
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
		if (g_Player.moveState == lastPressed::left)
		{
			if (g_Player.right)
			{
				g_Player.moveState = lastPressed::right;
			}
			else
			{
				g_Player.moveState = lastPressed::none;
			}
		}
		break;
	case SDLK_d:
		g_Player.right = false;
		if (g_Player.moveState == lastPressed::right)
		{
			if (g_Player.left)
			{
				g_Player.moveState = lastPressed::left;
			}
			else
			{
				g_Player.moveState = lastPressed::none;
			}
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
	FillRect(g_Player.rect);
	if (g_Debug) {
		SetColor(1.f, 0.f, 0.f);
		DrawVector(g_Player.pos, Scale(g_Player.speed, 0.125f));
	}
	SetColor(1.f, 0.f, 0.f);
	DrawArc(g_Player.pos.x - g_Player.width * 0.5f, (g_Player.pos.y - g_Player.height * 0.5f), 10.f, 10.f, 0.f, 2 * g_Pi);
	DrawArc(g_Player.pos.x + g_Player.width * 0.5f, (g_Player.pos.y - g_Player.height * 0.5f), 10.f, 10.f, 0.f, 2 * g_Pi);
	DrawArc(g_Player.pos.x - g_Player.width * 0.5f, (g_Player.pos.y + g_Player.height * 0.5f), 10.f, 10.f, 0.f, 2 * g_Pi);
	DrawArc(g_Player.pos.x + g_Player.width * 0.5f, (g_Player.pos.y + g_Player.height * 0.5f), 10.f, 10.f, 0.f, 2 * g_Pi);
}

void UpdatePlayer(float elapsedSec)
{
	switch (g_Player.moveState)
	{
		case lastPressed::none:
			g_Player.speed.x = 0;
			break;
		case lastPressed::left:
			g_Player.speed.x = -g_MoveSpeed;
			break;
		case lastPressed::right:
			g_Player.speed.x = g_MoveSpeed;
			break;
	}
	//position the player will attempt to move to. the position is located in the exact center of the rectangle, hence the half height and half width increases to find the borders of it
	Point2f attemptPos = Point2f{ g_Player.pos.x + (g_Player.speed.x * elapsedSec), g_Player.pos.y + (g_Player.speed.y * elapsedSec) };

	//finds the positions of the corners of the rectangle
	//this is used in conjunction with the attempted position to determine the direction the rectangle is colliding from
	Point2f oldLowerRightPos{ g_Player.pos.x + g_Player.width * 0.5f - 1, g_Player.pos.y + g_Player.height * 0.5f - 1 };
	Point2f oldLowerLeftPos{ g_Player.pos.x - g_Player.width * 0.5f + 1, g_Player.pos.y + g_Player.height * 0.5f - 1 };
	Point2f oldUpperLeftPos{ g_Player.pos.x - g_Player.width * 0.5f + 1, g_Player.pos.y - g_Player.height * 0.5f + 1 };
	Point2f oldUpperRightPos{ g_Player.pos.x + g_Player.width * 0.5f - 1, g_Player.pos.y - g_Player.height * 0.5f + 1 };

	//triggers gravity if walking off side of platform
	if (g_pGridArray[GetIndexFromPos(Point2f{ oldLowerRightPos.x, oldLowerRightPos.y + 2.f })] == 0 && g_pGridArray[GetIndexFromPos(Point2f{ oldLowerLeftPos.x, oldLowerLeftPos.y + 2.f })] == 0)
	{
		g_Player.falling = true;
	}

	//finds the positions of the corners of the new attempted position of the rectangle and then finds the index number for what tile they are colliding with
	Point2f lowerRightPos{ attemptPos.x + g_Player.width * 0.5f - 1, attemptPos.y + g_Player.height * 0.5f - 1 };
	Point2f lowerLeftPos{ attemptPos.x - g_Player.width * 0.5f + 1, attemptPos.y + g_Player.height * 0.5f - 1 };
	Point2f upperLeftPos{ attemptPos.x - g_Player.width * 0.5f + 1, attemptPos.y - g_Player.height * 0.5f + 1 };
	Point2f upperRightPos{ attemptPos.x + g_Player.width * 0.5f - 1, attemptPos.y - g_Player.height * 0.5f + 1 };
	int lowerRightIndexPos{ GetIndexFromPos(lowerRightPos) };
	int lowerLeftIndexPos{ GetIndexFromPos(lowerLeftPos) };
	int upperLeftIndexPos{ GetIndexFromPos(upperLeftPos) };
	int upperRightIndexPos{ GetIndexFromPos(upperRightPos) };
	bool collidingUp{};
	bool collidingDown{};
	bool collidingLeft{};
	bool collidingRight{};

	//vertical collision
	if (g_Player.falling)
	{
		//downward collision
		if (g_pGridArray[lowerRightIndexPos] == 1 || g_pGridArray[lowerLeftIndexPos] == 1)
		{
			collidingDown = true;
		}

		//upward collision
		if (g_pGridArray[upperRightIndexPos] == 1 || g_pGridArray[upperLeftIndexPos] == 1)
		{
			collidingUp = true;
		}
	}

	//horizontal collision

	Point2f oldLeftPos{};
	Point2f oldRightPos{};
	Point2f leftPos{};
	Point2f rightPos{};
	int leftIndexPos{};
	int rightIndexPos{};

	//scans along height of rectangle to find collision, then breaks loop
	for (float index{ 1 }; index < g_Player.height - 1; ++index)
	{
		oldLeftPos = Point2f{ g_Player.pos.x - g_Player.width * 0.5f, (g_Player.pos.y - g_Player.height * 0.5f) + index };
		oldRightPos = Point2f{ g_Player.pos.x + g_Player.width * 0.5f, (g_Player.pos.y - g_Player.height * 0.5f) + index };
		leftPos = Point2f{ attemptPos.x - g_Player.width * 0.5f, (attemptPos.y - g_Player.height * 0.5f) + index };
		rightPos = Point2f{ attemptPos.x + g_Player.width * 0.5f, (attemptPos.y - g_Player.height * 0.5f) + index };
		leftIndexPos = GetIndexFromPos(leftPos);
		rightIndexPos = GetIndexFromPos(rightPos);

		//leftward collision
		if (g_pGridArray[leftIndexPos] == 1)
		{
			if (g_pGridArray[GetIndexFromPos(Point2f{ leftPos.x, oldLeftPos.y })] == 1)
			{
				collidingLeft = true;
				if (collidingDown && g_pGridArray[lowerRightIndexPos] == 0)
				{
					collidingDown = false;
				}
				if (collidingUp && g_pGridArray[upperRightIndexPos] == 0)
				{
					collidingUp = false;
				}
			}
			else
			{
				for (float diffIndex{ 1 }; diffIndex < (g_Player.height - index); ++diffIndex)
				{
					if (g_pGridArray[GetIndexFromPos(Point2f{ leftPos.x, oldLeftPos.y + diffIndex })] == 1)
					{
						collidingLeft = true;
						if (collidingDown && g_pGridArray[lowerRightIndexPos] == 0)
						{
							collidingDown = false;
						}
						if (collidingUp && g_pGridArray[upperRightIndexPos] == 0)
						{
							collidingUp = false;
						}
						break;
					}
				}
			}
			break;
		}

		//rightward collision
		else if (g_pGridArray[rightIndexPos] == 1)
		{
			if (g_pGridArray[GetIndexFromPos(Point2f{ rightPos.x, oldRightPos.y })] == 1)
			{
				collidingRight = true;
				if (collidingDown && g_pGridArray[lowerLeftIndexPos] == 0)
				{
					collidingDown = false;
				}
				if (collidingUp && g_pGridArray[upperLeftIndexPos] == 0)
				{
					collidingUp = false;
				}
			}
			else
			{
				for (float diffIndex{ 1 }; diffIndex < (g_Player.height - index); ++diffIndex)
				{
					if (g_pGridArray[GetIndexFromPos(Point2f{ rightPos.x, oldRightPos.y + diffIndex })] == 1)
					{
						collidingRight = true;
						if (collidingDown && g_pGridArray[lowerLeftIndexPos] == 0)
						{
							collidingDown = false;
						}
						if (collidingUp && g_pGridArray[upperLeftIndexPos] == 0)
						{
							collidingUp = false;
						}
						break;
					}
				}
			}
			break;
		}
	}
	if (collidingDown)
	{
		g_Player.pos.y = GetRow(lowerLeftIndexPos, g_GridWidth) * g_GridSize - g_Player.height * 0.5f - 1.f;
		attemptPos.y = g_Player.pos.y;
		g_Player.speed = Vector2f{ g_Player.speed.x, 0.f };
		g_Player.falling = false;
	}
	if (collidingUp)
	{
		g_Player.pos.y = (GetRow(upperLeftIndexPos, g_GridWidth) + 1) * g_GridSize + g_Player.height * 0.5f - 1.f;
		attemptPos.y = g_Player.pos.y;
		g_Player.speed = Vector2f{ g_Player.speed.x, 0.f };
	}
	//no vertical collision? keep falling.
	if (g_Player.falling)
	{
		g_Player.speed = Add(g_Player.speed, Vector2f{ 0.f, -(g_Gravity * elapsedSec) });
	}

	if (collidingLeft)
	{
		g_Player.pos.x = (GetCol(leftIndexPos, g_GridWidth) + 1) * g_GridSize + g_Player.width * 0.5f + 1.f;
		attemptPos.x = g_Player.pos.x;
		g_Player.speed = Vector2f{ 0.f, g_Player.speed.y };
	}
	if (collidingRight)
	{
		g_Player.pos.x = GetCol(rightIndexPos, g_GridWidth) * g_GridSize - g_Player.width * 0.5f - 1.f;
		attemptPos.x = g_Player.pos.x;
		g_Player.speed = Vector2f{ 0.f, g_Player.speed.y };
	}

	//moves player to the the attempted position calculated at the beginning and modified through these collision checks
	g_Player.pos = attemptPos;

	//updates rectangle info for drawing the player
	g_Player.rect.top = g_Player.pos.y - g_Player.height / 2;
	g_Player.rect.left = g_Player.pos.x - g_Player.width / 2;
}

#pragma endregion River

#pragma region Attila
void Init() {
	g_Debug = true;

	g_GridSize = 80.f;

	g_GridWidth = static_cast<int>(g_WindowWidth / g_GridSize);
	g_GridHeight = static_cast<int>(g_WindowHeight / g_GridSize);
	g_GridArraySize = g_GridHeight * g_GridWidth;


	//Changed array - River
	g_pGridArray = new int[g_GridArraySize] 
		{
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
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

#pragma region River

int GetIndexFromPos(const Point2f& pos)
{
	int index{ static_cast<int>(pos.y / g_GridSize) * g_GridWidth + static_cast<int>(pos.x / g_GridSize) };
	return index;
}

#pragma endregion River


#pragma endregion ownDefinitions