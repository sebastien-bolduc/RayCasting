/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 2.00
 * @since 2012-02-20
 * 
 * gcc raycasting.c `sdl-config --cflags --libs` -lSDL_gfx -lSDL_ttf -o raycasting
 */
 
#include <math.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_ttf.h"

#include "../MyOwnFramework/mof/mof_font.h"

#include "RCA/bsptree.h"
#include "RCA/element.h"
#include "RCA/keyboard.h"
#include "RCA/raycaster.h"
#include "RCA/sector.h"

SDL_Surface *screen;
SDL_Event event;

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const char *WINDOW_TITLE = "RayCasting";
const char *WINDOW_FONT = "/home/user/Downloads/arial.ttf";

mof_Font *text = NULL;
char test[100] = {"/0"};
int mapflag = 0;
int release_m = 1;

Element *player;
Sector *sector_1;
Sector *sector_2;
Sector *sector_3;
Sector *sector_4;
Sector *sector_5;
Sector *sector_6;
Sector *sector_7;
Sector *sector_8;
Sector *sector_9;
Sector *sector_10;
Sector *sector_11;
BSPtree *bsptree;

/**
 * Initialization.
 */
void RCA_Init()
{
  /* video */
  SDL_Init(SDL_INIT_VIDEO);
  
  /* window */
  screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
  SDL_WM_SetCaption(WINDOW_TITLE, 0);

  /* keyboard */
  //SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
  
  /* TODO: add your code here */
  text = mof_Font__new(screen, WINDOW_FONT);
  
  bsptree = RCA_NewBSPtree(640, 0, 640, 719);
  player = RCA_NewElement(640, 310, 270);
  sector_1 = RCA_NewSector();
  sector_2 = RCA_NewSector();
  sector_3 = RCA_NewSector();
  sector_4 = RCA_NewSector();
  sector_5 = RCA_NewSector();
  sector_6 = RCA_NewSector();
  sector_7 = RCA_NewSector();
  sector_8 = RCA_NewSector();
  sector_9 = RCA_NewSector();
  sector_10 = RCA_NewSector();
  sector_11 = RCA_NewSector();
}

/**
 * Loading.
 */
