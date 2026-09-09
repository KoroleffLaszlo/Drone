cmake_minimum_required(VERSION 3.8)
project(quad_navigation)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)

add_executable(nav_node src/nav_node.cpp)
target_include_directories(nav_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(nav_node PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(
  nav_node
  "rclcpp"
  "geometry_msgs"
  "std_msgs"
)

add_executable(pose_node src/pose_node.cpp)
target_include_directories(pose_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(pose_node PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(
  pose_node
  "rclcpp"
  "geometry_msgs"
  "std_msgs"
)

add_executable(tf_node src/tf_node.cpp)
target_include_directories(tf_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)
target_compile_features(tf_node PUBLIC c_std_99 cxx_std_17)
ament_target_dependencies(
  tf_node
  "rclcpp"
  "geometry_msgs"
  "std_msgs"
  "tf2"
  "tf2_ros"
)

install(
  TARGETS
    nav_node
    pose_node
    tf_node
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)

  set(ament_cmake_copyright_FOUND TRUE)

  set(ament_cmake_cpplint_FOUND TRUE)

  ament_lint_auto_find_test_dependencies()

  install(
  DIRECTORY 
    launch
    rviz
  DESTINATION share/${PROJECT_NAME}
)
endif()

ament_package()
