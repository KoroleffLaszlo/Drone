#include <chrono>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2/LinearMath/Quaternion.hpp"
#include "tf2_ros/transform_broadcaster.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"

using namespace std::chrono_literals;

class TfBroadcastNode: public rclcpp::Node
{
    public:
        TfBroadcastNode()
            : Node("tf_broadcast_node")
            {
                tf_broadcaster_ = 
                    std::make_unique<tf2_ros::TransformBroadcaster>(*this);

                pose_subscriber_ = 
                this->create_subscription<geometry_msgs::msg::PoseStamped>(
                    "/drone/pose",
                    10,
                    [this](const geometry_msgs::msg::PoseStamped::SharedPtr msg)
                        {
                            pose_callback(msg);
                        }
                    );
            }

    private:
        void pose_callback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
        {
            geometry_msgs::msg::TransformStamped tf;

            tf.header.stamp = msg->header.stamp;

            // hard coded frame ids can change later
            tf.header.frame_id = "world";
            //tf.child_frame_id = "drone";
            tf.child_frame_id = "body_link"; // for urdf model vizualization

            tf.transform.translation.x = msg->pose.position.x;
            tf.transform.translation.y = msg->pose.position.y;
            tf.transform.translation.z = msg->pose.position.z;

            tf.transform.rotation = msg->pose.orientation;

            tf_broadcaster_->sendTransform(tf);
        }

        std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr pose_subscriber_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<TfBroadcastNode>();

  rclcpp::spin(node);
  rclcpp::shutdown();
  
  return 0;
}