void RCA_Load()
{
  /* TODO: add your code here */
  int invisible[4] = {255, 255, 255, 0};
  int white[4] = {255, 255, 255, 255};
  int light_grey[4] = {125, 125, 125, 255};
  int grey[4] = {100, 100, 100, 255};
  int light_green[4] = {0, 255, 0, 255};
  int dark_green[4] = {0, 100, 0, 255};
  int light_yellow[4] = {255, 255, 0, 255};
  int dark_yellow[4] = {100, 100, 0, 255};
  int light_red[4] = {200, 0, 0, 255};
  int dark_red[4] = {150, 0, 0, 255};
  
  RCA_AddWallToSector(sector_1, 600, 225, 600, 275, 0, 0, 0, 0, invisible, invisible, invisible);
  RCA_AddWallToSector(sector_1, 300, 200, 600, 200, 0, 0, 0, 0, invisible, white, invisible);
  RCA_AddWallToSector(sector_1, 300, 200, 300, 275, 0, 0, 0, 0, invisible, grey, invisible);
  RCA_AddWallToSector(sector_1, 500, 275, 600, 275, 0, 0, 0, 0, invisible, invisible, invisible);
  
  RCA_AddWallToSector(sector_2, 600, 200, 640, 200, 0, 0, 0, 0, invisible, white, invisible);
  RCA_AddWallToSector(sector_2, 600, 225, 640, 225, 0, 0, 0, 0, invisible, invisible, invisible);
  
  RCA_AddWallToSector(sector_3, 600, 225, 640, 225, -15, 15, 0, 0, light_green, light_yellow, light_green);
  RCA_AddWallToSector(sector_3, 600, 275, 640, 275, -15, 15, 0, 0, light_green, invisible, light_green);
  RCA_AddWallToSector(sector_3, 600, 225, 600, 275, -15, 15, 0, 0, dark_green, dark_yellow, dark_green);
  RCA_AddWallToSector(sector_3, 640, 225, 640, 275, -15, 15, 0, 0, dark_green, invisible, dark_green);
  
  RCA_AddWallToSector(sector_4, 600, 275, 640, 275, 0, 0, 0, 0, invisible, invisible, invisible);
  RCA_AddWallToSector(sector_4, 600, 400, 640, 400, 0, 0, 0, 0, invisible, white, invisible);
  RCA_AddWallToSector(sector_4, 600, 275, 600, 300, 0, 0, 0, 0, invisible, invisible, invisible);
  
  RCA_AddWallToSector(sector_5, 790, 200, 800, 200, 0, 0, 0, 0, invisible, white, invisible);
  RCA_AddWallToSector(sector_5, 800, 200, 800, 300, 0, 0, 0, 0, invisible, grey, invisible);
  RCA_AddWallToSector(sector_5, 790, 320, 800, 300, 0, 0, 0, 0, invisible, invisible, invisible);
  
  RCA_AddWallToSector(sector_6, 640, 200, 790, 200, 0, 0, 0, 0, invisible, white, invisible);
  RCA_AddWallToSector(sector_6, 640, 400, 790, 400, 0, 0, 0, 0, invisible, white, invisible);
  RCA_AddWallToSector(sector_6, 790, 320, 790, 360, 0, 0, 0, 0, invisible, invisible, invisible);
  RCA_AddWallToSector(sector_6, 640, 225, 640, 275, 0, 0, 0, 0, invisible, invisible, invisible);
  
  RCA_AddWallToSector(sector_7, 790, 320, 790, 360, 20, 20, 0, 0, grey, invisible, grey);
  RCA_AddWallToSector(sector_7, 790, 320, 800, 300, 20, 20, 0, 0, light_grey, light_grey, light_grey);
  RCA_AddWallToSector(sector_7, 790, 360, 800, 380, 20, 20, 0, 0, light_grey, light_grey, light_grey);
  RCA_AddWallToSector(sector_7, 800, 300, 800, 380, 20, 20, 0, 0, grey, invisible, grey);
  
  RCA_AddWallToSector(sector_8, 790, 360, 800, 380, 0, 0, 0, 0, invisible, invisible, invisible);
  RCA_AddWallToSector(sector_8, 800, 380, 800, 400, 0, 0, 0, 0, invisible, grey, invisible);
  RCA_AddWallToSector(sector_8, 790, 400, 800, 400, 0, 0, 0, 0, invisible, white, invisible);
  
  RCA_AddWallToSector(sector_9, 500, 275, 500, 300, 0, 0, 0, 0, invisible, invisible, invisible);
  RCA_AddWallToSector(sector_9, 300, 275, 300, 300, 0, 0, 0, 0, invisible, grey, invisible);
  
  RCA_AddWallToSector(sector_10, 500, 275, 600, 275, 30, 45, 15, 5, light_red, invisible, light_red);
  RCA_AddWallToSector(sector_10, 500, 300, 600, 300, 30, 45, 15, 5, light_red, invisible, light_red);
  RCA_AddWallToSector(sector_10, 500, 275, 500, 300, 15, 5, 0, 0, dark_red, dark_yellow, dark_red);
  RCA_AddWallToSector(sector_10, 600, 275, 600, 300, 45, 50, 0, 0, dark_red, invisible, dark_red);
  
  RCA_AddWallToSector(sector_11, 500, 300, 600, 300, 0, 0, 0, 0, invisible, invisible, invisible);
  RCA_AddWallToSector(sector_11, 300, 300, 300, 400, 0, 0, 0, 0, invisible, grey, invisible);
  RCA_AddWallToSector(sector_11, 300, 400, 600, 400, 0, 0, 0, 0, invisible, white, invisible);
  
  /* BSP tree */
  RCA_AddNodeToBSPtreeFront(bsptree, 790, 0, 790, 719);
    RCA_AddLeafToBSPtreeBack(bsptree->front, sector_6);
	RCA_AddNodeToBSPtreeFront(bsptree->front, 790, 320, 800, 300);
	  RCA_AddLeafToBSPtreeBack(bsptree->front->front, sector_5);
	  RCA_AddNodeToBSPtreeFront(bsptree->front->front, 790, 360, 800, 380);
	    RCA_AddLeafToBSPtreeFront(bsptree->front->front->front, sector_8);
		RCA_AddLeafToBSPtreeBack(bsptree->front->front->front, sector_7);
  RCA_AddNodeToBSPtreeBack(bsptree, 600, 0, 600, 719);
    RCA_AddNodeToBSPtreeBack(bsptree->back, 0, 275, 1279, 275);
      RCA_AddLeafToBSPtreeBack(bsptree->back->back, sector_1);
	  RCA_AddNodeToBSPtreeFront(bsptree->back->back, 0, 300, 1279, 300);
	    RCA_AddLeafToBSPtreeFront(bsptree->back->back->front, sector_11);
		RCA_AddNodeToBSPtreeBack(bsptree->back->back->front, 500, 0, 500, 719);
		  RCA_AddLeafToBSPtreeBack(bsptree->back->back->front->back, sector_9);
		  RCA_AddLeafToBSPtreeFront(bsptree->back->back->front->back, sector_10);
	RCA_AddNodeToBSPtreeFront(bsptree->back, 0, 275, 1279, 275);
	  RCA_AddLeafToBSPtreeFront(bsptree->back->front, sector_4);
	  RCA_AddNodeToBSPtreeBack(bsptree->back->front, 0, 225, 1279, 225);
	    RCA_AddLeafToBSPtreeFront(bsptree->back->front->back, sector_3);
	    RCA_AddLeafToBSPtreeBack(bsptree->back->front->back, sector_2);
}

