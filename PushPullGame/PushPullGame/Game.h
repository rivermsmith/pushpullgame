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

#pragma region enums

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

#pragma endregion enums

#pragma region structs

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
struct Bullet
{
	Entity bulletEntity{};
	int bulletIndex{};
};

#pragma endregion structs

bool g_Debug{};
const float g_Gravity{ -9.81f * 300 };

Point2f g_MousePosition{};
#pragma region grid
int* g_pGridArray{};
float
g_GridSize{};
int
g_GridHeight{},
g_GridWidth{},
g_GridArraySize{};
#pragma endregion grid
#pragma region player
Player g_Player{};
const float g_MoveSpeed{ 900.f };
const float g_JumpSpeed{ 1800.f };
bool g_HoldJump{};
#pragma endregion player
#pragma region enemies
Enemy g_Enemy{
	Entity{
		40.f, 40.f,
		Point2f{160.f, 160.f},
		10.f, entityType::enemy
	},
	1, 0.f
};
#pragma endregion enemies
#pragma region bullets
int g_BulletAmount{ 0 };
const int g_MaxBulletAmount{ 50 };
Bullet* g_pBulletArray{};
#pragma endregion bullets

// Declare your own functions here

#pragma region debug
void DebugBullet();
#pragma endregion debug

void Init();
void Delete();

#pragma region draw

void DrawPlayer();
void DrawBullets();
void DrawPlatforms();
void DrawPushPullRange();
void DrawEnemy(const Enemy& enemy);

#pragma endregion draw

#pragma region update

void UpdatePlayer(float elapsedSec);
void UpdateBullets(float elapsedSec);
void UpdateEnemy(Enemy& enemy, float elapsedSec);

int CreateBullet(const Point2f& startPos, const Vector2f& speed);
void RemoveBullet(Bullet& bullet);

Point2f HandleWallCollision(Entity& entity, Point2f& attemptPos, int bulletIndex = 0);
void HandleEntityCollision(Entity& entity, Point2f& attemptPos);

#pragma endregion update

#pragma region utils

int GetIndex(int rowIdx, int colIdx, int nrCols);
int GetRow(int index, int numCols);
int GetCol(int index, int numCols);
int GetIndexFromPos(const Point2f& pos);

Color4f rgba(float r, float g, float b, float a = 255);
float calculateAngle(const Point2f& originPoint, const Point2f& endPoint);

#pragma endregion utils

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
