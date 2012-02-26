/**
 * @author Sebastien Bolduc <sebastien.bolduc@gmail.com>
 * @version 1.00
 * @since 2012-02-24
 */
 
#ifndef RCA_KEYBOARD_H_
#define RCA_KEYBOARD_H_

int RCA_KEYBOARD__SDLK_UP_FLAG = 0;
int RCA_KEYBOARD__SDLK_DOWN_FLAG = 0;
int RCA_KEYBOARD__SDLK_RIGHT_FLAG = 0;
int RCA_KEYBOARD__SDLK_LEFT_FLAG = 0;
int RCA_KEYBOARD__SDLK_m_FLAG = 0;

/**
 * Set the flag of a keyboard's key.
 * 
 * @param key   Key stroke on the keyboard.
 * @param value Value to affecte to the flag.
 */
void RCA_SetKeyboardFlag(SDLKey key, int value)
{
  switch (key)
  {
	case SDLK_UP:
	  RCA_KEYBOARD__SDLK_UP_FLAG = value;
	  break;
	case SDLK_DOWN:
	  RCA_KEYBOARD__SDLK_DOWN_FLAG = value;
	  break;
	case SDLK_RIGHT:
	  RCA_KEYBOARD__SDLK_RIGHT_FLAG = value;
	  break;
	case SDLK_LEFT:
	  RCA_KEYBOARD__SDLK_LEFT_FLAG = value;
	  break;
	case SDLK_m:
	  RCA_KEYBOARD__SDLK_m_FLAG = value;
	  break;
	default:
	  break;
  }
}

/**
 * Poll event related to keyboard.
 * 
 * Poll for event happening with the keyboard and set 'off' or
 * 'on' the flag for the key stroke.
 * 
 * @param event SDL_Event typedef.
 */
void RCA_PollKeyboardEvent(SDL_Event *event)
{
  if (event->type == SDL_KEYDOWN)
  {
	RCA_SetKeyboardFlag(event->key.keysym.sym, 1);
  }
  if (event->type == SDL_KEYUP)
  {
	RCA_SetKeyboardFlag(event->key.keysym.sym, 0);
  }
}

/**
 * Check the flag of a key.
 * 
 * @param key Key to check for.
 * @return    Value of the key's flag.
 */
int RCA_CheckKeyboardKey(SDLKey key)
{
  switch (key)
  {
	case SDLK_UP:
	  return RCA_KEYBOARD__SDLK_UP_FLAG;
	  break;
	case SDLK_DOWN:
	  return RCA_KEYBOARD__SDLK_DOWN_FLAG;
	  break;
	case SDLK_RIGHT:
	  return RCA_KEYBOARD__SDLK_RIGHT_FLAG;
	  break;
	case SDLK_LEFT:
	  return RCA_KEYBOARD__SDLK_LEFT_FLAG;
	  break;
	case SDLK_m:
	  return RCA_KEYBOARD__SDLK_m_FLAG;
	  break;
	default:
	  return 0;
	  break;
  }
}

#endif