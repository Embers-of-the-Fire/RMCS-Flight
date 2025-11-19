ros2 launch rtabmap_launch rtabmap.launch.py \
    rtabmap_viz:=false rtabmap_args:="--delete_db_on_start --Vis/MinInliers 10 --Vis/MaxFeatures 600" \
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