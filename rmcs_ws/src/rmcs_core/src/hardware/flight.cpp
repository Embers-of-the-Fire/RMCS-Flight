#include <nav_msgs/msg/odometry.hpp>
#include <px4_ros2/navigation/experimental/local_position_measurement_interface.hpp>
#include <rclcpp/node.hpp>
#include <rmcs_executor/component.hpp>
#include <std_msgs/msg/int32.hpp>

namespace rmcs_core::hardware {

class Flight
    : public rmcs_executor::Component
    , public rclcpp::Node {
public:
    Flight()
        : Node{
              get_component_name(),
              rclcpp::NodeOptions{}.automatically_declare_parameters_from_overrides(true)}
        , local_position_interface_{*this, px4_ros2::PoseFrame::LocalNED, px4_ros2::VelocityFrame::LocalNED}
        , logger_(get_logger()) {
        if (!local_position_interface_.doRegister()) {
            throw std::runtime_error("Failed to register LocalPositionMeasurementInterface");
        } else {
            RCLCPP_INFO(logger_, "LocalPositionMeasurementInterface registered successfully.");
        }
        subscription_ = create_subscription<nav_msgs::msg::Odometry>(
            "/rtabmap/odom", rclcpp::QoS{rclcpp::KeepLast(10)}.reliable(),
            // NOLINTNEXTLINE False positive. if fixed you'll get a messy template error.
            [this](const nav_msgs::msg::Odometry::SharedPtr msg) { update_position(msg); });
    }

    void update() override {}

private:
    void update_position(const nav_msgs::msg::Odometry::SharedPtr& msg) {
        px4_ros2::LocalPositionMeasurement local_position_measurement{};

        local_position_measurement.timestamp_sample = get_clock()->now();

        local_position_measurement.velocity_xy = Eigen::Vector2f{
            static_cast<float>(msg->twist.twist.linear.x),
            static_cast<float>(msg->twist.twist.linear.y)};
        local_position_measurement.velocity_xy_variance = Eigen::Vector2f{0.3f, 0.4f};

        local_position_measurement.position_z = msg->pose.pose.position.z;
        local_position_measurement.position_z_variance = 0.33F;

        local_position_measurement.attitude_quaternion = Eigen::Quaternionf{
            static_cast<float>(msg->pose.pose.orientation.w),
            static_cast<float>(msg->pose.pose.orientation.x),
            static_cast<float>(msg->pose.pose.orientation.y),
            static_cast<float>(msg->pose.pose.orientation.z)};
        local_position_measurement.attitude_variance = Eigen::Vector3f{0.2, 0.1, 0.05};

        RCLCPP_INFO(
            logger_, "Updating position with velocity xy: %f, %f",
            static_cast<float>(msg->twist.twist.linear.x),
            static_cast<float>(msg->twist.twist.linear.y));

        try {
            local_position_interface_.update(local_position_measurement);
            RCLCPP_INFO(logger_, "Successfully sent position update to navigation interface.");
        } catch (const px4_ros2::NavigationInterfaceInvalidArgument& e) {
            RCLCPP_ERROR_THROTTLE(logger_, *get_clock(), 1000, "Exception caught: %s", e.what());
        }
    }

private:
    px4_ros2::LocalPositionMeasurementInterface local_position_interface_;

    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscription_;
    rclcpp::Logger logger_;
};

} // namespace rmcs_core::hardware

#include <pluginlib/class_list_macros.hpp>
PLUGINLIB_EXPORT_CLASS(rmcs_core::hardware::Flight, rmcs_executor::Component)
