#pragma once

#include "mdImageManipulationPrerequisites.h"

// #define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

#ifdef CreateWindow
# undef CreateWindow
#endif // CreateWindow


namespace MD {

using Window = SDL_Window;
using MSGBoxData = SDL_MessageBoxData;
using WindowRenderer = SDL_Renderer;

Window*
CreateWindow(String str, uint32_t w, uint32_t h, int32_t flags) {
  return SDL_CreateWindow(str.c_str(), w, h, flags);
}

SDL_Window* g_window;
SDL_Renderer* g_renderer;


SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    SDL_APP_FAILURE;
  }

  bool result = SDL_CreateWindowAndRenderer("SDL3 Window", 
                                            800, 
                                            600, 
                                            SDL_WINDOW_OPENGL, 
                                            &g_window, 
                                            &g_renderer);
  if (!result) {
    std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return SDL_APP_FAILURE;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
  return SDL_APP_CONTINUE;
}

}
