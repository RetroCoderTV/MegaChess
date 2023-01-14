#ifndef ASSETS_H
#define ASSETS_H

#pragma once

#include <SDL.h>
#include <map>
#include <string>
#include <SDL_image.h>

#define TEXTURE_PIECES "Pieces"
#define TEXTURE_CHESSBOARD "ChessBoard"
#define TEXTURE_CHESSBOARD_HIGHLIGHT "ChessBoardHighlight"


class Assets
{
public:
	static bool Init(SDL_Renderer* renderer);
	static void Quit();

	static SDL_Texture* GetTexture(const char* file);

	static const int CELL_SIZE=64; //Specific to Chess Game

private:
	static std::map<std::string,SDL_Texture*> sTextureCache;

	static Uint32 board_colors[2];


};


#endif