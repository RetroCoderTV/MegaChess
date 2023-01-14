#ifndef MEGAENGINE_H
#define MEGAENGINE_H

#pragma once

#include <SDL.h>
#include <box2d/box2d.h>

class MegaEngine
{
public:
	static const int INPUT_UP;
	static const int INPUT_DOWN;
	static const int INPUT_LEFT;
	static const int INPUT_RIGHT;
	static const int INPUT_FIRE1;

	static SDL_Renderer* renderer;
	static void initGame(int GameWidth,int GameHeight);
	static void closeWindow();
	static SDL_Rect getWindowBounds();
	static float getDeltaTime();
	static void setDeltaTime(float dTime);

	static void runGameLoop(class Game* game);

	static void handleInput();
	static int inputbyte;
	static Uint32 mouseX;
	static Uint32 mouseY;
	static bool mouseButtonHeld1;
	static bool mouseButtonClicked1;


	static void initPhysics(b2Vec2 Gravity);

	static bool isPhysicsEnabled;
	static b2World* world;
	static b2Vec2 GetGravity();
	static void SetGravity(float X,float Y);

private:
	static SDL_Window* window;
	static bool quit;
	static float deltaTime;

	static int gameWidth;
	static int gameHeight;

	static b2Vec2 gravity;
};

#endif