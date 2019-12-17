#include <SDL2/SDL.h>
#include <string>

namespace lycan {
namespace ext {

class SDLRender {
public:
    enum {
        FMT_RGBA32,
        FMT_BGRA32,
        FMT_ARGB32,
        FMT_ABGR32,
        FMT_RGB24,
        FMT_BGR24,
        FMT_GRAY,
        FMT_YUV420
    };

    static void Init() {
        SDL_Init(SDL_INIT_EVERYTHING);
    }

    SDLRender(std::string _name) 
        : name(_name)
        , window(NULL)
        , surface(NULL)
        , renderer(NULL) {

    }
    int ShowImage(void* pData, int width, int height, int pitch, int format) {
        if(window == NULL) {
            window = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              width, height, SDL_WINDOW_OPENGL);
            renderer = SDL_CreateRenderer(window, -1, 0);
            Uint32 fmt = SDL_PIXELFORMAT_RGBA8888;

            switch(format) {
            case FMT_RGBA32:
                fmt = SDL_PIXELFORMAT_RGBA8888;
                break;
            case FMT_BGRA32:
                fmt = SDL_PIXELFORMAT_BGRA32;
                break;
            case FMT_ARGB32:
                fmt = SDL_PIXELFORMAT_ARGB32;
                break;
            case FMT_ABGR32: 
                fmt = SDL_PIXELFORMAT_ABGR32;
                break;
            case FMT_RGB24: 
                fmt = SDL_PIXELFORMAT_RGB24;
                break;
            case FMT_BGR24: 
                fmt = SDL_PIXELFORMAT_BGR24;
                break;
            case FMT_GRAY:
                fmt = SDL_PIXELFORMAT_INDEX8;
                break;
            case FMT_YUV420:
                fmt = SDL_PIXELFORMAT_IYUV;
                break;
            default: 
                fmt = SDL_PIXELFORMAT_RGBA8888;
            };
            texture = SDL_CreateTexture(renderer, fmt, SDL_TEXTUREACCESS_STREAMING, width, height);
        }

        SDL_UpdateTexture(texture, NULL, pData, pitch);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        return 0;
    }

    int Wait() {
        return SDL_WaitEvent(&event);
    }

    bool LoopEnd() {
        return event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE;
    }
private:
    std::string     name;
    SDL_Window*     window;
    SDL_Surface*    surface;
    SDL_Renderer*   renderer;
    SDL_Texture*    texture;
    SDL_Event       event;
    int             width;
    int             height;
};

}}