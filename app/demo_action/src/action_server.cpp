#include <rclcpp/rclcpp.hpp>
#include <rclcpp_action/rclcpp_action.hpp>
#include "demo_interface/action/progress.hpp"

class ActionServer : public rclcpp::Node
{
    public:

        ActionServer(std::string name) : Node(name)
        {
            RCLCPP_INFO(this->get_logger(), "Action Server is running.");

            /* 创建服务器 */
            p_server = rclcpp_action::create_server<demo_interface::action::Progress>(
                this,
                "get_sum",
                std::bind(&ActionServer::handle_goal, this, std::placeholders::_1, std::placeholders::_2),
                std::bind(&ActionServer::handle_cancel, this, std::placeholders::_1),
                std::bind(&ActionServer::handle_accepted, this, std::placeholders::_1)
            );
        }

        ~ActionServer()
        {
            RCLCPP_INFO(this->get_logger(), "Action Server is ending.");
        }

    private:

        // using GoalCallback = std::function<GoalResponse(const GoalUUID &, std::shared_ptr<const typename ActionT::Goal>)>;
        rclcpp_action::GoalResponse handle_goal(const rclcpp_action::GoalUUID &uuid, std::shared_ptr<const demo_interface::action::Progress::Goal> goal)
        {
            (void) uuid;

            if (goal->num <= 1)
            {
                RCLCPP_INFO(this->get_logger(), "num必须大于1.");
                return rclcpp_action::GoalResponse::REJECT;
            }

            RCLCPP_INFO(this->get_logger(), "成功接收数据 num:%ld.", goal->num);
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }

        // using CancelCallback = std::function<CancelResponse(std::shared_ptr<ServerGoalHandle<ActionT>>)>;
        rclcpp_action::CancelResponse handle_cancel(std::shared_ptr<rclcpp_action::ServerGoalHandle<demo_interface::action::Progress>> goal_handle)
        {
            (void) goal_handle;

            RCLCPP_INFO(this->get_logger(), "取消请求.");
            return rclcpp_action::CancelResponse::ACCEPT;
        }

        // using AcceptedCallback = std::function<void (std::shared_ptr<ServerGoalHandle<ActionT>>)>;
        void handle_accepted(std::shared_ptr<rclcpp_action::ServerGoalHandle<demo_interface::action::Progress>> goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "处理ACCEPTED.");

            std::thread(

                [this, goal_handle] ()
                {
                    auto feedback = std::make_shared<demo_interface::action::Progress::Feedback>();
                    auto result = std::make_shared<demo_interface::action::Progress::Result>();
                    int num = goal_handle->get_goal()->num;
                    int sum = 0;
                    rclcpp::Rate rate(1);

                    for (int idx=1; idx <= num; idx++)
                    {
                        sum += idx;
                        double progress = idx / (double) num;
                        feedback->progress = progress;
                        goal_handle->publish_feedback(feedback);
                        RCLCPP_INFO(this->get_logger(), "连续反馈中，进度%.2f.", progress);

                        if (goal_handle->is_canceling())
                        {
                            result->sum = sum;
                            goal_handle->canceled(result);
                            RCLCPP_INFO(this->get_logger(), "Action Server 取消请求");
                            return;
                        }

                        rate.sleep();
                    }

                    // 生成最终结果
                    if (rclcpp::ok())
                    {
                        result->sum = sum;
                        goal_handle->succeed(result);
                    }
                }

            ).detach();
        }

    private:
        rclcpp_action::Server<demo_interface::action::Progress>::SharedPtr p_server;
};

int main(int argc, char **argv)
{
    /* 初始化 */
    rclcpp::init(argc, argv);

    /* 创建节点 */
    auto node = std::make_shared<ActionServer>("Server01");

    /* 循环 */
    rclcpp::spin(node);

    /* 停止 */
    rclcpp::shutdown();

    return 0;
}