/*Project 4
  3/16/2018
  Group 7
  Hannah Phillips, Travis Barnett and Yagmur Dardagan

  Coder: Yagmur Dardagan
*/

/*it declares states for finite state machine

*/
typedef enum {
  STATE_START,
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
  WAIT,

} State;
