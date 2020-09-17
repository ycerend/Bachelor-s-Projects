/*Project 5
  3/29/2018
  Group 7
  Hannah Phillips, Travis Barnett and Yagmur Dardagan

  Coder: Travis Barnett
*/

/*it declares states for finite state machine 
 * 
 * For project 5, previous states have been temporarily commented out.
 */
typedef enum {
  P5_START,
  STATE_RAMP_CENTER_UP_P5,
  WAIT_P5,
  STATE_CENTER_RAMP_DOWN_P5,
  /*STATE_START,
  STATE_CENTER_RAMP_UP,
  STATE_CENTER_RAMP_DOWN,
  STATE_LEFT_RAMP_UP,
  STATE_LEFT_RAMP_DOWN,
  STATE_LEFT_RAMP_STOP,
  STATE_BACK_RAMP_UP,
  STATE_BACK_RAMP_DOWN,
  STATE_BACK_RAMP_STOP,
  STATE_RIGHT_RAMP_UP,
  STATE_RIGHT_RAMP_DOWN,
  STATE_RIGHT_RAMP_STOP,
  WAIT,*/

} State;
