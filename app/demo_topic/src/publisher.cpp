#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "demo_interface/msg/person_info.hpp"

class Publisher : public rclcpp::Node
{
    public:
        Publisher(std::string name) : Node(name)
        {
            RCLCPP_INFO(this->get_logger(), "Publisher is running.");

            /* 1、创建发布者 */
            p_pub = this->create_publisher<demo_interface::msg::PersonInfo>("PersonInfo", 10);

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
            demo_interface::msg::PersonInfo info;
            info.name = "hello msg.";
            info.age = 18;

            RCLCPP_INFO(this->get_logger(), "Publisher is sendling msg: %s.", info.name.c_str());

            p_pub->publish(info);
        }
    
    private:
        rclcpp::Publisher<demo_interface::msg::PersonInfo>::SharedPtr p_pub;
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