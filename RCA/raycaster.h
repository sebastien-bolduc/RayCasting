/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.00
 * @since 2012-02-24
 * 
 * Raycasting using the method describe at that website:
 * {@link http://www.gamedev.net/page/resources/_/technical/graphics-programming-and-theory/advanced-raycasting-techniques-r872} 
 */
 
#include <math.h>

#include "element.h"
#include "sector.h"

#ifndef RCA_RAYCASTER_H_
#define RCA_RAYCASTER_H_

/**
 * Find slope of wall (gradient).
 * 
 * @param wall An array containing the wall's start and end coordinate.
 * @return     Slope (gradient) of the line (wall).
 */
double RCA_FindWallsGradient(double wall[4])
{
  double m;
  double x1 = wall[0]; double y1 = wall[1];
  double x2 = wall[2]; double y2 = wall[3];
	
  m = (y2 - y1) / (x2 - x1);			/* must handle infinite case ... */
	
  return m;
}

/**
 * Find slope of ray (gradient).
 * 
 * @param angle_of_ray Angle of the ray casted.
 * @return             Slope (gradient) of the line (ray).
 */
double RCA_FindRaysGradient(double angle_of_ray)
{
	if (angle_of_ray == 0 || angle_of_ray == 180)
	  return 0;
	else 
	  return tan(angle_of_ray * M_PI / 180);		/* must handle infinite case ... */
}

/**
 * Find Intersection of ray and wall.
 * 
 * @param element      Pointer to an Element object.
 * @param wall         An array containing the wall's start and end coordinate.
 * @param angle_of_ray Angle of the ray casted.
 * @return             Intersection point of the ray and wall.
 */
double *RCA_FindWallIntersection(Element *element, double wall[4], double angle_of_ray)
{
  static double intersection[2] = {0, 0};
	
  double b1;
  double x, y;
  
  double playerx = element->x; double playery = element->y;
  double x1 = wall[0]; double y1 = wall[1];
  double x2 = wall[2]; double y2 = wall[3];
  double m1 = RCA_FindWallsGradient(wall);
  double m2 = RCA_FindRaysGradient(angle_of_ray);
  
  /* handling the infinite case and more... */
  if (m1 == 0 && m2 == 0)
  {
	x = (fabs(x1 - playerx) < fabs(x2 - playerx)) ? x1 : x2;
	x = ((fabs(x1 - playerx) + fabs(x2 - playerx)) == fabs(x1 - x2)) ? playerx : x;
	
	if (fabs(playery - y1) <= 1)
	  y = y1;
	else 
	  return NULL;
  }
  else if (isinf(m1) && isinf(m2))
  {
	y = (fabs(y1 - playery) < fabs(y2 - playery)) ? y1 : y2;
	y = ((fabs(y1 - playery) + fabs(y2 - playery)) == fabs(y1 - y2)) ? playery : y;
	
	if (fabs(playerx - x1) <= 1)
	  x = x1;
	else 
	  return NULL;
  }
  else if (isinf(m1))
  {
	x = x1 - playerx;						/* origin of the coordinate system is the player */
	y = x * m2;
	x += playerx;
	y += playery;
  }
  else if (isinf(m2))
  {
	x = playerx;							/* origin of the coordinate system is the player */
	y = y1 + (x - x1) * m1;
  }
  else 
  {
    b1 = -(x1 - playerx) * m1 + (y1 - playery);
  
    x = b1 / (m2 - m1) + playerx;			/* origin of the coordinate system is the player */
    y = y1 + (x - x1) * m1;
  }
  
  intersection[0] = x;
  intersection[1] = y;
  
  return intersection;
}

/**
 * Getting distance to wall slice.
 * 
 * @param element      Pointer to an Element object.
 * @param intersection Intersection point of the ray and wall.
 * @param angle_of_ray Angle of the ray casted.
 * @return             Distance to wall.
 */
double RCA_GettingDistanceToWall(Element *element, double *intersection, double angle_of_ray)
{
  double distance;
  double x_diff, y_diff;	

  double playerx = element->x; double playery = element->y;	
  double xs = intersection[0]; double ys = intersection[1];
  double angle = angle_of_ray;	
	
  x_diff = fabs(playerx - xs);
  y_diff = fabs(playery - ys);
  
  distance = sqrt(pow(x_diff, 2) + pow(y_diff, 2));
  
  return distance;
}

/**
 * Getting height of wall slice.
 * 
 * @param distance Distance to wall slice.
 * @return         Height of wall.
 */
double RCA_GettingHeightOfWall(double distance)
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

/* ------------------------------------------------------------------------------------------ */
/**
 * Check angle limit.
 * 
 * @param angle Angle to check.
 * @return      Corrected angle (if needed).
 */
double RCA_CheckAngleLimit(double angle)
{
  double correct_angle = angle;

  /* check angle limits */
  if (angle < 0)
  {
	correct_angle = angle + 360;
  }
  if (angle >= 360)
  {
	correct_angle = angle - 360;
  }
  
  return correct_angle;
}

/**
 * Check wall limit.
 * 
 * @param wall An array containing the wall's start and end coordinate.
 * @param xs   Intersection point.
 * @param ys   Intersection point.
 * @return     True (1) or false (0) depending if we are within the limit of the wall.
 */
int RCA_CheckWallLimit(double wall[4], double xs, double ys)
{
  if (fabs((wall[0] - xs) + (wall[2] - xs)) > (fabs(wall[2] - wall[0])))
	return 0;
  if (fabs((wall[1] - ys) + (wall[3] - ys)) > (fabs(wall[3] - wall[1])))
	return 0;
	
  return 1;
}

/**
 * Get quadrant for angle.
 * 
 * @param angle Angle to get quadrant for.
 * @return      Quadrant in which angle is.
 */
int RCA_GetQuadrant(double angle)
{
  if ((angle >= 0) && (angle < 90)) return(1);          /* angle in first quadrant */
  if ((angle >= 90) && (angle < 180)) return(2);        /* angle in second quadrant */
  if ((angle >= 180) && (angle < 270)) return(3);       /* angle in third quadrant */
  if ((angle >= 270) && (angle < 360)) return(4);       /* angle in fourth quadrant */
}

/**
 * Get quadrant for intersection point.
 * 
 * @param element Pointer to an Element object.
 * @param xs      Intersection point.
 * @param ys      Intersection point.
 * @return        Quadrant in which intersection point is.
 */
int RCA_GetIntersectionQuadrant(Element *element, double xs, double ys)
{
  double x_diff, y_diff;

  x_diff = -element->x + xs;
  y_diff = -element->y + ys;

  if (x_diff > 0) 
  {
	if (y_diff < 0) return(4); else return(1);
  } 
  else 
  {
	if (y_diff < 0) return(3); else return(2);
  }  
}

/**
 * Check correctness of intersection.
 * 
 * @param element      Pointer to an Element object.
 * @param xs           Intersection point.
 * @param ys           Intersection point.
 * @param angle_of_ray Angle of the ray casted.
 * @return             True (1) or false (0) depending of the ray and intersection position.
 */
int RCA_CorrectIntersection(Element *element, double xs, double ys, double angle_of_ray)
{
  /* this doesn't work quite right */
  /* if (RCA_GetIntersectionQuadrant(element, xs, ys) == RCA_GetQuadrant(angle_of_ray)) return(1); else return(0); */
  
  double adj, opp;
  double angle;

  adj = -element->x + xs;
  opp = -element->y + ys;
  
  if (adj == 0 && opp >= 0)
	angle = 90;
  else if (adj == 0)
	angle = 270;	
  else if (adj > 0 && opp >= 0)
	angle = fabs(atan(opp / adj)) * 180 / M_PI;
  else if (adj < 0 && opp >= 0)
	angle = 180 - fabs(atan(opp / adj)) * 180 / M_PI;
  else if (adj < 0 && opp < 0)
	angle = 180 + fabs(atan(opp / adj)) * 180 / M_PI;
  else
	angle = 360 - fabs(atan(opp / adj)) * 180 / M_PI;
  
  if (fabs(angle - angle_of_ray) < 10 || fabs(angle - angle_of_ray) > 350)
	return (1);
  else
	return (0);
}
/* ------------------------------------------------------------------------------------------ */

/**
 * Draw Rays casted.
 * 
 * @param screen  A copy of the current SDL surface.
 * @param element Pointer to an Element object.
 * @param sector  Pointer to a Sector object.
 */
void RCA_DrawRays(SDL_Surface *screen, Element *element, Sector *sector)
{
  double intersection[2] = {0, 0}; double *tmp = NULL;
  double current_top_distance = -1; double distance = 0;
  int i;
  double ray_angle = element->direction + 30;
  int flag = 0;
  
  for (i = 0; i < 256; i++)
  {
	current_top_distance = -1;
	intersection[0] = 0;
	intersection[1] = 0;
	flag = 0;
	  
	sector->current = sector->first->next;
	while((sector->current) != NULL)
	{
	  tmp = RCA_FindWallIntersection(element, RCA_WallOfSector(sector->current), RCA_CheckAngleLimit(ray_angle));
	  
	  if (tmp != NULL)
	  {
	    if (RCA_CorrectIntersection(element, tmp[0], tmp[1], RCA_CheckAngleLimit(ray_angle)) &&
		    RCA_CheckWallLimit(RCA_WallOfSector(sector->current), tmp[0], tmp[1]))
	    {
	      distance = RCA_GettingDistanceToWall(element, tmp, RCA_CheckAngleLimit(ray_angle));
	      if (current_top_distance == -1 || distance < current_top_distance)
	      {
		    intersection[0] = tmp[0];
		    intersection[1] = tmp[1];
		    current_top_distance = distance;
		    flag = 1;
	      }
		}
	  }
	  sector->current = sector->current->next;
	}

	if (flag)
	{
	  lineRGBA(screen, element->x, element->y, intersection[0], intersection[1], 255, 255, 0, 50);
	}
	
	ray_angle -= (60.0 / 256);
  }
}

/**
 * Floor casting.
 * 
 * @param screen           A copy of the current SDL surface.
 * @param position_of_wall Position of the wall being drawn.
 * @param bottom_of_wall   Bottom of the wall being drawn.
 * @param top_of_wall      Top of previous wall drawn.
 */
void RCA_FloorCasting(SDL_Surface *screen, int position_of_wall, int bottom_of_wall, int top_of_wall)
{
  int x1 = position_of_wall;
  int y1 = bottom_of_wall;
  int x2 = x1 + 5;
  int y2 = top_of_wall;
	
  boxRGBA(screen, x1, y1, x2, y2, 0, 0, 100, 255);
}

/**
 * Ceiling casting.
 * 
 * @param screen               A copy of the current SDL surface.
 * @param position_of_wall     Position of the wall being drawn.
 * @param top_of_wall          Top of the wall being drawn.
 * @param top_of_previous_wall Top of previous wall drawn.
 */
void RCA_CeilingCasting(SDL_Surface *screen, int position_of_wall, int top_of_wall, int top_of_previous_wall)
{
  int x1 = position_of_wall;
  int y1 = top_of_wall;
  int x2 = x1 + 5;
  int y2 = top_of_previous_wall;
	
  boxRGBA(screen, x1, y1, x2, y2, 100, 0, 0, 255);
}

/**
 * Wall casting.
 * 
 * @param screen  A copy of the current SDL surface.
 * @param element Pointer to an Element object.
 * @param sector  Pointer to a Sector object.
 */
void RCA_WallCasting(SDL_Surface *screen, Element *element, Sector *sector) 
{
  if (sector == NULL)
    return;

  int i;
  double *intersection = NULL;	
  double distance = 0, previous_distance = -1, corrected_distance = 0;
  double height;
  int floor_top, floor_bottom, previous_floor_top = (screen->h - 1);
  int middle_top, middle_bottom, previous_middle_top = (screen->h - 1);
  int ceiling_top, ceiling_bottom, previous_ceiling_bottom = 0;
  int no_floor_floor_flag = 0, no_floor_ceiling_flag = 1;
  int no_ceiling_floor_flag = 1, no_ceiling_ceiling_flag = 0;
  double ray_angle = element->direction + 30;
  int slice_position = 1275;
  Sector *wall = NULL;
	
  for (i = 0; i < 256; i++)
  {
	previous_distance = -1;
	previous_floor_top = (screen->h - 1);
	previous_middle_top = (screen->h - 1);
	previous_ceiling_bottom = 0;
	no_floor_ceiling_flag = 1;
	no_ceiling_floor_flag = 1;
	  
	sector->current = sector->first->next;
	while((sector->current) != NULL)
	{
	  intersection = RCA_FindWallIntersection(element, RCA_WallOfSector(sector->current), RCA_CheckAngleLimit(ray_angle));
	  
	  if (intersection != NULL)
	  {
	    if (RCA_CorrectIntersection(element, intersection[0], intersection[1], RCA_CheckAngleLimit(ray_angle)) &&
		    RCA_CheckWallLimit(RCA_WallOfSector(sector->current), intersection[0], intersection[1]))
	    {
		  wall = sector->current;	
			
		  distance = RCA_GettingDistanceToWall(element, intersection, RCA_CheckAngleLimit(ray_angle));
		  
		  /* correcting distance */
		  corrected_distance = distance * fabs(cos((ray_angle - element->direction) * M_PI / 180)); 
		  
		  height = RCA_GettingHeightOfWall(corrected_distance);
		  
		  /* bottom */
		  if (wall->floor != 0)
		  {
			floor_top = (screen->h / 2) - (int)(height / 2) + (int)height - (int)floor(wall->floor * height / 100);
		    floor_bottom = (screen->h / 2) - (int)(height / 2) + (int)height;
			if (distance > previous_distance && previous_distance != -1)
		    {
			  if (floor_bottom >= previous_floor_top)
			  {
			    floor_bottom = previous_floor_top - 1;
				no_floor_floor_flag = 1;
			  }
		    }
			else
		    {
			  if (floor_top <= previous_floor_top)
			  {
			    no_floor_ceiling_flag = 1;
			  }
		    }
			
			if (floor_top <= floor_bottom)
		    {
			  boxRGBA(screen, slice_position, floor_top, slice_position + 5, floor_bottom, wall->r, wall->g, wall->b, wall->a);
			  if (!no_floor_floor_flag)
			    RCA_FloorCasting(screen, slice_position, floor_bottom, (previous_floor_top > floor_bottom) ? previous_floor_top : (screen->h - 1));
			  if (!no_floor_ceiling_flag)
			    RCA_CeilingCasting(screen, slice_position, floor_top, previous_floor_top);
		    }
		  }
		  previous_floor_top = floor_top;
		  no_floor_floor_flag = 0;
		  no_floor_ceiling_flag = 0;
		  
		  /* top */
		  if (wall->ceiling != 0)
		  {
			ceiling_top = (screen->h / 2) - (int)(height / 2);
		    ceiling_bottom = (screen->h / 2) - (int)(height / 2) + (int)floor(wall->ceiling * height / 100);			
			if (distance > previous_distance && previous_distance != -1)
		    {
			  if (ceiling_top <= previous_ceiling_bottom)
			  {
			    ceiling_top = previous_ceiling_bottom + 1;
				no_ceiling_ceiling_flag = 1;
			  }
		    }
			else
		    {
			  if (ceiling_bottom >= previous_ceiling_bottom)
			  {
			    no_ceiling_floor_flag = 1;
			  }
		    }
			
			if (ceiling_bottom >= ceiling_top)
		    {
			  boxRGBA(screen, slice_position, ceiling_top, slice_position + 5, ceiling_bottom, wall->r, wall->g, wall->b, wall->a);
			  if (!no_ceiling_ceiling_flag)
			    RCA_CeilingCasting(screen, slice_position, ceiling_top, (previous_ceiling_bottom < ceiling_top) ? previous_ceiling_bottom : 0);
			  if (!no_ceiling_floor_flag)
			    RCA_FloorCasting(screen, slice_position, ceiling_bottom, previous_ceiling_bottom);
		    }
		  }
		  previous_ceiling_bottom = ceiling_bottom;
		  no_ceiling_floor_flag = 0;
		  no_ceiling_ceiling_flag = 0;
			
		  /* middle */
		  if (wall->floor == 0 && wall->ceiling == 0)
		  {
			middle_top = (screen->h / 2) - (int)(height / 2);
		    middle_bottom = (screen->h / 2) - (int)(height / 2) + (int)height;
			if (distance > previous_distance && previous_distance != -1)
		    {
			  if (middle_bottom >= previous_middle_top)
			  {
			    middle_bottom = previous_middle_top - 1;
			  }
		    }
			
			if (middle_top <= middle_bottom)
		    {
			  boxRGBA(screen, slice_position, middle_top, slice_position + 5, middle_bottom, wall->r, wall->g, wall->b, wall->a);
			  RCA_FloorCasting(screen, slice_position, middle_bottom, (screen->h - 1));
			  RCA_CeilingCasting(screen, slice_position, middle_top, 0);
		    }
		  }
		  previous_middle_top = middle_top;
		  
		  previous_distance = distance;
		}
	  }
	  sector->current = sector->current->next;
	}  
	  
	ray_angle -= (60.0 / 256);
	slice_position -= 5;
  }
}

#endif