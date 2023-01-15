#include "Assets.h"


std::map<std::string,SDL_Texture*> Assets::sTextureCache;
Uint32 Assets::board_colors[2]={0x008B4411, 0xc7ad7f};

bool Assets::Init(SDL_Renderer* renderer)
{
    // Initialize SDL_image
    int img_flags=IMG_INIT_PNG;
    if(!(IMG_Init(img_flags) & img_flags))
    {
        // Error initializing SDL_image
        printf("Error initializing SDL_image");
        return false;
    }

    // Build the full path to the resources folder
    std::string resources_path="C:/_MyDev/MegaChess/MegaChess/Resources/";

    // Load the textures and add them to the cache
    std::string fullpath="";
    fullpath=resources_path + "chess_pieces.png";
    SDL_Surface* surface=IMG_Load(fullpath.c_str());
    sTextureCache[TEXTURE_PIECES]=SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    //fullpath=resources_path + "yoshi.png";
    //surface=IMG_Load(fullpath.c_str());
    //sTextureCache[TEXTURE_YOSHI]=SDL_CreateTextureFromSurface(renderer,surface);
    
    surface=SDL_CreateRGBSurface(0,8 * CELL_SIZE,8 * CELL_SIZE,32,0,0,0,0);
    if(surface == NULL)
    {
        printf("Error %s\n",SDL_GetError());
    }

    for(int i=0; i < 8; i++)
    {
        for(int j=0; j < 8; j++)
        {
            int x=j * CELL_SIZE;
            int y=i * CELL_SIZE;
            SDL_Rect rect={x, y, CELL_SIZE, CELL_SIZE};
            SDL_FillRect(surface,&rect,board_colors[(i + j) % 2]);
        }
    }
    sTextureCache[TEXTURE_CHESSBOARD]=SDL_CreateTextureFromSurface(renderer,surface);

    surface=SDL_CreateRGBSurface(0,1,1,32,0,0,0,0);
    if(surface == NULL)
    {
        printf("Error %s\n",SDL_GetError());
    }

    int x=0;
    int y=0;
    SDL_Rect rect={x, y, 1, 1};
    SDL_FillRect(surface,&rect,0xFFa0a0a0);
    sTextureCache[TEXTURE_CHESSBOARD_HIGHLIGHT]=SDL_CreateTextureFromSurface(renderer,surface);

    SDL_FreeSurface(surface);
    surface=nullptr;

    return true;
}

void Assets::Quit()
{
    // Destroy the textures and quit SDL_image
    for(auto& kv : sTextureCache)
    {
        SDL_DestroyTexture(kv.second);
    }
    sTextureCache.clear();
    IMG_Quit();
}

SDL_Texture* Assets::GetTexture(const char* file)
{
    // Look up the texture in the cache
    auto it=sTextureCache.find(file);
    if(it != sTextureCache.end())
    {
        return it->second;
    }

    printf("Error in GetTexture(): File=%s not found \n",file);
    return nullptr;
}

