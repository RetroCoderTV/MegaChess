#ifndef CAMERA_H
#define CAMERA_H

#pragma once

#include "MegaEngine.h"

class Camera
{
public:
    static void render();
    static void SetPosition(float X,float Y);
    static float  GetX();
    static float  GetY();
    static float GetZoom();
    static void SetZoom(float Zoom);

    static void MoveLeft(float val);
    static void MoveRight(float val);

private:
    static SDL_FRect viewportRect; // Camera's position
    static float zoom; // Camera's zoom level (1.0f is normal)

};

#endif
