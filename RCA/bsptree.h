/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.00
 * @since 2012-03-04 
 */
 
#include <assert.h>

#include "element.h"
#include "raycaster.h"
#include "sector.h"

#ifndef RCA_BSPTREE_H_
#define RCA_BSPTREE_H_

#define RCA_BSPTREE_TYPE (1<<2)		/* dynamic type checking */

typedef struct node {
  unsigned int type;
  double x1;
  double y1;
  double x2;
  double y2;
  Sector *sector;
  struct node *front;
  struct node *back;
} BSPtree;

/**
 * Constructor.
 * 
 * @param bsptree Pointer to a BSPtree object.
 * @param x1      Start point of line.
 * @param y1 	  Start point of line.
 * @param x2      End point of line.
 * @param y2      End point of line.
 */
void RCA_ConstructBSPtree(BSPtree *bsptree, double x1, double y1, double x2, double y2)
{
  /* here OR the RCA_BSPTREE_TYPE constant into the type */
  bsptree->type |= RCA_BSPTREE_TYPE;
  
  bsptree->x1 = x1;
  bsptree->y1 = y1;
  bsptree->x2 = x2;
  bsptree->y2 = y2;
  
  bsptree->sector = NULL;
  bsptree->front = NULL;
  bsptree->back = NULL;
}

/**
 * New.
 * 
 * @param x1 Start point of line.
 * @param y1 Start point of line.
 * @param x2 End point of line.
 * @param y2 End point of line.
 * @return   An object BSPtree.
 */
BSPtree *RCA_NewBSPtree(double x1, double y1, double x2, double y2) 
{	
  BSPtree *bsptree = malloc(sizeof(BSPtree));
  bsptree->type = RCA_BSPTREE_TYPE;
  
  /* call the constructor */
  RCA_ConstructBSPtree(bsptree, x1, y1, x2, y2);
  
  return bsptree;
}

/**
 * Check object for validity.
 * 
 * Check to see if the object we are trying to interact with is of
 * the good type.
 * 
 * @param bsptree Pointer to a BSPtree object.
 */
void RCA_CheckBSPtree(BSPtree *bsptree)
{
  /* check if we have a valid BSPtree object */
  if (bsptree == NULL || 
	  !(bsptree->type & RCA_BSPTREE_TYPE))
  {
	assert(0);
  }
}

/**
 * Destructor.
 * 
 * @param bsptree Pointer to a BSPtree object.
 */ 
void RCA_DestroyBSPtree(BSPtree *bsptree)
{
  /* check if we have a valid BSPtree object */
  RCA_CheckBSPtree(bsptree);

  /* set type to 0 indicate this is no longer a BSPtree object */
  bsptree->type = 0;

  /* free the memory allocated for the object */
  if (bsptree->front != NULL)
	RCA_DestroyBSPtree(bsptree->front);
  if (bsptree->back != NULL)
	RCA_DestroyBSPtree(bsptree->back);

  free(bsptree);
}

/**
 * Add Front node.
 * 
 * @param current_node Pointer to a BSPtree object.
 */
void RCA_AddNodeToBSPtreeFront(BSPtree *current_node, double x1, double y1, double x2, double y2)
{
  /* check if we have a valid BSPtree object */
  RCA_CheckBSPtree(current_node);
	
  BSPtree *new_node = malloc(sizeof(BSPtree));
  new_node->type = RCA_BSPTREE_TYPE;
  
  /* call the constructor */
  RCA_ConstructBSPtree(new_node, x1, y1, x2, y2);
  
  /* connect the node to the parent node */
  current_node->front = new_node;
}

/**
 * Add Back node.
 * 
 * @param current_node Pointer to a BSPtree object.
 */
void RCA_AddNodeToBSPtreeBack(BSPtree *current_node, double x1, double y1, double x2, double y2)
{
  /* check if we have a valid BSPtree object */
  RCA_CheckBSPtree(current_node);
	
  BSPtree *new_node = malloc(sizeof(BSPtree));
  new_node->type = RCA_BSPTREE_TYPE;
  
  /* call the constructor */
  RCA_ConstructBSPtree(new_node, x1, y1, x2, y2);
  
  /* connect the node to the parent node */
  current_node->back = new_node;
}

