#ifndef SPRITE_H
#define SPRITE_H

#pragma once


#include <string>
#include "MegaEngine.h"

enum PhysicsBodyType
{
	STATIC_BODY,
	KINEMATIC_BODY,
	DYNAMIC_BODY

};

class Sprite
{
public:
	Sprite();
	~Sprite();
	virtual void init(std::string AssetString);
	virtual void init(std::string AssetString, int SrcX, int SrcY, int SrcW, int SrcH, float DstX, float DstY, float DstW, float DstH);
	virtual void update();
	virtual void render();

	SDL_Rect& getSrcRect();
	SDL_FRect& getDstRect();
	std::string& getAssetString();


	void activatePhysics(PhysicsBodyType bodyType, float Density);
	bool GetIsPhysicsObject();


private:
	SDL_Rect srcRect;
	SDL_FRect dstRect;
	std::string assetString;
	
	bool isPhysicsObject;
	b2Body* body;
	float density;
	float friction;
};


#endif
