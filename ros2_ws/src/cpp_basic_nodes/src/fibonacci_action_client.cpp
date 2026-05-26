// =========================================================
// fibonacci_action_client.cpp
// 목적:
//   - ROS2 C++ Action Client의 기본 구조를 이해한다.
//   - /fibonacci Action Server에 Goal을 전송한다.
//   - Feedback을 수신해 출력한다.
//   - Result를 수신해 최종 결과를 출력한다.
// =========================================================

#include <chrono>
#include <cstdlib>
#include <future>
#include <memory>
#include <sstream>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using namespace std::chrono_literals;

// ---------------------------------------------------------
// FibonacciActionClient 클래스
// ---------------------------------------------------------
class FibonacciActionClient : public rclcpp::Node
{
public:
    using Fibonacci = example_interfaces::action::Fibonacci;
    using GoalHandleFibonacci = rclcpp_action::ClientGoalHandle<Fibonacci>;

    explicit FibonacciActionClient(int32_t order)
    : Node("cpp_fibonacci_action_client"), order_(order)
    {
        // -------------------------------------------------
        // Action Client 생성
        // Action 이름은 Server의 이름과 같아야 한다.
        // -------------------------------------------------
        action_client_ = rclcpp_action::create_client<Fibonacci>(
            this,
            "fibonacci"
        );

        RCLCPP_INFO(this->get_logger(), "Fibonacci Action Client has started.");
    }

    // -----------------------------------------------------
    // Goal 전송 함수
    // -----------------------------------------------------
    void send_goal()
    {
        if (!action_client_->wait_for_action_server(5s))
        {
            RCLCPP_ERROR(
                this->get_logger(),
                "Action server not available after waiting."
            );

            rclcpp::shutdown();
            return;
        }

        auto goal_msg = Fibonacci::Goal();
        goal_msg.order = order_;

        RCLCPP_INFO(
            this->get_logger(),
            "Sending goal: order=%d",
            goal_msg.order
        );

        auto send_goal_options =
            rclcpp_action::Client<Fibonacci>::SendGoalOptions();

        send_goal_options.goal_response_callback =
            std::bind(
                &FibonacciActionClient::goal_response_callback,
                this,
                std::placeholders::_1
            );

        send_goal_options.feedback_callback =
            std::bind(
                &FibonacciActionClient::feedback_callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            );

        send_goal_options.result_callback =
            std::bind(
                &FibonacciActionClient::result_callback,
                this,
                std::placeholders::_1
            );

        action_client_->async_send_goal(goal_msg, send_goal_options);
    }

private:
    rclcpp_action::Client<Fibonacci>::SharedPtr action_client_;
    int32_t order_;

    // -----------------------------------------------------
    // Goal 수락 여부 콜백
    // -----------------------------------------------------
    void goal_response_callback(
        const GoalHandleFibonacci::SharedPtr & goal_handle)
    {
        if (!goal_handle)
        {
            RCLCPP_ERROR(this->get_logger(), "Goal was rejected by server.");
        }
        else
        {
            RCLCPP_INFO(this->get_logger(), "Goal accepted by server.");
        }
    }

    // -----------------------------------------------------
    // Feedback 수신 콜백
    // -----------------------------------------------------
    void feedback_callback(
        GoalHandleFibonacci::SharedPtr,
        const std::shared_ptr<const Fibonacci::Feedback> feedback)
    {
        RCLCPP_INFO(
            this->get_logger(),
            "Feedback received: %s",
            sequence_to_string(feedback->sequence).c_str()
        );
    }

    // -----------------------------------------------------
    // Result 수신 콜백
    // -----------------------------------------------------
    void result_callback(
        const GoalHandleFibonacci::WrappedResult & result)
    {
        switch (result.code)
        {
            case rclcpp_action::ResultCode::SUCCEEDED:
                RCLCPP_INFO(this->get_logger(), "Goal succeeded.");
                break;

            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(this->get_logger(), "Goal was aborted.");
                rclcpp::shutdown();
                return;

            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_WARN(this->get_logger(), "Goal was canceled.");
                rclcpp::shutdown();
                return;

            default:
                RCLCPP_ERROR(this->get_logger(), "Unknown result code.");
                rclcpp::shutdown();
                return;
        }

        RCLCPP_INFO(
            this->get_logger(),
            "Final result: %s",
            sequence_to_string(result.result->sequence).c_str()
        );

        rclcpp::shutdown();
    }

    // -----------------------------------------------------
    // sequence 벡터를 문자열로 변환하는 유틸리티 함수
    // -----------------------------------------------------
    std::string sequence_to_string(const std::vector<int32_t> & sequence)
    {
        std::stringstream ss;

        ss << "[";

        for (size_t i = 0; i < sequence.size(); ++i)
        {
            ss << sequence[i];

            if (i + 1 < sequence.size())
            {
                ss << ", ";
            }
        }

        ss << "]";

        return ss.str();
    }
};

// ---------------------------------------------------------
// main 함수
// ---------------------------------------------------------
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    int32_t order = 6;

    if (argc >= 2)
    {
        order = std::atoi(argv[1]);
    }

    auto action_client = std::make_shared<FibonacciActionClient>(order);

    action_client->send_goal();

    rclcpp::spin(action_client);

    rclcpp::shutdown();

    return 0;
}