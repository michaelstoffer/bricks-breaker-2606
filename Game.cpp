#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	gameOver = false;
	gameWon = false;
	bricks.clear();

	// Five evenly spaced bricks in a single row.
	for (int i = 0; i < 5; ++i)
	{
		Box brick;
		brick.width = 10;
		brick.height = 2;
		brick.x_position = 5 + i * 15;
		brick.y_position = 5;
		brick.doubleThick = true;
		brick.color = ConsoleColor::DarkGreen;
		bricks.push_back(brick);
	}
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	if (gameOver)
		return true;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (GetAsyncKeyState(VK_SPACE) & 0x1)
		ball.moving = !ball.moving;

	// Check the next ball position before moving it.
	if (ball.moving)
		CheckCollision();

	if (!gameOver)
		ball.Update();

	return true;
}

// All rendering, including text, occurs in the Render function.
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();

	paddle.Draw();
	ball.Draw();

	for (size_t i = 0; i < bricks.size(); ++i)
		bricks[i].Draw();

	if (gameOver)
	{
		const char* message = gameWon
			? "You win! Press 'R' to play again."
			: "You lose. Press 'R' to play again.";
		int messageWidth = (int)strlen(message);
		Console::ForegroundColor(ConsoleColor::White);
		Console::SetCursorPosition((WINDOW_WIDTH - messageWidth) / 2, WINDOW_HEIGHT / 2);
		std::cout << message;
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	int nextX = ball.x_position + ball.x_velocity;
	int nextY = ball.y_position + ball.y_velocity;

	// Check each brick and remove it after three hits.
	for (size_t i = 0; i < bricks.size(); ++i)
	{
		if (bricks[i].Contains(nextX, nextY))
		{
			bricks[i].color = ConsoleColor(bricks[i].color - 1);
			ball.y_velocity *= -1;

			if (bricks[i].color == ConsoleColor::Black)
				bricks.erase(bricks.begin() + i);

			if (bricks.empty())
			{
				gameWon = true;
				gameOver = true;
				ball.moving = false;
			}
			break;
		}
	}

	if (gameOver)
		return;

	if (paddle.Contains(nextX, nextY) && ball.y_velocity > 0)
		ball.y_velocity *= -1;

	if (nextY >= WINDOW_HEIGHT)
	{
		gameWon = false;
		gameOver = true;
		ball.moving = false;
	}
}
