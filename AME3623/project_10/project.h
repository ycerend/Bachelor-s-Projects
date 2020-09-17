/*Project 10
  5/3/2018
  Group 7
  Hannah Phillips, Travis Barnett and Yagmur Dardagan

  Coder: Yagmur Dardagan
*/

/*it declares states for finite state machine 
 * 
 * For project 5,7 and 8, previous states have been temporarily commented out.
 * This is the FSM for Project 10.
 */
typedef enum {


  FSM_START_P10,
  RAMP_UP_CENTRAL_FAN_P10,
  MOVE_FORWARD_P10,
  TURN_LEFT_P10,
  TURN_RIGHT_10,
  PHASE_2_P10,
  STATE_C_P10,
  STATE_D_P10,
  STATE_STOPPED_P10,

    /*
   * Project 9 finite state machine
   */

  /*
  STATE_START_P9,
  STATE_WAIT_P9,
  STATE_MIDDLE_FAN_RAMP_UP_P9,
  STATE_MOVE_FORWARD_P9,
  STATE_STOPPING,
  STATE_TURN_LEFT_P9,
  STATE_MOVE_FORWARD_2_P9,
  STATE_STOPPED,
  */

  /*
   * Project 8 finite state machine
   */
  /*P5_START,
  STATE_RAMP_CENTER_UP_P5,
  STATE_HOVER_P15,
  STATE_HOVER_P10,
  STATE_VELOCITY_GOAL_X,
  STATE_VELOCITY_GOAL_Y,
  STATE_CENTER_RAMP_DOWN,*/

  
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


/*It declares the variable distanceSensor and its values.
 */
typedef enum {
  DISTANCE_FRONT_RED = 0, 
  DISTANCE_RIGHT_YELLOW = 1
} DistanceSensor;
