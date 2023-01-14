#include "Game.h"

#include "MegaEngine.h"
#include <cstdio>

#include "Sprite.h"
#include "Assets.h"
#include "ChessPiece.h"

#include <sstream>

/*
TODO:
implement all draw conditons.
highligh last move on board
implement UI (Game options, Game Start, Game Restart, Show History)
Implement some ELO system
implement network gameplay
implement a web version

*/





void Game::run()
{
    MegaEngine::initGame(1920,1080);

    init();

    MegaEngine::runGameLoop(this);

    MegaEngine::closeWindow();
}

void Game::init()
{
    Chessboard_Sprite=new Sprite();
    Chessboard_Sprite->init(TEXTURE_CHESSBOARD);

    saAttr.nLength=sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle=TRUE;
    saAttr.lpSecurityDescriptor=NULL;

    StartStockfish();
    StartNewGame();

}

void Game::update()
{
    if(isCheckmate)
    {
        printf("\n*******\n*******\n*******\nCHECKMATE!\n*******\n*******\n*******\n");
        return;
    }

    if(isDraw)
    {
        printf("\n*******\n*******\n*******             DRAW!            *******\n*******\n*******\n");
        return;
    }

    if(player_to_act == 'w') // white turn
    {
        if(white_player == CPU_PLAYER)
        {
            //auto-play a turn
            if((autoplayTimer+=MegaEngine::getDeltaTime()) > autoplayInterval)
            {
                autoplayTimer=0.f;
                PlayMove(GetBestMove());
            }
        } 
        else // human player...
        {         
            if(MegaEngine::mouseButtonClicked1)
            {
                if(!clickedPiece) clickedPiece=getMouseOverPiece();

                if(clickedPiece)
                {
                    clickedPiece->PickupPiece();
                    std::vector<std::string> legal_squares=GetLegalMoves(clickedPiece->GetPiecePositionString().c_str());
                    GenerateLegalHighlights(legal_squares);
                }  
            }
            if(MegaEngine::mouseButtonHeld1)
            {
                if(clickedPiece)
                {
                    clickedPiece->FollowMouse(MegaEngine::mouseX,MegaEngine::mouseY);
                }   
            }
            else
            {
                if(clickedPiece)
                {
                    std::vector<std::string> legal_squares=GetLegalMoves(clickedPiece->GetPiecePositionString().c_str());
                    std::string moved_string=clickedPiece->DropPiece(legal_squares);
                    if(moved_string != "")
                    {
                        //todo... play the turn. The piece graphic has been moved already
                        std::string str=clickedPiece->GetPiecePositionString().c_str();
                        str+=moved_string;
                        printf("move finalised str=%s\n\n\n\n",str.c_str());
                        PlayMove(str);
                    }
                    
                    clickedPiece = nullptr;
                    highlightSprites.clear();
                    
                }
            }
        }
    }
    else if(player_to_act == 'b') // black turn
    {
        if(black_player == CPU_PLAYER)
        {
            //auto-play a turn
            if((autoplayTimer+=MegaEngine::getDeltaTime()) > autoplayInterval)
            {
                autoplayTimer=0.f;
                PlayMove(GetBestMove());
            }
        }
        else
        {
            if(MegaEngine::mouseButtonClicked1)
            {
                if(!clickedPiece) clickedPiece=getMouseOverPiece();

                if(clickedPiece)
                {
                    clickedPiece->PickupPiece();
                    std::vector<std::string> legal_squares=GetLegalMoves(clickedPiece->GetPiecePositionString().c_str());
                    GenerateLegalHighlights(legal_squares);
                }
            }
            if(MegaEngine::mouseButtonHeld1)
            {
                if(clickedPiece)
                {
                    clickedPiece->FollowMouse(MegaEngine::mouseX,MegaEngine::mouseY);
                }
            }
            else
            {
                if(clickedPiece)
                {
                    std::vector<std::string> legal_squares=GetLegalMoves(clickedPiece->GetPiecePositionString().c_str());
                    std::string moved_string=clickedPiece->DropPiece(legal_squares);
                    if(moved_string != "")
                    {
                        //todo... play the turn. The piece graphic has been moved already
                        std::string str=clickedPiece->GetPiecePositionString().c_str();
                        str+=moved_string;
                        printf("move finalised str=%s\n\n\n\n",str.c_str());
                        PlayMove(str);
                    }

                    clickedPiece=nullptr;
                    highlightSprites.clear();
                }
            }
        }        
    }
}

