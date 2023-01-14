#include "MegaEngine.h"

#include "Game.h"
#include "Assets.h"
#include "Camera.h"


struct SDL_Window* MegaEngine::window;
struct SDL_Renderer* MegaEngine::renderer=nullptr;
bool MegaEngine::quit=false;
const int MegaEngine::INPUT_UP=0b00000001;
const int MegaEngine::INPUT_DOWN=0b00000010;
const int MegaEngine::INPUT_LEFT=0b00000100;
const int MegaEngine::INPUT_RIGHT=0b00001000;
const int MegaEngine::INPUT_FIRE1=0b00010000;
int MegaEngine::inputbyte=0;
Uint32 MegaEngine::mouseX=0;
Uint32 MegaEngine::mouseY=0;
bool MegaEngine::mouseButtonHeld1=false;
bool MegaEngine::mouseButtonClicked1=false;
float MegaEngine::deltaTime=0.f;
int MegaEngine::gameWidth=0;
int MegaEngine::gameHeight=0;
b2World* MegaEngine::world=nullptr;
b2Vec2 MegaEngine::gravity=b2Vec2(0.f,0.f);
bool MegaEngine::isPhysicsEnabled=false;

void MegaEngine::initGame(int GameWidth,int GameHeight)
{
	Camera::SetZoom(2.f);

	gameWidth=GameWidth;
	gameHeight=GameHeight;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Failed to initialize SDL: %s",SDL_GetError());
		return;
	}

	// Create a window
	Uint32 flags=SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP;
	window=SDL_CreateWindow("SDL Game",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,gameWidth,gameHeight,flags);
	if(window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,"Failed to create window: %s",SDL_GetError());
		SDL_Quit();
		return;
	}

	renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

	//SDL_EventState(SDL_MOUSEMOTION,SDL_ENABLE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	Assets::Init(renderer);
}

void MegaEngine::closeWindow()
{
	// Clean up
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Rect MegaEngine::getWindowBounds()
{
	int x=0;
	int y=0;
	SDL_GetWindowPosition(window,&x,&y);
	int w=0;
	int h=0;
	SDL_GetWindowSize(window,&w,&h);
	SDL_Rect rect={x,y,w,h};
	return rect;
}

float MegaEngine::getDeltaTime()
{
	return deltaTime;
}

void MegaEngine::setDeltaTime(float dTime)
{
	deltaTime=dTime;
}

void MegaEngine::runGameLoop(Game* game)
{
	// Wait for the user to close the window

	Uint32 lastTime=SDL_GetTicks();
	SDL_Event event;
	while(!quit)
	{
		handleInput();

		if(isPhysicsEnabled)
		{
			float timeStep=1.f / 60.f;
			int32 velocityIterations=6;
			int32 positionIterations=2;
			world->Step(timeStep,velocityIterations,positionIterations);
		}

		Uint32 currentTime=SDL_GetTicks();
		MegaEngine::setDeltaTime((currentTime - lastTime) / 1000.0f);
		lastTime=currentTime;

		//updates
		game->update();




		//rendering
		SDL_Color clearColor={0x1f, 0x1f, 0x1f, 0x1f};
		SDL_SetRenderDrawColor(MegaEngine::renderer,clearColor.r,clearColor.g,clearColor.b,clearColor.a);

		SDL_RenderClear(MegaEngine::renderer);

		game->render();

		Camera::render();

		SDL_RenderPresent(MegaEngine::renderer);


	}
}

void MegaEngine::handleInput()
{
	mouseButtonClicked1=false;
	// Poll for events until none are available
	SDL_Event event;
	//int input=0;
	while(SDL_PollEvent(&event))
	{
		// Handle the event
		switch(event.type)
		{
		case SDL_WINDOWEVENT:
			if(event.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				//int newWidth=event.window.data1;
				//int newHeight=event.window.data2;
				//if(newWidth * GAME_HEIGHT / GAME_WIDTH <= newHeight)
				//{
				//	newWidth=(int)(newHeight * ASPECT_RATIO);
				//}
				//else
				//{
				//	newHeight=newWidth * GAME_HEIGHT / GAME_WIDTH;
				//}
				//SDL_SetWindowSize(window,newWidth,newHeight);
				//printf("window resized\n");
			}
			break;
		case SDL_QUIT:
			// Handle quit events
			quit=true;
			break;
		case SDL_MOUSEMOTION:
			//Update the mouse position
			mouseX = event.motion.x / Camera::GetZoom();
			mouseY = event.motion.y / Camera::GetZoom();
			//printf("MouseXY:(%u, %u)\n",mouseX,mouseY);
			break;
		case SDL_MOUSEBUTTONDOWN:
			//printf("Mouse clicked\n");
			mouseButtonHeld1=true;
			mouseButtonClicked1=true;
			break;
		case SDL_MOUSEBUTTONUP:
			//printf("Mouse released\n");
			
			mouseButtonHeld1=false;
			break;
		case SDL_KEYDOWN:
			if(event.key.keysym.sym == SDLK_w)
			{
				inputbyte|=INPUT_UP;
			}
			if(event.key.keysym.sym == SDLK_s)
			{
				inputbyte|=INPUT_DOWN;
			}
			if(event.key.keysym.sym == SDLK_a)
			{
				inputbyte|=INPUT_LEFT;
			}
			if(event.key.keysym.sym == SDLK_d)
			{
				inputbyte|=INPUT_RIGHT;
			}
			if(event.key.keysym.sym == SDLK_SPACE)
			{
				inputbyte|=INPUT_FIRE1;
			}
			break;
		case SDL_KEYUP:
			if(event.key.keysym.sym == SDLK_w)
			{
				inputbyte&=~INPUT_UP;
			}
			if(event.key.keysym.sym == SDLK_s)
			{
				inputbyte&=~INPUT_DOWN;
			}
			if(event.key.keysym.sym == SDLK_a)
			{
				inputbyte&=~INPUT_LEFT;
			}
			if(event.key.keysym.sym == SDLK_d)
			{
				inputbyte&=~INPUT_RIGHT;
			}
			if(event.key.keysym.sym == SDLK_SPACE)
			{
				inputbyte&=~INPUT_FIRE1;
			}
			break;
			// Add additional event handling code here...
		}
	}
}

void MegaEngine::initPhysics(b2Vec2 Gravity)
{
	gravity=Gravity;
	isPhysicsEnabled=true;
	world=new b2World(gravity);


}

b2Vec2 MegaEngine::GetGravity()
{
	return gravity;
}

void MegaEngine::SetGravity(float X,float Y)
{
	gravity.x=X;
	gravity.y=Y;
}