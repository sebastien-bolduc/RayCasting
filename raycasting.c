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

/* -------------------------------------------------------------------------------------------------------- */
/* walls and player data */
//double wall_1[4] = {-200, 100, 200, 100};
//double wall_1[4] = {-200, 150, 200, 50};
double wall_1[4] = {-200, 50, 200, 150};
double player_1[2] = {0, 50};

/**
 * Find slope of wall.
 */
double RCA_FindWallsSlope(double wall[4])
{
  double m;
  double x1 = wall[0]; double y1 = wall[1];
  double x2 = wall[2]; double y2 = wall[3];
	
  if ((x2 - x1) == 0) 
  {
	m = tan(90 * M_PI / 180);		/* wrong way to handel it ... */	
  } 
  else 
  {
	m = (y2 - y1) / (x2 - x1);
  }
	
  return m;
}

/**
 * Find slope of ray.
 */
double RCA_FindRaysSlope(double angle_of_ray)
{
	return tan(angle_of_ray * M_PI / 180);		/* must handle infinite case ... */
}

/**
 * Find Intersection.
 */
double *RCA_FindIntersection(double player[2], double wall[4], double angle_of_ray)
{
  static double intersection[2] = {0, 0};
	
  double b1;
  double x, y;
  
  double playerx = player[0]; double playery = player[1];
  double x1 = wall[0]; double y1 = wall[1];
  double m1 = RCA_FindWallsSlope(wall);
  double m2 = RCA_FindRaysSlope(angle_of_ray);
  
  b1 = -(x1 - playerx) * m1 + (y1 - playery);
  
  x = b1 / (m2 - m1) + playerx;			/* origin of the coordinate system is the player */
  y = y1 + (x - x1) * m1;
  
  intersection[0] = x;
  intersection[1] = y;
  
  return intersection;
}

/**
 * Getting distance to wall slice.
 */
double RCA_GettingWallDistance(double player[2], double *intersection, double angle_of_ray)
{
  double distance;
  double x_diff, y_diff;	

  double playerx = player[0]; double playery = player[1];	
  double xs = intersection[0]; double ys = intersection[1];
  double angle = angle_of_ray;	
	
  x_diff = fabs(playerx - xs);
  y_diff = fabs(playery - ys);
  
  if ( abs(x_diff) > abs(y_diff) )
  { 
	distance = x_diff / cos(angle * M_PI / 180); 
  }
  else
  {
	distance = y_diff / sin(angle * M_PI / 180);
  }
  
  return distance;
}

/**
 * Getting height of wall slice.
 */
double RCA_GettingWallHeight(double distance)
{
  double height;
  double certain_value = 20000;	
	
  if (distance == 0) 
  {
	distance = 1;							/* prevent the formula from dividing by zero */  
  }
  
  height = certain_value / distance;		/* get the height by just dividing */
  
  return height;
}

/**
 * Drawing the walls.
 */
void RCA_DrawingWalls(void)
{
  double *intersection = NULL;
  double distance = 0;
  double height = 0;
  int x = 0; int y;
  int i;
  double angle = 120.0;
  
  for (i = 0; i < 128; i++)
  {
	intersection = RCA_FindIntersection(player_1, wall_1, angle);
	distance = RCA_GettingWallDistance(player_1, intersection, angle);
	
	distance *= fabs(cos((angle - 90) * M_PI / 180));			/* correcting distance */
	
	height = RCA_GettingWallHeight(distance); 
	
	if (1)
	{
	  lineRGBA(screen, wall_1[0] + (screen->w / 2), wall_1[1], wall_1[2] + (screen->w / 2), wall_1[3], 255, 255, 255, 255);
	  lineRGBA(screen, wall_1[0], wall_1[1], wall_1[2], wall_1[3], 255, 255, 255, 255);
	  lineRGBA(screen, (screen->w / 2) + 57, 0, (screen->w / 2) + 57, 400, 0, 255, 0, 255);
	  lineRGBA(screen, (screen->w / 2) + 50, 100, (screen->w / 2) + 64, 100, 0, 255, 0, 255);
	  lineRGBA(screen, player_1[0] + (screen->w / 2), player_1[1], intersection[0] + (screen->w / 2), intersection[1], 255, 255, 0, 255);
	}
	else 
	{
	  y = (screen->h / 2) - (int)(height / 2);
  
	  boxRGBA(screen, x, y, (x + 9), (y + (int)height), 255, 0, 0, 255);
	
	  x += 10;
	}
	
	angle -= (60.0 / 128);
  }
  
  /* print to bottom of screen */
  sprintf(test, "(intersection) x: %lf -- y: %lf", intersection[0], intersection[1]);
  mof_Font__printf(text, test, 20, screen->h - 60);
  sprintf(test, "(distance) d: %lf", distance);
  mof_Font__printf(text, test, 20, screen->h - 40);
  sprintf(test, "(height) h: %lf", height);
  mof_Font__printf(text, test, 20, screen->h - 20);
}
/* -------------------------------------------------------------------------------------------------------- */

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
  player = RCA_NewElement(605, 700, 270);
  sector_1 = RCA_NewSector();
}

