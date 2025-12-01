#pragma once
#include <utils.h>
using namespace utils;

#pragma region gameInformation
// TODO: Set your name and group in the title here
std::string g_WindowTitle{ "PLACEHOLDER - Szeremi, Attila & Smith, River - 1DAE14-15" };

// Change the window dimensions here
float g_WindowWidth{ 1280 };
float g_WindowHeight{ 720 };
#pragma endregion gameInformation



#pragma region ownDeclarations
// Declare your own global variables here

bool g_Debug{};

#pragma region Attila
float
	g_GridSize{};
int
	g_GridHeight{},
	g_GridWidth{},
	g_GridArraySize{};

int* g_pGridArray{};

Point2f g_MousePosition{};
#pragma endregion Attila
#pragma region River

enum class lastPressed
{
	none,
	left,
	right
};

struct Player
{
	const float width{ 70.f };
	const float height{ 140.f };
	Point2f pos{ g_WindowWidth / 2, g_WindowHeight - 200.f };
	Circlef circ{ pos, 50.f };
	Rectf rect{ pos.x - width / 2, pos.y - height / 2, width, height };
	float hp{ 100.f };
	Vector2f speed{};
	bool falling{ true };
	bool left{ false };
	bool right{ false };
	lastPressed moveState{};
};
Player g_Player{};
const float g_Gravity{ -9.81f * 300 };
const float g_MoveSpeed{ 900.f };
const float g_JumpSpeed{ 1800.f };
bool g_HoldJump{};
#pragma endregion River

// Declare your own functions here
#pragma region River
void UpdatePlayer(float elapsedSec);
void DrawPlayer();

int GetIndexFromPos(const Point2f& pos);
#pragma endregion River

#pragma region Attila
void Init();
void Delete();
void DrawPlatforms();
void DrawPushPullRange();

//void CreatePlatforms(int array[], int gridWidth, int startRow, int endRow, int startCol, int endCol);

int GetIndex(int rowIdx, int colIdx, int nrCols);
int GetRow(int index, int numCols);
int GetCol(int index, int numCols);
#pragma endregion Attila

#pragma endregion ownDeclarations

#pragma region gameFunctions											
void Start();
void Draw();
void Update(float elapsedSec);
void End();
#pragma endregion gameFunctions

#pragma region inputHandling											
void OnKeyDownEvent(SDL_Keycode key);
void OnKeyUpEvent(SDL_Keycode key);
void OnMouseMotionEvent(const SDL_MouseMotionEvent& e);
void OnMouseDownEvent(const SDL_MouseButtonEvent& e);
void OnMouseUpEvent(const SDL_MouseButtonEvent& e);
#pragma endregion inputHandling
