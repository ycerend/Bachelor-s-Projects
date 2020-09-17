/*Project 8
  4/19/2018
  Group 7
  Hannah Phillips, Travis Barnett and Yagmur Dardagan

  Coder: Hannah Phillips
*/

/*it declares states for finite state machine 
 * 
 * For project 5 and 7, previous states have been temporarily commented out.
 * This is the FSM for Project 8.
 */
typedef enum {
  
  P5_START,
  STATE_RAMP_CENTER_UP_P5,
  STATE_HOVER_P15,
  STATE_HOVER_P10,
  STATE_VELOCITY_GOAL_X,
  STATE_VELOCITY_GOAL_Y,
  STATE_CENTER_RAMP_DOWN,
  /*
   * Project 7 finite state machine
   */
   /*P5_START,
  STATE_RAMP_CENTER_UP_P5,
  WAIT_P5,
  STATE_CENTER_RAMP_DOWN_P5,*/
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
