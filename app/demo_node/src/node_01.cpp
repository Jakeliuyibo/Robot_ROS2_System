#include "rclcpp/rclcpp.hpp"
#include <string>

class Node01 : public rclcpp::Node
{
    public:
        Node01(std::string name) : Node(name)
        {
            RCLCPP_INFO(this->get_logger(), "node 01 is running.");
        }
        ~Node01()
        {
            RCLCPP_INFO(this->get_logger(), "node 01 is ending.");
        }
};

int main(int argc, char **argv)
{
    /* 初始化 */
    rclcpp::init(argc, argv);

    /* 创建节点 */
    auto node = std::make_shared<Node01>("node01");

    /* 循环 */
    rclcpp::spin(node);

    /* 停止 */
    rclcpp::shutdown();

    return 0;
}