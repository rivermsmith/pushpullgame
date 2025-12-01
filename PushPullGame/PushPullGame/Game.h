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

enum class entityType
{
	bullet,
	enemy,
	player
};

struct Entity
{
	float width{};
	float height{};
	Point2f pos{};
	float hp{};
	entityType type{};
	Vector2f speed{};
	Rectf rect{ pos.x - width / 2, pos.y - height / 2, width, height };
};

struct Player
{
	Entity playerEntity{ 
		70.f,
		140.f,
		Point2f{ g_WindowWidth / 2, g_WindowHeight - 200.f },
		100.f,
		entityType::player 
	};
	bool falling{ true };
	bool left{ false };
	bool right{ false };
	lastPressed moveState{};
};
struct Enemy {
	Entity enemyEntity{};
	int fireSpeed{};
	float lastShot{};
};

Player g_Player{};
Enemy g_Enemy{
	Entity{
		40.f, 40.f,
		Point2f{160.f, 160.f},
		10.f, entityType::enemy
	},
	3, 0.f
};
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
Point2f HandleCollision(Entity& entity, Point2f& attemptPos);

#pragma endregion River

#pragma region Attila
void Init();
void Delete();
void DrawPlatforms();
void DrawPushPullRange();
void DrawEnemy(const Enemy& enemy);

void UpdateEnemy(Enemy& enemy, float elapsedSec);

//void CreatePlatforms(int array[], int gridWidth, int startRow, int endRow, int startCol, int endCol);

int GetIndex(int rowIdx, int colIdx, int nrCols);
int GetRow(int index, int numCols);
int GetCol(int index, int numCols);
Color4f rgba(float r, float g, float b, float a = 255);
float calculateAngle(const Point2f& originPoint, const Point2f& endPoint);
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
