#include <chrono>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

#define TIMER_MACRO 100ms

class NavigationNode : public rclcpp::Node
{
  public:
    NavigationNode() 
      : Node("navigation_node")
      {
        velocity_publisher_ =
          this->create_publisher<geometry_msgs::msg::Twist>(
            "/drone/cmd_vel",
            10
          );

        timer_ = this->create_wall_timer(
          TIMER_MACRO,
          [this]() 
            {
              publish_velocity_command();
            }
        );

        RCLCPP_INFO(
          this->get_logger(),
          "[INFO] Navigation Node Running..."
        );
      }

  private:
    void publish_velocity_command()
    {
      geometry_msgs::msg::Twist command;

      // linear velocity for circular trajectory
      command.linear.x = -radius * omega * std::sin(omega * t);
      command.linear.y = radius * omega * std::cos(omega * t);
      // command.linear.z = amplitude * omega * std::cos(omega * t);
      command.linear.z = amplitude;

      command.angular.x = 0.0;
      command.angular.y = 0.0;
      command.angular.z = 0.0;

      velocity_publisher_->publish(command);

      RCLCPP_INFO(
        this->get_logger(),
        "Published velocity command: %.2f | %.2f | %.2f \n", 
        command.linear.x, 
        command.linear.y, 
        command.linear.z
      );

      t += dt;
    }

    static constexpr double dt{0.1};
    static constexpr double radius{0.5}; //meters
    static constexpr double amplitude{5.0};
    static constexpr double altitude{5.0};


    static constexpr double omega{0.5}; // radians/sec

    double t{0.0};

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;

    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{

  rclcpp::init(argc, argv);
  auto node = std::make_shared<NavigationNode>();

  rclcpp::spin(node);
  rclcpp::shutdown();

  return 0;
}
