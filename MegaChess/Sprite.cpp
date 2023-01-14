#include "Sprite.h"

#include <SDL.h>
#include "Assets.h"


Sprite::Sprite()
{
	srcRect={0,0,0,0};
	dstRect={0.f,0.f,0.f,0.f};
	assetString="";
	isPhysicsObject=false;
	body=nullptr;
	density=0.f;
	friction=0.f;
}
Sprite::~Sprite()
{

}

// a standard non-animated sprite, that uses the entire texture.
void Sprite::init(std::string AssetString)
{
	assetString=AssetString;

	int w=0;
	int h=0;
	SDL_QueryTexture(Assets::GetTexture(assetString.c_str()),nullptr,nullptr,&w,&h);
	srcRect={0,0,w,h};
	dstRect={0,0,(float)w,(float)h};
}

// a single frame sprite, from a larger spritesheet
void Sprite::init(std::string AssetString,int SrcX,int SrcY,int SrcW,int SrcH,float DstX,float DstY,float DstW,float DstH)
{
	assetString=AssetString;
	srcRect={SrcX,SrcY,SrcW,SrcH};
	dstRect={DstX,DstY,DstW,DstH};
}

void Sprite::update()
{
	if(isPhysicsObject)
	{
		dstRect.x=body->GetPosition().x;
		dstRect.y=body->GetPosition().y;
	}
	
}

void Sprite::render()
{
	SDL_RenderCopyF(MegaEngine::renderer,Assets::GetTexture(assetString.c_str()),&srcRect,&dstRect);
}

SDL_Rect& Sprite::getSrcRect()
{
	return srcRect;
}

SDL_FRect& Sprite::getDstRect()
{
	return dstRect;
}

std::string& Sprite::getAssetString()
{
	return assetString;
}

void Sprite::activatePhysics(PhysicsBodyType bodyType, float Density)
{
	if(!MegaEngine::isPhysicsEnabled)
	{
		printf("activatePhysics failed. 'Physics Game Not Enabled");
		return;
	}
	isPhysicsObject=true;

	density=Density;

	if(bodyType == STATIC_BODY)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(dstRect.x,dstRect.y);
		body=MegaEngine::world->CreateBody(&bodyDef);
		b2PolygonShape box;
		box.SetAsBox(dstRect.w,dstRect.y);

		body->CreateFixture(&box,0.f);
	}
	else if(bodyType == KINEMATIC_BODY)
	{

	}
	else if(bodyType == DYNAMIC_BODY)
	{
		b2BodyDef bodyDef;
		bodyDef.position.Set(dstRect.x,dstRect.y);
		bodyDef.type=b2_dynamicBody;
		body=MegaEngine::world->CreateBody(&bodyDef);
		b2PolygonShape box;
		box.SetAsBox(dstRect.w,dstRect.y);
		b2FixtureDef fixtureDef;
		fixtureDef.shape=&box;
		fixtureDef.density=density;
		fixtureDef.friction=friction;
		body->CreateFixture(&fixtureDef);
	}
	

	switch(bodyType)
	{
	case STATIC_BODY:
		
		break;
	case KINEMATIC_BODY:
		
		break;
	case DYNAMIC_BODY:

		break;
	}
}

bool Sprite::GetIsPhysicsObject()
{
	return isPhysicsObject;
}
