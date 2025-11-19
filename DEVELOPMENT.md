# Development of UAV

This branch contains runtime code for the UAV.

## External Dependencies

Flight controller takes advantage of the SLAM runtime [`rmcs_slam`](https://Alliance-Algorithm/rmcs_slam).
Remember to sync the submodule.

## Launch

```bash
# 1. launch camera
./launch_camera.zsh
# 2. launch IMU util
ros2 launch rmcs_bringup camera-imu.launch.py
# 3. launch SLAM
./launch_rtabmap.zsh
```

