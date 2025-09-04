#ifndef TV_IO_HPP
#define TV_IO_HPP

class tv_io {
public:
  // steer
  // sideslip
  // velocity
  // etc. inputs to the algorithm
  float steer;
  float sideslip;
  float velocity;

  // motor limits
  // traction limits
  // desired yaw moment 
  //
  // wheel torques 
  // fx in corner frame
  // fy in corner frame 
  // mz in corner frame
  tv_io(){
    steer = 0;
    sideslip = 0;
    velocity = 0;
  }
};

#endif
