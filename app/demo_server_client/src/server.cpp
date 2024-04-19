#include <rclcpp/rclcpp.hpp>
#include "demo_interface/srv/add_two_int.hpp"

class Server : public rclcpp::Node
{
    public:
        Server(std::string name) : Node(name)
        {
            RCLCPP_INFO(this->get_logger(), "Server is running.");

            /* 创建服务器 */
            p_server = this->create_service<demo_interface::srv::AddTwoInt>(
                "Server",
                std::bind(&Server::server_cbk, this, std::placeholders::_1, std::placeholders::_2));
        }
        ~Server()
        {
            RCLCPP_INFO(this->get_logger(), "Server is ending.");
        }
    
    private:
        void server_cbk(const demo_interface::srv::AddTwoInt::Request::SharedPtr request,
                        const demo_interface::srv::AddTwoInt::Response::SharedPtr response
        )
        {
            RCLCPP_INFO(this->get_logger(), "Server is recving request.");
            response->sum = request->num1 + request->num2;
        }

    private:
        rclcpp::Service<demo_interface::srv::AddTwoInt>::SharedPtr p_server;
};


int main(int argc, char **argv)
{
    /* 初始化 */
    rclcpp::init(argc, argv);

    /* 创建节点 */
    auto node = std::make_shared<Server>("Server01");

    /* 循环 */
    rclcpp::spin(node);

    /* 停止 */
    rclcpp::shutdown();

    return 0;
}