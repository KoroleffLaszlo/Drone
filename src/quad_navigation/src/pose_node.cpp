#include <chrono>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

using namespace std::chrono_literals;

#define TIMER_MACRO 100ms

class DroneStateNode : public rclcpp::Node
{
  public:
    DroneStateNode()
      : Node("drone_state_node")
      {
        velocity_subscriber_ = 
          this->create_subscription<geometry_msgs::msg::Twist>(
            "/drone/cmd_vel",
            10,
            [this](const geometry_msgs::msg::Twist::SharedPtr msg)
              {
                velocity_callback(msg);
              }
          );
        
        pose_publisher_ = 
          this->create_publisher<geometry_msgs::msg::PoseStamped>(
            "/drone/pose",
            10
          );

        timer_ = this->create_wall_timer(
          TIMER_MACRO,
          [this]()
            {
              update_position();
            }
        );

        RCLCPP_INFO(
          this->get_logger(),
          "[INFO] Drone State Node Running..."
        );
      }
   
  private:
      void velocity_callback(
        const geometry_msgs::msg::Twist::SharedPtr msg)
      {
        vx_ = msg->linear.x;
        vy_ = msg->linear.y;
        vz_ = msg->linear.z;
      }

      void update_position()
      {
        constexpr double dt = 0.1;

        x_ += vx_ * dt;
        y_ += vy_ * dt;
        // z_ += vz_ * dt;
        z_ = vz_;
        // posestamped info for transformation (tf2) frame in rvis2 testing
        geometry_msgs::msg::PoseStamped obj;

        obj.header.stamp = this->get_clock()->now();
        obj.header.frame_id = "world";

        // changing position of 'drone' 
        obj.pose.position.x = x_;
        obj.pose.position.y = y_;
        obj.pose.position.z = z_;

        // neutral orientational position
        obj.pose.orientation.x = 0;
        obj.pose.orientation.y = 0;
        obj.pose.orientation.z = 0;
        obj.pose.orientation.w = 1;

        pose_publisher_->publish(obj);
      }

      double x_{};
      double y_{};
      double z_{};

      double vx_{};
      double vy_{};
      double vz_{};      

      rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr velocity_subscriber_;

      rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_publisher_;

      rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<DroneStateNode>();

  rclcpp::spin(node);
  rclcpp::shutdown();
  
  return 0;
}
