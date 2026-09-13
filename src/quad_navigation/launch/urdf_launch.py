from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import Command, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():

    urdf_file = PathJoinSubstitution([
        FindPackageShare("quad_navigation"),
        "urdf",
        "geometry.urdf.xacro"
    ])

    robot_state_publisher = Node(
        package="robot_state_publisher",
        executable="robot_state_publisher",
        parameters=[{
            "robot_description": Command(["xacro ", urdf_file])
        }]
    )

    return LaunchDescription([
        robot_state_publisher
    ])