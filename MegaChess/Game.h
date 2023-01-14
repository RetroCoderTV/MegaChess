#ifndef GAME_H
#define GAME_H


#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>

class ChessPiece;
class Sprite;

class Game
{

public:
	void run();
	void init();
	void update();
	void render();

	

private:
	//Chess Game Specifics:
	const std::string START_FEN="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hStdinRead,hStdinWrite,hStdoutRead,hStdoutWrite,hStderrRead,hStderrWrite;
	SECURITY_ATTRIBUTES saAttr;

	class Sprite* Chessboard_Sprite;
	std::vector<ChessPiece*> pieceSprites;
	std::vector<Sprite*> highlightSprites;

	int white_player=1; //0=CPU 1=HUMAN
	int black_player=0;
	char player_to_act=0; //0=White 1=Black
	const int CPU_PLAYER=0;
	const int HUMAN_PLAYER=1;

	void StartStockfish();
	void CloseStockfish();
	void SendStockfishCommand(std::string str);
	std::vector<std::string> ReadStockfishOutput(DWORD delayMillis);
	void GeneratePiecesFromFenString(std::string fenString);
	std::string currentFen="";
	void GenerateLegalHighlights(std::vector<std::string> legal_moves);

	void StartNewGame();
	void SetDifficultyLevel(int diff);
	std::string GetBestMove();
	float cpuThinkTime=1000.f;
	void PlayMove(std::string move);
	void GetFenFromStockfish();
	std::vector<std::string> MoveLog;
	float autoplayInterval=1.f /4;
	float autoplayTimer=0.f;
	void CheckMovesUntilMate();
	bool isCheckmate=false;
	bool isDraw=false;
	void StopStockfishAfterTime(DWORD millis);
	void CheckDrawConditions();
	std::vector<std::string> GetLegalMoves(std::string squareCode);


	ChessPiece* getMouseOverPiece();
	ChessPiece* clickedPiece;



};







#endif // !GAME_H