/**
 * Loading.
 */
void RCA_Load()
{
  /* TODO: add your code here */
  
  /* house */
  RCA_AddWallToSector(sector_1, 201, 299, 999, 299, 255, 0, 0, 255);
  RCA_AddWallToSector(sector_1, 200, 300, 200, 600, 100, 0, 0, 255);
  RCA_AddWallToSector(sector_1, 1000, 300, 1000, 600, 100, 0, 0, 255);
  RCA_AddWallToSector(sector_1, 200, 299, 599.1, 100, 0, 0, 255, 255);
  RCA_AddWallToSector(sector_1, 600.9, 100, 1000, 299, 0, 0, 100, 255);
  
  /* fence */
  RCA_AddWallToSector(sector_1, 0, 0, 0, 719, 255, 255, 255, 255);
  RCA_AddWallToSector(sector_1, 1279, 0, 1279, 719, 255, 255, 255, 255);
  RCA_AddWallToSector(sector_1, 0, 0, 1279, 0, 255, 255, 255, 255);
  RCA_AddWallToSector(sector_1, 0, 719, 1279, 719, 255, 255, 255, 255);
  
  /* column */
  RCA_AddWallToSector(sector_1, 501, 501, 509, 501, 0, 255, 0, 255);
  RCA_AddWallToSector(sector_1, 501, 449, 509, 449, 0, 255, 0, 255);
  RCA_AddWallToSector(sector_1, 500, 450, 500, 500, 0, 100, 0, 255);
  RCA_AddWallToSector(sector_1, 510, 450, 510, 500, 0, 100, 0, 255);
  
  RCA_AddWallToSector(sector_1, 701, 501, 709, 501, 0, 255, 0, 255);
  RCA_AddWallToSector(sector_1, 701, 449, 709, 449, 0, 255, 0, 255);
  RCA_AddWallToSector(sector_1, 700, 450, 700, 500, 0, 100, 0, 255);
  RCA_AddWallToSector(sector_1, 710, 450, 710, 500, 0, 100, 0, 255);
  
  RCA_AddWallToSector(sector_1, 601, 501, 609, 501, 0, 255, 0, 255);
  RCA_AddWallToSector(sector_1, 601, 449, 609, 449, 0, 255, 0, 255);
  RCA_AddWallToSector(sector_1, 600, 450, 590, 474.1, 0, 100, 0, 255);
  RCA_AddWallToSector(sector_1, 610, 450, 620, 474.1, 0, 100, 0, 255);
  RCA_AddWallToSector(sector_1, 590, 475.9, 600, 500, 0, 150, 0, 255);
  RCA_AddWallToSector(sector_1, 620, 475.9, 610, 500, 0, 150, 0, 255);
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
		RCA_RotateElement(player, 1);
	  }
	  if (event.motion.xrel < 0)
	  {
		RCA_RotateElement(player, -1);
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
  //RCA_DrawingWalls();
  
  if (mapflag)
  {
    RCA_DrawSector(screen, sector_1);
    RCA_DrawElement(screen, player);
    RCA_DrawRays(screen, player, sector_1);
  }
  else 
  {
	RCA_Draw3D(screen, player, sector_1);
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
  RCA_DestroyElement(player);
  RCA_DestroySector(sector_1);

  SDL_Quit();

  return 0;
}