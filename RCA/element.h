/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.00
 * @since 2012-02-24
 */
 
#include <assert.h>
#include <math.h>
#include "SDL.h"
#include "SDL_gfxPrimitives.h"

#ifndef RCA_ELEMENT_H_
#define RCA_ELEMENT_H_

#define RCA_ELEMENT_TYPE (1<<0)		/* dynamic type checking */

/**
 * Element class.
 */ 
typedef struct {
  unsigned int type;
  double x;
  double y;
  double direction;				/* in degree */ 
} Element;

/**
 * Constructor.
 *  
 * @param element   Pointer to an Element object.
 * @param x         Coordinate of Element.
 * @param y         Coordinate of Element.
 * @param direction Direction of Element.
 */
void RCA_ConstructElement(Element *element, double x, double y, double direction)
{
  /* here OR the RCA_ELEMENT_TYPE constant into the type */
  element->type |= RCA_ELEMENT_TYPE;
   
  element->x = x;
  element->y = y;
  element->direction = direction;
}

/**
 * New.
 * 
 * @param x         Coordinate of Element.
 * @param y         Coordinate of Element.
 * @param direction Direction of Element.
 * @return          An object Element.
 */
Element *RCA_NewElement(double x, double y, double direction)
{	
  Element *element = malloc(sizeof(Element));
  element->type = RCA_ELEMENT_TYPE;
  
  /* call the constructor */
  RCA_ConstructElement(element, x, y, direction);
  
  return element;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param element Pointer to an Element object.
 */
void RCA_CheckElement(Element *element)
{
  /* check if we have a valid Element object */
  if (element == NULL || 
	  !(element->type & RCA_ELEMENT_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param element Pointer to an Element object.
 */ 
void RCA_DestroyElement(Element *element)
{
  /* check if we have a valid Element object */
  RCA_CheckElement(element);

  /* set type to 0 indicate this is no longer an Element object */
  element->type = 0;

  /* free the memory allocated for the object */
  free(element);
}

/**
 * Move Element forward.
 * 
 * @param element Pointer to an Element object.
 * @param speed   How much unit to move the Element.
 */
void RCA_MoveElementForward(Element *element, double speed)
{
  /* check if we have a valid Element object */
  RCA_CheckElement(element);	
	
  /* don't forget to convert degree to rad */
  element->x += (cos(element->direction * M_PI / 180) * speed);
  element->y += (sin(element->direction * M_PI / 180) * speed);
}

/**
 * Move Element backward.
 * 
 * @param element Pointer to an Element object.
 * @param speed   How much unit to move the Element.
 */
void RCA_MoveElementBackward(Element *element, double speed)
{
  /* check if we have a valid Element object */
  RCA_CheckElement(element);	

  /* don't forget to convert degree to rad */
  element->x -= (cos(element->direction * M_PI / 180) * speed);
  element->y -= (sin(element->direction * M_PI / 180) * speed);
}

/**
 * Move Element left (of facing direction).
 * 
 * @param element Pointer to an Element object.
 * @param speed   How much unit to move the Element.
 */
void RCA_MoveElementLeft(Element *element, double speed)
{
  /* check if we have a valid Element object */
  RCA_CheckElement(element);	
	
  /* don't forget to convert degree to rad */
  element->x += (cos((element->direction - 90) * M_PI / 180) * speed);
  element->y += (sin((element->direction - 90) * M_PI / 180) * speed);
}

/**
 * Move Element right (of facing direction).
 * 
 * @param element Pointer to an Element object.
 * @param speed   How much unit to move the Element.
 */
void RCA_MoveElementRight(Element *element, double speed)
{
  /* check if we have a valid Element object */
  RCA_CheckElement(element);
	
  /* don't forget to convert degree to rad */
  element->x += (cos((element->direction + 90) * M_PI / 180) * speed);
  element->y += (sin((element->direction + 90) * M_PI / 180) * speed);
}

/**
 * Rotate direction of Element.
 * 0
 * @param element Pointer to an Element object.
 * @param speed   How much to rotate Element.
 */
void RCA_RotateElement(Element *element, int speed)
{
  /* check if we have a valid Element object */
  RCA_CheckElement(element);
  
  element->direction += speed;
  
  /* check angle limits */
  if (element->direction < 0)
  {
	element->direction += 360;
  }
  if (element->direction >= 360)
  {
	element->direction -= 360;
  }
}

/**
 * Draw Element.
 * 
 * @param screen  A copy of the current SDL surface.
 * @param element Pointer to an Element object.
 */
void RCA_DrawElement(SDL_Surface *screen, Element *element)
{
  /* check if we have a valid Element object */
  RCA_CheckElement(element);
  
  filledEllipseRGBA(screen, (int)element->x, (int)element->y, 10, 10, 255, 0, 0, 255);
  int lineX = element->x + (cos(element->direction * M_PI / 180) * 20);
  int lineY = element->y + (sin(element->direction * M_PI / 180) * 20);
  int arrow1X = element->x + (cos((element->direction + 30) * M_PI / 180) * 14);
  int arrow1Y = element->y + (sin((element->direction + 30) * M_PI / 180) * 14);
  int arrow2X = element->x + (cos((element->direction - 30) * M_PI / 180) * 14);
  int arrow2Y = element->y + (sin((element->direction - 30) * M_PI / 180) * 14); 
  lineRGBA(screen, (int)element->x, (int)element->y, (int)lineX, (int)lineY, 0, 255, 0, 255);
  lineRGBA(screen, (int)arrow1X, (int)arrow1Y, (int)lineX, (int)lineY, 0, 255, 0, 255);
  lineRGBA(screen, (int)arrow2X, (int)arrow2Y, (int)lineX, (int)lineY, 0, 255, 0, 255);
}

#endif
