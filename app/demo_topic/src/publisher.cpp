#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Publisher : public rclcpp::Node
{
    public:
        Publisher(std::string name) : Node(name)
        {
            RCLCPP_INFO(this->get_logger(), "Publisher is running.");

            /* 1、创建发布者 */
            p_pub = this->create_publisher<std_msgs::msg::String>("name", 10);

            /* 2、创建定时器发布消息 */
            p_timer = this->create_wall_timer(std::chrono::milliseconds(1000), std::bind(&Publisher::_send_msg, this));

        }
        ~Publisher()
        {
            RCLCPP_INFO(this->get_logger(), "Publisher is ending.");
        }
    
    private:
        void _send_msg()
        {
            std_msgs::msg::String name;
            name.data = "hello msg.";

            RCLCPP_INFO(this->get_logger(), "Publisher is sendling msg: %s.", name.data.c_str());

            p_pub->publish(name);
        }
    
    private:
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr p_pub;
        rclcpp::TimerBase::SharedPtr p_timer;
};

int main(int argc, char **argv)
{
    /* 初始化 */
    rclcpp::init(argc, argv);

    /* 创建节点 */
    auto node = std::make_shared<Publisher>("Pub01");

    /* 循环 */
    rclcpp::spin(node);

    /* 停止 */
    rclcpp::shutdown();

    return 0;
}