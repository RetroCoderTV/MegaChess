#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#pragma once

#include "Sprite.h"
#include <string>
#include <vector>

class ChessPiece : public Sprite
{

public:
	void init(std::string AssetString,int rank,int file,char piece);
	int GetRank();
	int GetFileInt();
	std::string GetPiecePositionString();

	void render() override;

	void PickupPiece();
	void FollowMouse(Uint32 mouseX, Uint32 mouseY);
	std::string DropPiece(std::vector<std::string> legal_squares);


private:
	const int source_texture_piece_width=256; // maybe better to calculate this earlier in program. In case I switch the texture for another one
	int rank=0;
	int file=0;

	float pickedup_x=0.f;
	float pickedup_y=0.f;

};




#endif // !CHESSPIECE_H