void Game::render()
{
    Chessboard_Sprite->render();

    for(int i=0; i < highlightSprites.size(); i++)
        highlightSprites[i]->render();
    
    for(int i=0; i < pieceSprites.size(); i++)
        pieceSprites[i]->render(); 
}

void Game::StartStockfish()
{
    // Start the Stockfish process

    ZeroMemory(&si,sizeof(si));
    si.cb=sizeof(si);
    ZeroMemory(&pi,sizeof(pi));

    // Create pipe for the child process's standard input
    if(!CreatePipe(&hStdinRead,&hStdinWrite,&saAttr,0))
    {
        printf("CreatePipe failed (%d).\n",GetLastError());
        return;
    }

    // Create pipe for the child process's standard output
    if(!CreatePipe(&hStdoutRead,&hStdoutWrite,&saAttr,0))
    {
        printf("CreatePipe failed (%d).\n",GetLastError());
        return;
    }

    // Create pipe for the child process's standard error
    if(!CreatePipe(&hStderrRead,&hStderrWrite,&saAttr,0))
    {
        printf("CreatePipe failed (%d).\n",GetLastError());
        return;
    }

    si.hStdError=GetStdHandle(STD_ERROR_HANDLE);
    si.hStdOutput=hStdoutWrite;
    si.hStdInput=hStdinRead;
    si.dwFlags|=STARTF_USESTDHANDLES;

    if(!CreateProcess(
        (LPCTSTR)L"D:/_Dev/stockfish_15.1_win_x64_avx2/stockfish-windows-2022-x86-64-avx2.exe",   // No module name (use command line)
        (LPTSTR)L"",                // Command line
        NULL,                   // Process handle not inheritable
        NULL,                   // Thread handle not inheritable
        TRUE,                  // Set handle inheritance to TRUE
        0,                      // No creation flags
        NULL,                   // Use parent's environment block
        NULL,                   // Use parent's starting directory
        &si,                    // Pointer to STARTUPINFO structure
        &pi)                    // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n",GetLastError());
    }
}

void Game::CloseStockfish()
{
}

void Game::SendStockfishCommand(std::string str)
{
    DWORD bytesWritten=0;
    str+='\n';
    if(WriteFile(hStdinWrite,str.c_str(),str.length(),&bytesWritten,NULL))
    {
        printf("Sent command:%s  . BytesWritten=%ld\n",str.c_str(),bytesWritten);
    }
}

std::vector<std::string> Game::ReadStockfishOutput(DWORD delayMillis)
{
    // need some form of PeekPipe check before ReadFile to stop it hanging when there was no response but we try to check one anyway.
    Sleep(delayMillis);
    printf("*************************Read called*********************\n");
    std::vector<std::string> lines;
    DWORD bytesRead=0;
    char buffer[1024 * 8];
    while(true)
    {
        // Check if the read operation was successful
        if(!ReadFile(hStdoutRead,buffer,sizeof(buffer),&bytesRead,NULL))
        {
            printf("FAIL bytes READ=%ld\n",bytesRead);
            DWORD errorCode=GetLastError();
            if(errorCode == ERROR_BROKEN_PIPE)
            {
                printf("Pipe was closed\n");
            }
            else
            {
                printf("Error occurred while reading from the pipe: %d\n",errorCode);
                // Additional error handling code
            }
            break;
        }

        std::stringstream bufferStream(std::string(buffer,bytesRead));
        std::string line;
        while(std::getline(bufferStream,line))
        {
            lines.push_back(line);
        }

        fflush(stdout);
        break;

    }
    return lines;
}

