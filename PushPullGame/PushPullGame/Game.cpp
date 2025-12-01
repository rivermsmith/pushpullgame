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
	DrawEnemy(g_Enemy);
#pragma endregion Attila

#pragma region River
	DrawPlayer();
	DrawBullets();

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
	UpdateBullets(elapsedSec);

#pragma endregion River
	UpdateEnemy(g_Enemy, elapsedSec);
}

void End()
{
	// free game resources here
	Delete();
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
		g_Player.playerEntity.speed.x = -g_MoveSpeed;
		g_Player.left = true;
		g_Player.moveState = lastPressed::left;
		break;
	case SDLK_d:
		g_Player.playerEntity.speed.x = g_MoveSpeed;
		g_Player.right = true;
		g_Player.moveState = lastPressed::right;
		break;
	case SDLK_SPACE:
		if (!g_Player.falling && !g_HoldJump)
		{
			g_Player.playerEntity.speed = Add(g_Player.playerEntity.speed, Vector2f{ 0.f, -g_JumpSpeed });
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
		if (g_Player.playerEntity.speed.y < 0)
		{
			g_Player.playerEntity.speed.y = 0;
		}
		g_HoldJump = false;
		break;
	case SDLK_r:
		g_Debug = !g_Debug;
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
	FillRect(g_Player.playerEntity.rect);
	if (g_Debug) {
		SetColor(1.f, 0.f, 0.f);
		DrawVector(g_Player.playerEntity.pos, Scale(g_Player.playerEntity.speed, 0.125f));
		DrawArc(g_Player.playerEntity.pos.x - g_Player.playerEntity.width * 0.5f, (g_Player.playerEntity.pos.y - g_Player.playerEntity.height * 0.5f), 10.f, 10.f, 0.f, 2 * g_Pi);
		DrawArc(g_Player.playerEntity.pos.x + g_Player.playerEntity.width * 0.5f, (g_Player.playerEntity.pos.y - g_Player.playerEntity.height * 0.5f), 10.f, 10.f, 0.f, 2 * g_Pi);
		DrawArc(g_Player.playerEntity.pos.x - g_Player.playerEntity.width * 0.5f, (g_Player.playerEntity.pos.y + g_Player.playerEntity.height * 0.5f), 10.f, 10.f, 0.f, 2 * g_Pi);
		DrawArc(g_Player.playerEntity.pos.x + g_Player.playerEntity.width * 0.5f, (g_Player.playerEntity.pos.y + g_Player.playerEntity.height * 0.5f), 10.f, 10.f, 0.f, 2 * g_Pi);
	}
	SetColor(1.f, 0.f, 0.f);
}

void UpdatePlayer(float elapsedSec)
{
	//handles horizontal movement direction
	switch (g_Player.moveState)
	{
		case lastPressed::none:
			g_Player.playerEntity.speed.x = 0;
			break;
		case lastPressed::left:
			g_Player.playerEntity.speed.x = -g_MoveSpeed;
			break;
		case lastPressed::right:
			g_Player.playerEntity.speed.x = g_MoveSpeed;
			break;
	}

	//position the player will attempt to move to. the position is located in the exact center of the rectangle, hence the half height and half width increases to find the borders of it
	Point2f attemptPos = Point2f{ g_Player.playerEntity.pos.x + (g_Player.playerEntity.speed.x * elapsedSec), g_Player.playerEntity.pos.y + (g_Player.playerEntity.speed.y * elapsedSec) };

	//moves player to the the attempted position calculated at the beginning and modified through these collision checks
	g_Player.playerEntity.pos = HandleWallCollision(g_Player.playerEntity, attemptPos);

	//no vertical collision? keep falling.
	if (g_Player.falling)
	{
		g_Player.playerEntity.speed = Add(g_Player.playerEntity.speed, Vector2f{ 0.f, -(g_Gravity * elapsedSec) });
	}

	//updates rectangle info for drawing the player
	g_Player.playerEntity.rect.top = g_Player.playerEntity.pos.y - g_Player.playerEntity.height / 2;
	g_Player.playerEntity.rect.left = g_Player.playerEntity.pos.x - g_Player.playerEntity.width / 2;
}

void DrawBullets()
{
	SetColor(1.f, 0.6f, 1.f);
	for (int index{}; index < g_BulletAmount; ++index)
	{
		Bullet bullet{ g_pBulletArray[index] };
		FillRect(bullet.bulletEntity.rect);
	}
}

void UpdateBullets(float elapsedSec)
{
	//updates each bullet in the array
	for (int index{}; index < g_BulletAmount; ++index)
	{
		Bullet bullet{ g_pBulletArray[index] };
		Point2f attemptPos = Point2f{ bullet.bulletEntity.pos.x + (bullet.bulletEntity.speed.x * elapsedSec), bullet.bulletEntity.pos.y + (bullet.bulletEntity.speed.y * elapsedSec) };
		bullet.bulletEntity.pos = HandleWallCollision(bullet.bulletEntity, attemptPos, bullet.bulletIndex);
		bullet.bulletEntity.rect.top = bullet.bulletEntity.pos.y - bullet.bulletEntity.height / 2;
		bullet.bulletEntity.rect.left = bullet.bulletEntity.pos.x - bullet.bulletEntity.width / 2;
		g_pBulletArray[index] = bullet;
	}
}

Point2f HandleWallCollision(Entity& entity, Point2f& attemptPos, int bulletIndex)
{
	//finds the positions of the corners of the rectangle
	//this is used in conjunction with the attempted position to determine the direction the rectangle is colliding from
	Point2f oldLowerRightPos{ entity.pos.x + entity.width * 0.5f - 1, entity.pos.y + entity.height * 0.5f - 1 };
	Point2f oldLowerLeftPos{ entity.pos.x - entity.width * 0.5f + 1, entity.pos.y + entity.height * 0.5f - 1 };
	Point2f oldUpperLeftPos{ entity.pos.x - entity.width * 0.5f + 1, entity.pos.y - entity.height * 0.5f + 1 };
	Point2f oldUpperRightPos{ entity.pos.x + entity.width * 0.5f - 1, entity.pos.y - entity.height * 0.5f + 1 };

	//triggers gravity if walking off side of platform
	if (entity.type == entityType::player
		&& g_pGridArray[GetIndexFromPos(Point2f{ oldLowerRightPos.x, oldLowerRightPos.y + 2.f })] == 0
		&& g_pGridArray[GetIndexFromPos(Point2f{ oldLowerLeftPos.x, oldLowerLeftPos.y + 2.f })] == 0)
	{
		g_Player.falling = true;
	}

	//finds the positions of the corners of the new attempted position of the rectangle and then finds the index number for what tile they are colliding with
	Point2f lowerRightPos{ attemptPos.x + entity.width * 0.5f - 1, attemptPos.y + entity.height * 0.5f - 1 };
	Point2f lowerLeftPos{ attemptPos.x - entity.width * 0.5f + 1, attemptPos.y + entity.height * 0.5f - 1 };
	Point2f upperLeftPos{ attemptPos.x - entity.width * 0.5f + 1, attemptPos.y - entity.height * 0.5f + 1 };
	Point2f upperRightPos{ attemptPos.x + entity.width * 0.5f - 1, attemptPos.y - entity.height * 0.5f + 1 };
	int lowerRightIndexPos{ GetIndexFromPos(lowerRightPos) };
	int lowerLeftIndexPos{ GetIndexFromPos(lowerLeftPos) };
	int upperLeftIndexPos{ GetIndexFromPos(upperLeftPos) };
	int upperRightIndexPos{ GetIndexFromPos(upperRightPos) };
	bool collidingUp{};
	bool collidingDown{};
	bool collidingLeft{};
	bool collidingRight{};

	//vertical collision
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

	//horizontal collision

	Point2f oldLeftPos{};
	Point2f oldRightPos{};
	Point2f leftPos{};
	Point2f rightPos{};
	int leftIndexPos{};
	int rightIndexPos{};

	//scans along height of rectangle to find collision, then breaks loop
	for (float index{ 1 }; index < entity.height - 1; ++index)
	{
		oldLeftPos = Point2f{ entity.pos.x - entity.width * 0.5f, (entity.pos.y - entity.height * 0.5f) + index };
		oldRightPos = Point2f{ entity.pos.x + entity.width * 0.5f, (entity.pos.y - entity.height * 0.5f) + index };
		leftPos = Point2f{ attemptPos.x - entity.width * 0.5f, (attemptPos.y - entity.height * 0.5f) + index };
		rightPos = Point2f{ attemptPos.x + entity.width * 0.5f, (attemptPos.y - entity.height * 0.5f) + index };
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
				for (float diffIndex{ 1 }; diffIndex < (entity.height - index); ++diffIndex)
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
				for (float diffIndex{ 1 }; diffIndex < (entity.height - index); ++diffIndex)
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

	//if a bullet is colliding, deletes it
	if (entity.type == entityType::bullet) {
		if (collidingDown || collidingUp || collidingLeft || collidingRight)
		{
			RemoveBullet(g_pBulletArray[bulletIndex]);
		}
	}
	else
	{
		if (collidingDown)
		{
			entity.pos.y = GetRow(lowerLeftIndexPos, g_GridWidth) * g_GridSize - entity.height * 0.5f - 1.f;
			attemptPos.y = entity.pos.y;
			entity.speed = Vector2f{ entity.speed.x, 0.f };
			if (entity.type == entityType::player) {
				g_Player.falling = false;
			}
		}
		if (collidingUp)
		{
			entity.pos.y = (GetRow(upperLeftIndexPos, g_GridWidth) + 1) * g_GridSize + entity.height * 0.5f - 1.f;
			attemptPos.y = entity.pos.y;
			entity.speed = Vector2f{ entity.speed.x, 0.f };
		}

		if (collidingLeft)
		{
			entity.pos.x = (GetCol(leftIndexPos, g_GridWidth) + 1) * g_GridSize + entity.width * 0.5f + 1.f;
			attemptPos.x = entity.pos.x;
			entity.speed = Vector2f{ 0.f, entity.speed.y };
		}
		if (collidingRight)
		{
			entity.pos.x = GetCol(rightIndexPos, g_GridWidth) * g_GridSize - entity.width * 0.5f - 1.f;
			attemptPos.x = entity.pos.x;
			entity.speed = Vector2f{ 0.f, entity.speed.y };
		}
	}
	
	return attemptPos;
}

//to be implemented
void HandleEntityCollision(Entity& entity, Point2f& attemptPos)
{

}

//create 2 bullets on game start for testing
void DebugBullet()
{
	CreateBullet(Point2f{ g_WindowWidth - 200.f, 200.f }, Vector2f{ -200.f, 200.f });
	CreateBullet(Point2f{ 200.f, 200.f }, Vector2f{ 250.f, 200.f });
}

//creates a new bullet if there are less than max bullets, and returns the index of the created bullet within the bullet array. raises current bullet amount
int CreateBullet(const Point2f& startPos, const Vector2f& speed)
{
	if (g_BulletAmount < g_MaxBulletAmount)
	{
		Bullet bullet{};
		bullet.bulletEntity.pos = startPos;
		bullet.bulletEntity.width = 15.f;
		bullet.bulletEntity.height = 15.f;
		bullet.bulletEntity.speed = speed;
		bullet.bulletEntity.rect.width = 15.f;
		bullet.bulletEntity.rect.height = 15.f;
		g_pBulletArray[g_BulletAmount] = bullet;
		++g_BulletAmount;
		return g_BulletAmount - 1;
	}
	else return -1;
}

//removes a bullet from the bullet array and moves everything after it down by 1. decrements current bullet amount
void RemoveBullet(Bullet& bullet)
{
	for (int index{ bullet.bulletIndex }; index <= g_BulletAmount; ++index)
	{
		g_pBulletArray[index] = g_pBulletArray[index + 1];
		g_pBulletArray[index + 1] = Bullet{};
		--g_BulletAmount;
	}
}
#pragma endregion River

#pragma region Attila
void Init() {
	//Placeholder -River

	g_Debug = true;

	g_GridSize = 80.f;

	g_GridWidth = static_cast<int>(g_WindowWidth / g_GridSize);
	g_GridHeight = static_cast<int>(g_WindowHeight / g_GridSize);
	g_GridArraySize = g_GridHeight * g_GridWidth;

	g_pBulletArray = new Bullet[g_MaxBulletAmount]{};

	//for initial bullet testing only -River
	DebugBullet();

	//Changed array -River
	g_pGridArray = new int[g_GridArraySize] 
		{
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
			1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
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
void Delete() {
	delete[] g_pGridArray;
	g_pGridArray = nullptr;
	delete g_pBulletArray;
	g_pBulletArray = nullptr;
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
	const float
		rayLength{ 500.f },
		rayWidth{ g_Pi / 180 * 5 };
	float
		rayAngle{ 0 };

	rayAngle = calculateAngle(g_Player.playerEntity.pos, g_MousePosition);

	const Color4f
		purple{ 0.4157f, 0.051f, 0.6784f, 0.5f };

	const Point2f
		leftEnd{
			g_Player.playerEntity.pos.x + rayLength * std::cosf(rayAngle + rayWidth / 2.f),
			g_Player.playerEntity.pos.y - rayLength * std::sinf(rayAngle + rayWidth / 2.f)
		},
		rightEnd{
			g_Player.playerEntity.pos.x + rayLength * std::cosf(rayAngle - rayWidth / 2.f),
			g_Player.playerEntity.pos.y - rayLength * std::sinf(rayAngle - rayWidth / 2.f)
		};

	utils::SetColor(purple);
	utils::FillArc(g_Player.playerEntity.pos, rayLength, rayLength, rayAngle - rayWidth / 2.f, rayAngle + rayWidth / 2.f);
}
void DrawEnemy(const Enemy& enemy) {
	const Color4f
		gray{ rgba(119, 123, 134) };

	calculateAngle(enemy.enemyEntity.pos, g_Player.playerEntity.pos);

	utils::SetColor(gray);
	utils::FillRect(enemy.enemyEntity.rect);
}

void UpdateEnemy(Enemy& enemy, float elapsedSec) {
	enemy.lastShot += elapsedSec;

	if (enemy.lastShot > enemy.fireSpeed) {
		std::cout << "Enemy fire";
		enemy.lastShot = 0.f;
	}

	//std::cout << enemy.pos.x << "; " << enemy.pos.y << "\n";

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
Color4f rgba(float r, float g, float b, float a) {
	float colorBit{ 255 };
	return Color4f{ r / colorBit, g / colorBit, b / colorBit, a / colorBit };
}
float calculateAngle(const Point2f& originPoint, const Point2f& endPoint) {
	const float
		adj{ endPoint.x - originPoint.x },
		opp{ originPoint.y - endPoint.y },
		hyp{
			sqrtf(powf(adj, 2) + powf(opp, 2))
		};

	if (g_Debug) {
		const Color4f
			pink{ 1.f, 0.f, 1.f, 1.f },
			blue{ 0.f,0.f,1.f,1.f },
			green{ 0.f,1.f,0.f,1.f };

		utils::SetColor(pink);
		utils::DrawLine(endPoint, originPoint);

		utils::SetColor(blue);
		utils::DrawLine(originPoint, Point2f{ endPoint.x, originPoint.y });

		utils::SetColor(green);
		utils::DrawLine(originPoint, Point2f{ originPoint.x, endPoint.y });
	}

	if (adj < 0) {
		if (opp > 0) {
			const float cosine{ opp / hyp };
			return g_Pi / 2.f + std::acosf(cosine);
		}
		else {
			const float cosine{ std::abs(adj) / hyp };
			return g_Pi + std::acosf(cosine);
		}
	}
	else if (opp < 0) {
		const float cosine{ std::abs(opp) / hyp };
		return  g_Pi / 2.f * 3.f + std::acosf(cosine);
	}
	else {
		const float
			cosine{ adj / hyp };
		return std::acosf(cosine);
	}

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