/**
 * Add Front leaf.
 * 
 * @param current_node Pointer to a BSPtree object.
 * @param sector       Pointer to a Sector object.
 */
void RCA_AddLeafToBSPtreeFront(BSPtree *current_node, Sector *sector)
{
  /* check if we have a valid BSPtree object */
  RCA_CheckBSPtree(current_node);
	
  BSPtree *new_leaf = malloc(sizeof(BSPtree));
  new_leaf->type = RCA_BSPTREE_TYPE;
  
  /* call the constructor */
  RCA_ConstructBSPtree(new_leaf, 0, 0, 0, 0);
  
  /* set the sector associated with the leaf */
  new_leaf->sector = sector;
  
  /* connect the leaf to the node */
  current_node->front = new_leaf;
}

/**
 * Add Back leaf.
 * 
 * @param current_node Pointer to a BSPtree object.
 * @param sector       Pointer to a Sector object.
 */
void RCA_AddLeafToBSPtreeBack(BSPtree *current_node, Sector *sector)
{
  /* check if we have a valid BSPtree object */
  RCA_CheckBSPtree(current_node);
	
  BSPtree *new_leaf = malloc(sizeof(BSPtree));
  new_leaf->type = RCA_BSPTREE_TYPE;
  
  /* call the constructor */
  RCA_ConstructBSPtree(new_leaf, 0, 0, 0, 0);
  
  /* set the sector associated with the leaf */
  new_leaf->sector = sector;
  
  /* connect the leaf to the node */
  current_node->back = new_leaf;
}

/**
 * Find location.
 * 
 * @param node    Pointer to a BSPtree object.
 * @param element Pointer to an Element object.
 * @return        If we are on (0), back (-1) or in front (1) of the separating line.
 */
int RCA_FindLocationInBSPtree(BSPtree *node, Element *element)
{
  /* check if we have a valid BSPtree object */
  RCA_CheckBSPtree(node);	
	
  if ((node->x1 - node->x2) == 0)
  {
	if ((element->x - node->x1) == 0)
	  return 0;
	else if ((element->x - node->x1) < 0)
	  return -1;
	else 
	  return 1;
  }
  else if ((node->y1 - node->y2) == 0)
  {
	if ((element->y - node->y1) == 0)
	  return 0;
	else if ((element->y - node->y1) < 0)
	  return -1;
	else 
	  return 1;
  }
  
  double m, b, y;
  
  m = (node->y2 - node->y1) / (node->x2 - node->x1);
  b = node->y1 - (node->x1 * m);
  
  y = (element->x * m) + b;
  
  if (element->y > y)
	return 1;
  else if (element->y < y)
	return -1;
  else 
	return 0;
}

/**
 * Traverse tree.
 * 
 * @param screen  A copy of the current SDL surface.
 * @param bsptree Pointer to a BSPtree object.
 * @param element Pointer to an Element object.
 */
void RCA_TraverseBSPtree(SDL_Surface *screen, BSPtree *bsptree, Element *element)
{
  if(bsptree == NULL)
    return;	
	
  /* check if we have a valid BSPtree object */
  RCA_CheckBSPtree(bsptree);	
	
  int location;
  
  location = RCA_FindLocationInBSPtree(bsptree, element);
 
  if (location > 0)      /* if element in front of location */
  {
    RCA_TraverseBSPtree(screen, bsptree->back, element);
    RCA_Draw3D(screen, element, bsptree->sector);
    RCA_TraverseBSPtree(screen, bsptree->front, element);
  }
  else if(location < 0) /* eye behind location */
  {
    RCA_TraverseBSPtree(screen, bsptree->front, element);
    RCA_Draw3D(screen, element, bsptree->sector);
    RCA_TraverseBSPtree(screen, bsptree->back, element);
  }
  else                  /* eye coincidental with partition hyperplane */
  {
    RCA_TraverseBSPtree(screen, bsptree->front, element);
    RCA_TraverseBSPtree(screen, bsptree->back, element);
  }
}

#endif