void Game::GeneratePiecesFromFenString(std::string fenString)
{
    printf("GeneratePiecesFromFenString() called. 'fenString'=%s\n",fenString.c_str());
    pieceSprites.clear(); //check this clear is sufficient. I probably need to delete pointers and null the references. 
    int rank=0;
    int file=0;
    SDL_Rect sRect={0,0,0,0};
    SDL_FRect dRect={0.f,0.f,0.f,0.f};
    std::vector<char> fenChars(fenString.begin(), fenString.end());
    for(int i=0; i < fenChars.size(); i++)
    {
        char c=fenChars[i];

        if(c == ' ')
        {
            player_to_act=fenChars[i + 1]; // the char after the first space is always there so this won't cause problems.
            break; 
        }
        if(c == '/')
        {
            rank++;
            file=0;
            continue;
        }
        if(c < 'A')
        {
            file+=(int)c - '0'; 
            continue;
        }

        ChessPiece* spr=new ChessPiece();
        spr->init(
            TEXTURE_PIECES,
            7-rank, 
            file++,
            c);
        pieceSprites.push_back(spr);
    }
}

void Game::GenerateLegalHighlights(std::vector<std::string> legal_moves)
{
    highlightSprites.clear();

    for(int i=0; i < legal_moves.size(); i++)
    {
        float x=((int)(legal_moves[i][0] - 'a') * (float)Assets::CELL_SIZE) + Assets::CELL_SIZE / 4;
        float y=((7-((int)(legal_moves[i][1] - '0')-1)) *(float)Assets::CELL_SIZE) + Assets::CELL_SIZE/4;
        Sprite* spr=new Sprite();
        spr->init(TEXTURE_CHESSBOARD_HIGHLIGHT);
        spr->getDstRect()={x,y,Assets::CELL_SIZE / 2, Assets::CELL_SIZE / 2};
        highlightSprites.push_back(spr);
    }
}

void Game::StartNewGame()
{
    // Send a command to Stockfish
    SendStockfishCommand("uci");
    std::vector<std::string> uci_response=ReadStockfishOutput(20);
    for(int i=0; i < uci_response.size(); i++)
    {
        printf("uci_response %d=%s\n",i,uci_response[i].c_str());
    }

    SetDifficultyLevel(20); // set diff 0-20

    SendStockfishCommand("isready");
    std::vector<std::string> isready_response=ReadStockfishOutput(2);
    for(int i=0; i < isready_response.size(); i++)
    {
        printf("isready_response %d=%s\n",i,isready_response[i].c_str());
    }

    

    currentFen=START_FEN;
    GeneratePiecesFromFenString(START_FEN); // TODO: Could turn this into a parameter of this function, so we can start game from any position.
}

void Game::SetDifficultyLevel(int diff)
{
    std::string cmd="setoption name Skill Level value " + std::to_string(diff);
    SendStockfishCommand(cmd);
    
}

std::string Game::GetBestMove()
{
    std::string best;

    std::string cmd="position fen ";
    cmd+=currentFen;
    SendStockfishCommand(cmd);
    SendStockfishCommand("go depth 10");
    std::vector<std::string> position_startpos_response=ReadStockfishOutput((DWORD)cpuThinkTime);
    for(int i=0; i < position_startpos_response.size(); i++)
    {
        printf("go-depth output: %d=%s\n",i,position_startpos_response[i].c_str());
        if(position_startpos_response[i].compare(0,9,"bestmove ") == 0)
        {
            best=position_startpos_response[i].substr(9);
            std::size_t found=best.find(" ");
            best=best.substr(0,found);
        }
    }
    return best;
}

void Game::PlayMove(std::string move)
{
    std::string cmd="position fen ";
    cmd+=currentFen;
    cmd+=" moves ";
    cmd+=move;
    SendStockfishCommand(cmd);

    GetFenFromStockfish();

    GeneratePiecesFromFenString(currentFen); 
}

