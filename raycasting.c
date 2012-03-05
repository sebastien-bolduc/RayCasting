/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 0.01
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
Sector *sector_12;
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
  sector_12 = RCA_NewSector();
}

/**
 * Loading.
 */
void RCA_Load()
{
  /* TODO: add your code here */
  RCA_AddWallToSector(sector_1, 300, 200, 600, 200, 0, 255, 255, 255, 255);
  RCA_AddWallToSector(sector_1, 300, 200, 300, 225, 0, 100, 100, 100, 255);
  
  RCA_AddWallToSector(sector_2, 600, 200, 640, 200, 0, 255, 255, 255, 255);
  
  RCA_AddWallToSector(sector_3, 600, 225, 640, 225, -55, 0, 255, 0, 255);
  
  RCA_AddWallToSector(sector_4, 300, 225, 300, 275, 0, 100, 100, 100, 255);
  
  RCA_AddWallToSector(sector_5, 600, 225, 600, 275, -55, 0, 100, 0, 255);
  
  RCA_AddWallToSector(sector_6, 300, 275, 300, 400, 0, 100, 100, 100, 255);
  RCA_AddWallToSector(sector_6, 300, 400, 600, 400, 0, 255, 255, 255, 255);
  
  RCA_AddWallToSector(sector_7, 600, 275, 640, 275, -55, 0, 255, 0, 255);
  
  RCA_AddWallToSector(sector_8, 600, 400, 640, 400, 0, 255, 255, 255, 255);
  
  RCA_AddWallToSector(sector_9, 640, 200, 800, 200, 0, 255, 255, 255, 255);
  RCA_AddWallToSector(sector_9, 800, 200, 800, 225, 0, 100, 100, 100, 255);
  
  RCA_AddWallToSector(sector_10, 640, 225, 640, 275, -55, 0, 100, 0, 255);
  
  RCA_AddWallToSector(sector_11, 800, 225, 800, 275, 0, 100, 100, 100, 255);
  
  RCA_AddWallToSector(sector_12, 800, 275, 800, 400, 0, 100, 100, 100, 255);
  RCA_AddWallToSector(sector_12, 640, 400, 800, 400, 0, 255, 255, 255, 255);
  
  /* BSP tree */
  RCA_AddNodeToBSPtreeFront(bsptree, 0, 275, 1219, 275);
    RCA_AddLeafToBSPtreeFront(bsptree->front, sector_12);
    RCA_AddNodeToBSPtreeBack(bsptree->front, 0, 225, 1219, 225);
      RCA_AddLeafToBSPtreeBack(bsptree->front->back, sector_9);
	  RCA_AddNodeToBSPtreeFront(bsptree->front->back, 700, 0, 700, 719);
	    RCA_AddLeafToBSPtreeBack(bsptree->front->back->front, sector_10);
		RCA_AddLeafToBSPtreeFront(bsptree->front->back->front, sector_11);
  RCA_AddNodeToBSPtreeBack(bsptree, 0, 275, 1219, 275);
    RCA_AddNodeToBSPtreeFront(bsptree->back, 600, 0, 600, 719);
	  RCA_AddLeafToBSPtreeBack(bsptree->back->front, sector_6);
	  RCA_AddNodeToBSPtreeFront(bsptree->back->front, 0, 300, 1219, 300);
	    RCA_AddLeafToBSPtreeBack(bsptree->back->front->front, sector_7);
		RCA_AddLeafToBSPtreeFront(bsptree->back->front->front, sector_8);
	RCA_AddNodeToBSPtreeBack(bsptree->back, 0, 225, 1219, 225);
	  RCA_AddNodeToBSPtreeFront(bsptree->back->back, 400, 0, 400, 719);
	    RCA_AddLeafToBSPtreeBack(bsptree->back->back->front, sector_4);
		RCA_AddLeafToBSPtreeFront(bsptree->back->back->front, sector_5);
      RCA_AddNodeToBSPtreeBack(bsptree->back->back, 600, 0, 600, 719);
		RCA_AddLeafToBSPtreeBack(bsptree->back->back->back, sector_1);
		RCA_AddNodeToBSPtreeFront(bsptree->back->back->back, 0, 210, 1219, 210);
		  RCA_AddLeafToBSPtreeBack(bsptree->back->back->back->front, sector_2);
		  RCA_AddLeafToBSPtreeFront(bsptree->back->back->back->front, sector_3);
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
    RCA_DrawElement(screen, player);
    //RCA_DrawRays(screen, player, sector_1);
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

  SDL_Quit();

  return 0;
}