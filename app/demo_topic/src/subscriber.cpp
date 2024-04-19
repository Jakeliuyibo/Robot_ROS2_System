#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "demo_interface/msg/person_info.hpp"

class Subscriber : public rclcpp::Node
{
    public:
        Subscriber(std::string name) : Node(name)
        {
            RCLCPP_INFO(this->get_logger(), "Subscriber is running.");

            /* 1、创建订阅者 */
            p_sub = this->create_subscription<demo_interface::msg::PersonInfo>("PersonInfo", 10, std::bind(&Subscriber::_recv_msg_cbk, this, std::placeholders::_1));
        }
        ~Subscriber()
        {
            RCLCPP_INFO(this->get_logger(), "Subscriber is ending.");
        }
    
    private:
        void _recv_msg_cbk(const demo_interface::msg::PersonInfo::SharedPtr pInfo)
        {
            RCLCPP_INFO(this->get_logger(), "Subscriber is recving msg: name:%s, age:%ld", (pInfo->name).c_str(), pInfo->age);
        }
    
    private:
        rclcpp::Subscription<demo_interface::msg::PersonInfo>::SharedPtr p_sub;
        
};

int main(int argc, char **argv)
{
    /* 初始化 */
    rclcpp::init(argc, argv);

    /* 创建节点 */
    auto node = std::make_shared<Subscriber>("Sub01");

    /* 循环 */
    rclcpp::spin(node);

    /* 停止 */
    rclcpp::shutdown();

    return 0;
}