void Game::GetFenFromStockfish()
{
    SendStockfishCommand("d");
    std::vector<std::string> response=ReadStockfishOutput(100);
    for(int i=0; i < response.size(); i++)
    {
        printf("response %d=%s\n",i,response[i].c_str());
        //cache fen string, for use in rendering pieces
        if(response[i].compare(0,5,"Fen: ") == 0)
        {  
            std::string fenStr=currentFen;
            size_t spaceIndex=fenStr.find(" ");
            if(spaceIndex != std::string::npos)
            {
                fenStr=fenStr.substr(0,spaceIndex);
            }
            MoveLog.push_back(fenStr);
            currentFen=response[i].substr(5);
        }
    }

    CheckMovesUntilMate();
    CheckDrawConditions();
}

void Game::CheckMovesUntilMate()
{
    SendStockfishCommand("isready");
    std::vector<std::string> isready_response=ReadStockfishOutput(100);
    for(int i=0; i < isready_response.size(); i++)
    {
        printf("isready_response %d=%s\n",i,isready_response[i].c_str());
    }

    SendStockfishCommand("go mate movetime=100");
    StopStockfishAfterTime(105);
    std::vector<std::string> response=ReadStockfishOutput(200);
    for(int i=0; i < response.size(); i++)
    {
        printf("movesuntilmatecheck_response %d=%s\n",i,response[i].c_str());
        if(response[i].find("mate 0") != std::string::npos)
        {
            isCheckmate=true;
        }

    }

}

void Game::StopStockfishAfterTime(DWORD millis)
{
    Sleep(millis);
    SendStockfishCommand("stop");
}

void Game::CheckDrawConditions()
{
    // check repetition...
    for(int i=0; i < MoveLog.size(); i++)  //TODO: come up with a better way.
    {
        for(int j=i + 1; j < MoveLog.size(); j++)
        {
            for(int k=j + 1; k < MoveLog.size(); k++)
            {
                if(MoveLog[i] == MoveLog[j] && MoveLog[j] == MoveLog[k])
                {
                    printf("DRAW BY REPETITION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! %d, %d, and %d\n",i,j,k);
                }
            }
        }
    }


    // check 50 rule... (50 rule is checked in the GetFenFromStockfish function
    
    // check stalemate...
}

std::vector<std::string> Game::GetLegalMoves(std::string squareCode)
{
    std::string cmd="go searchmoves ";
    cmd+=squareCode;
    std::vector<std::string> legals;
    
    for(int f=0; f < 8; f++)
    {
        for(int r=0; r < 8; r++)
        {
            std::string destinationStr="";
            destinationStr+=(char)(f + 'a');
            destinationStr+=(char)(r + '0' + 1);
            SendStockfishCommand(cmd + destinationStr);
            SendStockfishCommand("stop");

            std::vector<std::string> response=ReadStockfishOutput(0);
            for(int i=0; i < response.size(); i++)
            {
                printf("legal_response %d=%s\n",i,response[i].c_str());
                if((response[i].find("bestmove")) != std::string::npos)
                {
                    if((response[i].find("(n")) == std::string::npos)
                    {
                        std::string str=response[i].substr(11,2); //hardcoded location of destination square
                        legals.push_back(str);
                        printf("LEGAL MOVE ADDED TO LIST\n\n");
                    }
                }          
            }
        }
    }

    //DEBUG INFO...
    printf("legals size=%d\n",(int)legals.size());
    for(int i=0; i < legals.size(); i++)
    {
        printf("LEGALLINE:%s\n",legals[i].c_str());
    }
    // DEBUG END

    return legals;
}

ChessPiece* Game::getMouseOverPiece()
{
    for(ChessPiece* piece : pieceSprites)
    {
        SDL_FRect pieceRect=piece->getDstRect();
        if(MegaEngine::mouseX >= pieceRect.x && MegaEngine::mouseX <= pieceRect.x + pieceRect.w &&
            MegaEngine::mouseY >= pieceRect.y && MegaEngine::mouseY <= pieceRect.y + pieceRect.h)
        {
            return piece;
        }
    }
    return nullptr;
}
