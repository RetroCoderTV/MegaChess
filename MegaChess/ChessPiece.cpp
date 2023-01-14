#include "ChessPiece.h"

#include "Assets.h"
#include "Camera.h"

void ChessPiece::init(std::string AssetString, int rank, int file, char piece)
{
	this->rank=rank;
	this->file=file;
	getAssetString()=AssetString;

	if(piece == 'K')
	{
		getSrcRect()={0,0,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'Q')
	{
		getSrcRect()={1* source_texture_piece_width,0,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'B')
	{
		getSrcRect()={2 * source_texture_piece_width,0,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'N')
	{
		getSrcRect()={3 * source_texture_piece_width,0,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'R')
	{
		getSrcRect()={4 * source_texture_piece_width,0,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'P')
	{
		getSrcRect()={5 * source_texture_piece_width,0,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'k')
	{
		getSrcRect()={0,source_texture_piece_width,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'q')
	{
		getSrcRect()={1 * source_texture_piece_width,source_texture_piece_width,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'b')
	{
		getSrcRect()={2 * source_texture_piece_width,source_texture_piece_width,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'n')
	{
		getSrcRect()={3 * source_texture_piece_width,source_texture_piece_width,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'r')
	{
		getSrcRect()={4 * source_texture_piece_width,source_texture_piece_width,source_texture_piece_width,source_texture_piece_width};
	}
	else if(piece == 'p')
	{
		getSrcRect()={5 * source_texture_piece_width,source_texture_piece_width,source_texture_piece_width,source_texture_piece_width};
	}



	// TOGGLE WHITE OR BLACK AT TOP:
	//getDstRect()={(float)file * Assets::CELL_SIZE, (float)rank * Assets::CELL_SIZE,Assets::CELL_SIZE, Assets::CELL_SIZE};
	getDstRect()={(float)file * Assets::CELL_SIZE, (float)(8 - rank - 1) * Assets::CELL_SIZE,Assets::CELL_SIZE, Assets::CELL_SIZE};

}

int ChessPiece::GetRank()
{
	return rank;
}

int ChessPiece::GetFileInt()
{
	return file;
}

std::string ChessPiece::GetPiecePositionString()
{
	std::string str="";
	str+=(file + 'a');
	str+=(rank + '0' + 1);
	return str;
}

void ChessPiece::render()
{
	//SDL_BlendMode previousBlendMode;
	//SDL_GetRenderDrawBlendMode(MegaEngine::renderer,&previousBlendMode);
	//SDL_SetRenderDrawBlendMode(MegaEngine::renderer,SDL_BLENDMODE_BLEND);

	//Uint8 r,g,b;
	//SDL_GetTextureColorMod(Assets::GetTexture(getAssetString().c_str()),&r,&g,&b);
	//SDL_SetTextureColorMod(Assets::GetTexture(getAssetString().c_str()),0,255,0);

	Sprite::render();

	//SDL_SetRenderDrawBlendMode(MegaEngine::renderer,previousBlendMode);
	//SDL_SetTextureColorMod(Assets::GetTexture(getAssetString().c_str()),r,g,b);
}

void ChessPiece::PickupPiece()
{
	pickedup_x=getDstRect().x;
	pickedup_y=getDstRect().y;
	printf("Picked up piece x,y: %f, %f\n\n\n\n\n",pickedup_x,pickedup_y);
}

void ChessPiece::FollowMouse(Uint32 mouseX,Uint32 mouseY)
{
	getDstRect().x=(float)mouseX - (Assets::CELL_SIZE * 0.5f);
	getDstRect().y=(float)mouseY - (Assets::CELL_SIZE * 0.5f);
}

std::string ChessPiece::DropPiece(std::vector<std::string> legal_squares)
{
	printf("legalsquares size=%d\n\n\n",(int)legal_squares.size());
	for(int i=0; i < legal_squares.size(); i++)
	{
		float f=((int)(legal_squares[i][0] - 'a')) * Assets::CELL_SIZE;
		float r= (7 - ((int)(legal_squares[i][1] - '0' - 1))) * Assets::CELL_SIZE;
		printf("F and R=%f,%f\n",f,r);

		SDL_FPoint point={getDstRect().x + Assets::CELL_SIZE/2,getDstRect().y + Assets::CELL_SIZE/2 };
		SDL_FRect square={f,r,Assets::CELL_SIZE,Assets::CELL_SIZE};
		printf("point %f,%f  .... square %f,%f,%f,%f\n\n\n\n",point.x,point.y,square.x,square.y,square.w,square.h);
		if (SDL_PointInFRect(&point,&square))
		{
			printf("COLLISION! PLACED ONTO SQUARE\n");
			getDstRect().x=f;
			getDstRect().y=r;
			return legal_squares[i];
		}
	}

	printf("Dropped piece x,y: %f, %f\n\n\n\n\n",pickedup_x,pickedup_y);

	getDstRect().x=pickedup_x;
	getDstRect().y=pickedup_y;

	pickedup_x=0.f;
	pickedup_y=0.f;

	return "";
}
