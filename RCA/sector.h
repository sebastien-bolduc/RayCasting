/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 2.00
 * @since 2012-02-26 
 */

#include <assert.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#ifndef RCA_SECTOR_H_
#define RCA_SECTOR_H_

#define RCA_SECTOR_TYPE (1<<1)		/* dynamic type checking */

/**
 * Sector class.
 */ 
typedef struct wall {
  unsigned int type;
  double x1;
  double y1;
  double x2;
  double y2;
  double floor;
  double ceiling;
  double floor_slope;
  double ceiling_slope;
  int bottom_color[4];
  int middle_color[4];
  int top_color[4];
  struct wall *first;
  struct wall *last;
  struct wall *current;
  struct wall *previous;
  struct wall *next;
} Sector;

/**
 * Constructor.
 *  
 * @param sector        Pointer to a Sector object.
 * @param x1	        Start point of wall.
 * @param y1	        Start piont of wall.
 * @param x2	        End point of wall.
 * @param y2	        End piont of wall.
 * @param floor         Ground on which the wall rest.
 * @param ceiling       Ceiling on which the wall rest.
 * @param floor_slope   Slope of the floor.
 * @param ceiling_slope Slope of the ceiling.
 * @param bottom_color  Bottom wall's color.
 * @param middle_color  Middle wall's color.
 * @param top_color	    Top wall's color.
 */
void RCA_ConstructSector(Sector *sector, double x1, double y1, double x2, double y2, double floor, double ceiling, 
						 double floor_slope, double ceiling_slope, int bottom_color[4], int middle_color[4], int top_color[4])
{
  /* here OR the RCA_SECTOR_TYPE constant into the type */
  sector->type |= RCA_SECTOR_TYPE;
  
  sector->x1 = x1;
  sector->y1 = y1;
  sector->x2 = x2;
  sector->y2 = y2;
  sector->floor = floor;
  sector->ceiling = ceiling;
  sector->floor_slope = floor_slope;
  sector->ceiling_slope = ceiling_slope;
  sector->bottom_color[0] = bottom_color[0]; sector->bottom_color[1] = bottom_color[1]; sector->bottom_color[2] = bottom_color[2]; sector->bottom_color[3] = bottom_color[3];
  sector->middle_color[0] = middle_color[0]; sector->middle_color[1] = middle_color[1]; sector->middle_color[2] = middle_color[2]; sector->middle_color[3] = middle_color[3];
  sector->top_color[0] = top_color[0]; sector->top_color[1] = top_color[1]; sector->top_color[2] = top_color[2]; sector->top_color[3] = top_color[3];
}

/**
 * New.
 * 
 * @return An object Sector.
 */
