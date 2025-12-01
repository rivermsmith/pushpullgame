#pragma once
#include "structs.h"
#include <vector>
#include <string>

namespace utils
{

	const float g_Pi{ 3.1415926535f };

#pragma region OpenGLDrawFunctionality
	void ClearBackground(float r, float g, float b);
	void ClearBackground();
	// Choose a floating point value for each color component between 0.0f and 1.0f. The a (alpha) is optional.
	// Usage: SetColor(1.0f, 0.0f, 0.0f); set the drawing color to red
	void SetColor(float r, float g, float b, float a = 1);
	// Pass a Color4f object to the function
	void SetColor(const Color4f& color);

	// Draws a line by connecting the two points specified by (x1,y1) and (x2,y2)
	void DrawLine(float x1, float y1, float x2, float y2, float lineWidth = 1.0f);
	// Draws a line by connecting the two points specified by p1 and p1
	void DrawLine(const Point2f& p1, const Point2f& p2, float lineWidth = 1.0f);

	void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth = 1);
	void DrawTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3, float lineWidth = 1);
	void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void FillTriangle(const Point2f& p1, const Point2f& p2, const Point2f& p3);
	void DrawRect(float left, float top, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Point2f& topLeft, float width, float height, float lineWidth = 1.0f);
	void DrawRect(const Rectf& rect, float lineWidth = 1.0f);
	void FillRect(float left, float top, float width, float height);
	void FillRect(const Point2f& topLeft, float width, float height);
	void FillRect(const Rectf& rect);

	void DrawEllipse(float centerX, float centerY, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Point2f& center, float radX, float radY, float lineWidth = 1.0f);
	void DrawEllipse(const Ellipsef& ellipse, float lineWidth = 1.0f);
	void FillEllipse(float centerX, float centerY, float radX, float radY);
	void FillEllipse(const Ellipsef& ellipse);
	void FillEllipse(const Point2f& center, float radX, float radY);

	// Draws an arc. The angle parameters are in radians, not in degrees.
	void DrawArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f);
	// Draws an arc. The angle parameters are in radians, not in degrees.
	void DrawArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle, float lineWidth = 1.0f);
	// Fills an arc. The angle parameters are in radians, not in degrees.
	void FillArc(float centerX, float centerY, float radX, float radY, float fromAngle, float tillAngle);
	// Fills an arc. The angle parameters are in radians, not in degrees.
	void FillArc(const Point2f& center, float radX, float radY, float fromAngle, float tillAngle);

	void DrawPolygon(const std::vector<Point2f>& vertices, bool closed = true, float lineWidth = 1.0f);
	void DrawPolygon(const Point2f* pVertices, size_t nrVertices, bool closed = true, float lineWidth = 1.0f);
	void FillPolygon(const std::vector<Point2f>& vertices);
	void FillPolygon(const Point2f* pVertices, size_t nrVertices);
#pragma endregion OpenGLDrawFunctionality

#pragma region TextureFunctionality

	struct Texture
	{
		GLuint id;
		float width;
		float height;
	};
	bool TextureFromFile(const std::string& path, Texture& texture);
	bool TextureFromString(const std::string& text, TTF_Font* pFont, const Color4f& textColor, Texture& texture);
	bool TextureFromString(const std::string& text, const std::string& fontPath, int ptSize, const Color4f& textColor, Texture& texture);
	void TextureFromSurface(const SDL_Surface* pSurface, Texture& textureData);
	void DrawTexture(const Texture& texture, const Point2f& dstTopLeft, const Rectf& srcRect = {});
	void DrawTexture(const Texture& texture, const Rectf& dstRect, const Rectf& srcRect = {});
	void DeleteTexture(Texture& texture);
#pragma endregion TextureFunctionality

