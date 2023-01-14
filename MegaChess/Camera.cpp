#include "Camera.h"
//#include <SDL.h> //not need because its already in MegaEngine

#include <cstdio>

SDL_FRect Camera::viewportRect=SDL_FRect();

float Camera::zoom=1.f;

void Camera::render()
{
    SDL_RenderSetScale(MegaEngine::renderer,zoom,zoom);

    // Create a viewport using the x and y position of the camera

    viewportRect.w=MegaEngine::getWindowBounds().w / zoom;
    viewportRect.h=MegaEngine::getWindowBounds().h / zoom;

    SDL_Rect viewRect;
    viewRect.x=(int)viewportRect.x;
    viewRect.y=(int)viewportRect.y;
    viewRect.w=(int)viewportRect.w;
    viewRect.h=(int)viewportRect.h;
    SDL_RenderSetViewport(MegaEngine::renderer,&viewRect);
}

void Camera::SetPosition(float X,float Y)
{
    printf("SetPosition called with X=%f, Y=%f\n",X,Y);
    viewportRect.x+=X;
    viewportRect.y+=Y;
}


float Camera::GetX()
{
    return viewportRect.x;
}

float Camera::GetY()
{
    return viewportRect.y;
}

float Camera::GetZoom()
{
    return zoom;
}

void Camera::SetZoom(float Zoom)
{
    zoom=Zoom;
}

void Camera::MoveLeft(float val)
{
    viewportRect.x-=val;
}

void Camera::MoveRight(float val)
{
    viewportRect.x+=val;
}

