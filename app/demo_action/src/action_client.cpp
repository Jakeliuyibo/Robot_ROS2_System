#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include "demo_interface/action/progress.hpp"

class ActionClient : public rclcpp::Node
{
    using action_type = demo_interface::action::Progress;

    public:
        ActionClient(std::string name) : Node(name)
        {
            RCLCPP_INFO(this->get_logger(), "Action Client is running.");

            /* 创建客户端 */
            p_client = rclcpp_action::create_client<action_type>(
                this,
                "get_sum"
            );

            /* 连接服务器 */
            if (!p_client->wait_for_action_server(std::chrono::seconds(10)))
            {
                RCLCPP_ERROR(this->get_logger(), "连接服务器失败.");
                return;
            }

            /* 发送消息 */
            action_type::Goal goal;
            rclcpp_action::Client<action_type>::SendGoalOptions goal_opt;
            goal.num = 10;
            goal_opt.goal_response_callback = std::bind(&ActionClient::goal_response_handle, this, std::placeholders::_1);
            goal_opt.feedback_callback      = std::bind(&ActionClient::feedback_handle, this, std::placeholders::_1, std::placeholders::_2);
            goal_opt.result_callback        = std::bind(&ActionClient::result_handle, this, std::placeholders::_1);
            p_client->async_send_goal(goal, goal_opt);
        }
        ~ActionClient()
        {
            RCLCPP_INFO(this->get_logger(), "Action Client is ending.");
        }
    
    private:

        void goal_response_handle(
            rclcpp_action::ClientGoalHandle<action_type>::SharedPtr goal_handle
            )
        {
            if (!goal_handle)
            {
                RCLCPP_INFO(this->get_logger(), "目标请求被拒绝.");
            }
            else
            {
                RCLCPP_INFO(this->get_logger(), "目标请求正在被处理中.");
            }
        }

        void feedback_handle(
            rclcpp_action::ClientGoalHandle<action_type>::SharedPtr goal_handle,
            const std::shared_ptr<const action_type::Feedback> feed_back
            )
        {
            (void) goal_handle;

            double progress = feed_back->progress;
            RCLCPP_INFO(this->get_logger(), "目标处理进程%.2f.", progress);
        }

        void result_handle(
            const rclcpp_action::ClientGoalHandle<action_type>::WrappedResult &result
            )
        {
            switch(result.code)
            {
                case rclcpp_action::ResultCode::SUCCEEDED :
                {
                    RCLCPP_INFO(this->get_logger(), "执行成功，结果为%ld", result.result->sum);
                    break;
                }
                case rclcpp_action::ResultCode::ABORTED :
                {
                    RCLCPP_ERROR(this->get_logger(), "执行异常");
                    break;
                }
                case rclcpp_action::ResultCode::CANCELED :
                {
                    RCLCPP_ERROR(this->get_logger(), "执行被取消");
                    break;
                }
                case rclcpp_action::ResultCode::UNKNOWN :
                default:
                {
                    RCLCPP_ERROR(this->get_logger(), "执行遇到未知错误");
                    break;
                }
            }
        }

    private:
        rclcpp_action::Client<action_type>::SharedPtr p_client;
};


int main(int argc, char **argv)
{
    /* 初始化 */
    rclcpp::init(argc, argv);

    /* 创建节点 */
    auto node = std::make_shared<ActionClient>("Client01");

    /* 循环 */
    rclcpp::spin(node);

    /* 停止 */
    rclcpp::shutdown();

    return 0;
}