#pragma region CollisionFunctionality

	float GetDistance(const Point2f& point1, const Point2f& point2);
	float GetDistance(float x1, float y1, float x2, float y2);

	bool IsPointInCircle(const Circlef& circle, const Point2f& point);
	bool IsPointInCircle(const Circlef& circle, float pointX, float pointY);
	bool IsPointInCircle(const Point2f& circleCenter, float radius, const Point2f& point);
	bool IsPointInCircle(const Point2f& circleCenter, float radius, float pointX, float pointY);
	bool IsPointInCircle(float circleX, float circleY, float radius, const Point2f& point);
	bool IsPointInCircle(float circleX, float circleY, float radius, float pointX, float pointY);

	bool IsPointInRect(const Rectf& rectangle, const Point2f& point);
	bool IsPointInRect(const Rectf& rectangle, float pointX, float pointY);
	bool IsPointInRect(float rectangleX, float rectangleY, float rectangleWidth, float rectangleHeight, const Point2f& point);
	bool IsPointInRect(float rectangleX, float rectangleY, float rectangleWidth, float rectangleHeight, float pointX, float pointY);

	bool IsOverLapping(const Circlef& circle1, const Circlef& circle2);
	bool IsOverLapping(const Circlef& circle1, const Point2f& circle2Center, float radius2);
	bool IsOverLapping(const Circlef& circle1, float circle2CenterX, float Circle2CenterY, float radius2);
	bool IsOverLapping(const Point2f& circle1Center, float radius1, const Point2f& circle2Center, float radius2);
	bool IsOverLapping(const Point2f& circle1Center, float radius1, float Circle2CenterX, float Circle2CenterY, float radius2);
	bool IsOverLapping(float circle1CenterX, float circle1CenterY, float radius1, float circle2CenterX, float circle2CenterY, float radius2);
			   
	bool IsOverLapping(const Rectf& rectangle1, const Rectf& rectangle2);
	bool IsOverLapping(const Rectf& rectangle1, float rectangle2Left, float rectangle2Top, float rectangle2Width, float rectangle2Heigth);
	bool IsOverLapping(float rectangle1Left, float rectangle1Height, float rectangle1Width, float rectangle1Heigth, float rectangle2Left, float rectangle2Height, float rectangle2Width, float rectangle2Heigth);

#pragma endregion CollisionFunctionality

#pragma region Vectors
	void DrawVector(const Vector2f& vector);
	void DrawVector(float vectorX, float VectorY);
	void DrawVector(const Point2f& startPoint, const Vector2f& vector);
	void DrawVector(const Point2f& startPoint, float vectorX, float VectorY);
	void DrawVector(float startPointX, float startPointY, const Vector2f& vector);
	void DrawVector(float startPointX, float startPointY, float vectorX, float vectorY);

	std::string ToString(const Vector2f& vector);
	std::string ToString(float vectorX, float vectorY);

	Vector2f Add(const Vector2f& vector1, const Vector2f& vector2);
	Vector2f Add(const Vector2f& vector1, float vector2X, float vector2Y);
	Vector2f Add(float vector1X, float vector1Y, float vector2X, float vector2Y);
	
	Vector2f Substact(const Vector2f& vector1, const Vector2f& vector2);
	Vector2f Substact(const Vector2f& vector1, float vector2X, float vector2Y);
	Vector2f Substact(float vector1X, float vector1Y, float vector2X, float vector2Y);

	float DotProduct(const Vector2f& vector1, const Vector2f& vector2);
	float DotProduct(const Vector2f& vector1, float vector2X, float vector2Y);
	float DotProduct(float vector1X, float vector1Y, float vector2X, float vector2Y);

	float CrossProduct(const Vector2f& vector1, const Vector2f& vector2);
	float CrossProduct(const Vector2f& vector1, float vector2X, float vector2Y);
	float CrossProduct(float vector1X, float vector1Y, float vector2X, float vector2Y);

	float Length(const Vector2f& vector);
	float Length(float vectorX, float vectorY);

	Vector2f Scale(const Vector2f& vector, float scale);
	Vector2f Scale(float vectorX, float vectorY, float scale);

	Vector2f Normalize(const Vector2f& vector);
	Vector2f Normalize(float vectorX, float vectorY);

	float AngleBetween(const Vector2f& vector1, const Vector2f& vector2);
	float AngleBetween(const Vector2f& vector1, float vector2X, float vector2Y);
	float AngleBetween(float vector1X, float vector1Y, float vector2X, float vector2Y);

	bool AreEqual(const Vector2f& vector1, const Vector2f& vector2);
	bool AreEqual(const Vector2f& vector1, float vector2X, float vector2Y);
	bool AreEqual(float vector1X, float vector1Y, float vector2X, float vector2Y);

#pragma endregion Vectors
}