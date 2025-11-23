# Development of UAV

This branch contains runtime code for the UAV.

## External Dependencies

Flight controller takes advantage of the SLAM runtime [`rmcs_slam`](https://Alliance-Algorithm/rmcs_slam).
Remember to sync the submodule.

To use gazebo simulation, you have to install ROS's gazebo port:
```bash
sudo apt install ros-jazzy-ros-gz
```
Or you cannot found `gz gui` subcommand.