Sector *RCA_NewSector(void) 
{	
  Sector *sector = malloc(sizeof(Sector));
  sector->type = RCA_SECTOR_TYPE;
  
  /* call the constructor */
  int tmp[4] = {0};
  RCA_ConstructSector(sector, 0, 0, 0, 0, 0, 0, 0, 0, tmp, tmp, tmp);
  
  /* set pointer for previous and next element */
  sector->first = sector;
  sector->last = sector;
  sector->current = sector;
  sector->previous = NULL;
  sector->next = NULL;
  
  return sector;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param sector Pointer to a Sector object.
 */
void RCA_CheckSector(Sector *sector)
{
  /* check if we have a valid Sector object */
  if (sector == NULL || 
	  !(sector->type & RCA_SECTOR_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param sector Pointer to a Sector object.
 */ 
void RCA_DestroySector(Sector *sector)
{
  /* check if we have a valid Sector object */
  RCA_CheckSector(sector);

  /* set type to 0 indicate this is no longer a Sector object */
  sector->type = 0;

  /* free the memory allocated for the object */
  Sector *fst = sector->first;
  Sector *bkup = sector->first;
  Sector *cur = NULL;
  for (cur = fst; cur != NULL; cur = bkup)
  {
	bkup = cur->next;
	free(cur);
  }
}

/**
 * Add a wall to the sector.
 * 
 * Add a wall to the sector already existing.  Only the master
 * (the first created sector) can access this list.
 * 
 * @param sector        Pointer to a Sector object.
 * @param x1	        Start point of wall.
 * @param y1	        Start piont of wall.
 * @param x2	        End point of wall.
 * @param y2	        End piont of wall.
 * @param floor         Ground on which the wall rest.
 * @param ceiling       Ceiling on which the wall rest.
 * @param floor_slope   Slope of the floor.
 * @param ceiling_slope Slope of the ceiling.
 * @param bottom_color  Bottom wall's color.
 * @param middle_color  Middle wall's color.
 * @param top_color	    Top wall's color.
 */
void RCA_AddWallToSector(Sector *sector, double x1, double y1, double x2, double y2, double floor, double ceiling, 
						 double floor_slope, double ceiling_slope, int bottom_color[4], int middle_color[4], int top_color[4])
{
  /* check if we have a valid Sector object */
  RCA_CheckSector(sector);
	
  Sector *new_wall = malloc(sizeof(Sector));
  new_wall->type = RCA_SECTOR_TYPE;
  
  /* call the constructor */
  RCA_ConstructSector(new_wall, x1, y1, x2, y2, floor, ceiling, floor_slope, ceiling_slope, bottom_color, middle_color, top_color);
  
  /* set pointer for current element */
  new_wall->first = NULL;
  new_wall->last = NULL;
  new_wall->current = NULL;
  new_wall->previous = sector->current;
  if (sector->current->next != NULL)
  {
	new_wall->next = sector->current->next;
  }
  else 
  {
	new_wall->next = NULL;
	sector->last = new_wall;
  }
  
  /* set pointer for previous element */
  sector->current->next = new_wall;
  
  /* set current pointer to newly created element */
  sector->current = new_wall;
}

/**
 * Remove a wall from the sector.
 * 
 * Remove a wall from the sector already existing (the current one).  
 * Only the master (the first created sector) can access this list.
 * 
 * @param master Pointer to a mof_Graphicelement object.
 */
void RCA_RemoveWallFromSector(Sector *sector)
{ 
  /* check if we have a valid Sector object */
  RCA_CheckSector(sector);
  
  /* never remove the first element (the master) */
  if (sector->current->previous == NULL) 
    return;
  
  /* set pointer for previous element */
  sector->current->previous->next = sector->current->next;
  
  /* set pointer for next element*/
  if (sector->current->next != NULL)
  {
    sector->current->next->previous = sector->current->previous;
  }
  
  /* set type to 0 indicate this is no longer a Sector object */
  sector->current->type = 0;
  
  /* remove element */
  Sector *tmp = sector->current;
  Sector *bkup = sector->current->previous;
  free(tmp);
  sector->current = bkup;
}

/**
 * Return current wall of sector.
 * 
 * @param sector Pointer to a Sector object.
 * @return       Wall of sector.
 */
double *RCA_WallOfSector(Sector *sector)
{
  static double wall[4];
  
  wall[0] = sector->x1;
  wall[1] = sector->y1;
  wall[2] = sector->x2;
  wall[3] = sector->y2;
  
  return wall;
}

/**
 * Draw Sector.
 * 
 * @param screen A copy of the current SDL surface.
 * @param sector Pointer to a Sector object.
 */
void RCA_DrawSector(SDL_Surface *screen, Sector *sector)
{
  /* check if we have a valid Sector object */
  RCA_CheckSector(sector);
  
  sector->current = sector->first->next;
  while((sector->current) != NULL)
  {
	lineRGBA(screen, sector->current->x1, sector->current->y1, sector->current->x2, sector->current->y2, 
			 sector->current->top_color[0], sector->current->top_color[1], sector->current->top_color[2], 255);
	sector->current = sector->current->next;
  }
}

#endif