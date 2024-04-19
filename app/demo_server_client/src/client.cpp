#include <rclcpp/rclcpp.hpp>
#include "demo_interface/srv/add_two_int.hpp"

class Client : public rclcpp::Node
{
    public:
        Client(std::string name) : Node(name)
        {
            RCLCPP_INFO(this->get_logger(), "Client is running.");

            /* 创建客户端 */
            p_client = this->create_client<demo_interface::srv::AddTwoInt>("Server");

            /* 创建定时器 */
            p_timer = this->create_wall_timer(std::chrono::milliseconds(1000), std::bind(&Client::_request, this));

        }
        ~Client()
        {
            RCLCPP_INFO(this->get_logger(), "Client is ending.");
        }
    
    private:
        void _request()
        {
            /* 等待客户端上线 */
            while (!p_client->wait_for_service(std::chrono::seconds(1)))
            {
                RCLCPP_INFO(this->get_logger(), "Client is waiting server power on.");
            }

            /* 发送请求 */
            auto req = std::make_shared<demo_interface::srv::AddTwoInt_Request>();
            req->num1 = 10;
            req->num2 = 20;

            /* 发送数据 */
            p_client->async_send_request(req, std::bind(&Client::_client_cbk, this, std::placeholders::_1));
            RCLCPP_INFO(this->get_logger(), "Client send num1:%ld, num2:%ld.", req->num1, req->num2);
        }

        void _client_cbk(rclcpp::Client<demo_interface::srv::AddTwoInt>::SharedFuture response)
        {
            auto res = response.get();
            RCLCPP_INFO(this->get_logger(), "Client recv response sum:%ld.", res->sum);
        }

    private:
        rclcpp::Client<demo_interface::srv::AddTwoInt>::SharedPtr p_client;
        rclcpp::TimerBase::SharedPtr p_timer;
};


int main(int argc, char **argv)
{
    /* 初始化 */
    rclcpp::init(argc, argv);

    /* 创建节点 */
    auto node = std::make_shared<Client>("Client01");

    /* 循环 */
    rclcpp::spin(node);

    /* 停止 */
    rclcpp::shutdown();

    return 0;
}