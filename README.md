## Build 

Using ROS2 Jazzy and `colcon`

Running commands from the root workspace:
```
source /opt/ros/jazzy/setup.bash
colcon build --symlink-install
source install/setup.bash
```

## Simulate 

Using RViz for visual simulation of drone

Running command from the root workspace:
```
ros2 launch quad_navigation drone_tf_demo.xml
```

## Basic Debugging 

For simple debugging of individual nodes

```
ros2 run quad_navigation nav_node
ros2 run quad_navigation pose_node 
```