/**
 * Unloading.
 */
void RCA_Unload()
{
  /* TODO: add your code here */
}

/**
 * Updating.
 * 
 * @param running_loop Determine if the loop still have to be executed.
 */
void RCA_Update(int *running_loop)
{
  /* TODO: add your code here */	
  while (SDL_PollEvent(&event))			/* every event must be poll from the queue... */
  {
	/* handling the SDL window */
	if (event.type == SDL_QUIT)
	{
	  *running_loop = 0;
	}
	if (event.type == SDL_VIDEORESIZE)
	{
	  screen = SDL_SetVideoMode(event.resize.w, event.resize.h, 0, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE);
	}
	
	/* handling the mouse */
	if (event.type == SDL_MOUSEMOTION)
	{
	  if (event.motion.xrel > 0)
	  {
		RCA_RotateElement(player, 0.5);
	  }
	  if (event.motion.xrel < 0)
	  {
		RCA_RotateElement(player, -0.5);
	  }
	}
	
	/* handling the keyboard (game-type input) */
	RCA_PollKeyboardEvent(&event);
  }
  
  /* taking care of the keyboard (game-type input) */
  if (RCA_CheckKeyboardKey(SDLK_LEFT))
  {
	RCA_MoveElementLeft(player, 1);
  }
  if (RCA_CheckKeyboardKey(SDLK_RIGHT))
  {
	RCA_MoveElementRight(player, 1);
  }
  if (RCA_CheckKeyboardKey(SDLK_UP))
  {
	RCA_MoveElementForward(player, 1);
  }
  if (RCA_CheckKeyboardKey(SDLK_DOWN))
  {
	RCA_MoveElementBackward(player, 1);
  }
  
  /* toggling map */
  if (RCA_CheckKeyboardKey(SDLK_m))
  {
	if (release_m)
	{
	  mapflag = (mapflag) ? 0 : 1;
	  release_m = 0;
	}
  }
  else 
  {
	release_m = 1;
  }
}

/**
 * Drawing.
 */
void RCA_Draw()
{	
  /* clear the screen */
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  
  /* TODO: add your code here */
  if (mapflag)
  {
    RCA_DrawSector(screen, sector_1);
	RCA_DrawSector(screen, sector_2);
	RCA_DrawSector(screen, sector_3);
	RCA_DrawSector(screen, sector_4);
	RCA_DrawSector(screen, sector_5);
	RCA_DrawSector(screen, sector_6);
	RCA_DrawSector(screen, sector_7);
	RCA_DrawSector(screen, sector_8);
	RCA_DrawSector(screen, sector_9);
	RCA_DrawSector(screen, sector_10);
	RCA_DrawSector(screen, sector_11);
    RCA_DrawElement(screen, player);
  }
  else 
  {
	RCA_TraverseBSPtree(screen, bsptree, player);
  }
}

/**
 * Main function of the application.
 * 
 * @param argc Arguments passed on the command line (number).
 * @param argv Arguments passed on the command line (values).
 * @return     0 (meaning we're done)
 */
int main(int argc, char **argv)
{
  RCA_Init();
  RCA_Load();
	
  int running_loop = 1;
  while(running_loop)
  {
    RCA_Update(&running_loop);
	   
	RCA_Draw();
	 
	SDL_Delay(1);

	SDL_Flip(screen);
  }

  /* Destroy our objects */
  mof_Font__destroy(text);
  
  RCA_DestroyBSPtree(bsptree);
  RCA_DestroyElement(player);
  RCA_DestroySector(sector_1);
  RCA_DestroySector(sector_2);
  RCA_DestroySector(sector_3);
  RCA_DestroySector(sector_4);
  RCA_DestroySector(sector_5);
  RCA_DestroySector(sector_6);
  RCA_DestroySector(sector_7);
  RCA_DestroySector(sector_8);
  RCA_DestroySector(sector_9);
  RCA_DestroySector(sector_10);
  RCA_DestroySector(sector_11);

  SDL_Quit();

  return 0;
}