/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 0.01
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
	
  if ((x2 - x1) == 0) 
  {
	m = 10000000;		/* wrong way to handel it ... */	
  } 
  else 
  {
	m = (y2 - y1) / (x2 - x1);
  }
	
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
  double m1 = RCA_FindWallsGradient(wall);
  double m2 = RCA_FindRaysGradient(angle_of_ray);
  
  b1 = -(x1 - playerx) * m1 + (y1 - playery);
  
  x = b1 / (m2 - m1) + playerx;			/* origin of the coordinate system is the player */
  y = y1 + (x - x1) * m1;
  
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
  
  /* this might be error prone */
  /*if (abs(x_diff) > abs(y_diff))
  { 
	distance = x_diff / fabs(cos(angle * M_PI / 180)); 
  }
  else
  {
	distance = y_diff / fabs(sin(angle * M_PI / 180));
  }*/
  
  distance = sqrt(pow(x_diff, 2) + pow(y_diff, 2));
  
  return distance;
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
  if (fabs((wall[0] - xs) + (wall[2] - xs)) > (fabs(wall[2] - wall[0]) + 1))
	return 0;
  if (fabs((wall[1] - ys) + (wall[3] - ys)) > (fabs(wall[3] - wall[1]) + 1))
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
  //if (RCA_GetIntersectionQuadrant(element, xs, ys) == RCA_GetQuadrant(angle_of_ray)) return(1); else return(0);
  
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
  
  for (i = 0; i < 128; i++)
  {
	current_top_distance = -1;
	intersection[0] = 0;
	intersection[1] = 0;
	flag = 0;
	  
	sector->current = sector->first->next;
	while((sector->current) != NULL)
	{
	  tmp = RCA_FindWallIntersection(element, RCA_WallOfSector(sector->current), RCA_CheckAngleLimit(ray_angle));
	  
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
	  sector->current = sector->current->next;
	}

	if (flag)
	{
	  lineRGBA(screen, element->x, element->y, intersection[0], intersection[1], 255, 255, 0, 50);
	}
	
	ray_angle -= (60.0 / 128);
  }
}

#endif