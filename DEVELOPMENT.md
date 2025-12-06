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

## Build

```bash
build-rmcs --packages-skip microxrcedds_agent 
```

## Run

```bash
ros2 launch realsense2_camera rs_launch.py \
    enable_rgbd:=true enable_sync:=true \
    align_depth.enable:=true enable_color:=true enable_depth:=true \
    enable_gyro:=true enable_accel:=true unite_imu_method:=2 \
    pointcloud.enable:=true
```

```bash
ros2 launch rmcs_bringup camera-imu.launch.py
```

```bash
ros2 launch rtabmap_launch rtabmap.launch.py \
    rtabmap_viz:=false rtabmap_args:="--delete_db_on_start --Odom/Strategy 1 --Vis/CorType 1 --Vis/MinInliers 10 --Vis/MaxFeatures 600" \
    frame_id:=camera_link \
    rgb_topic:=/camera/camera/color/image_raw \
    depth_topic:=/camera/camera/aligned_depth_to_color/image_raw \
    camera_info_topic:=/camera/camera/color/camera_info \
    approx_sync:=true \
    sync_queue_size:=20 \
    guess_min_translation:=0.01 \
    guess_min_rotation:=0.01 \
    imu_topic:=/imu/data \
    wait_imu_to_init:=true
```

```bash
cd /workspaces/RMCS/rmcs_ws/thirdparty/micro-xrce-dds-agent/build && ./MicroXRCEAgent serial -b 921600 -D /dev/ttyACM0
```

```bash
ros2 launch foxglove_bridge foxglove_bridge_launch.xml
```

## Debug

Inspect:
```bash
listener aux_global_position
```
