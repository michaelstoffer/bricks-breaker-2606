#pragma once
#include "Box.h"
#include "Ball.h"
#include <vector>

class Game
{
	Ball ball;
	Box paddle;

	// Store the bricks by value.
	std::vector<Box> bricks;
	bool gameOver = false;
	bool gameWon = false;

public:
	Game();
	bool Update();
	void Render() const;
	void Reset();
	void ResetBall();
	void CheckCollision();
};