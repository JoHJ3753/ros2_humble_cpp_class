// =========================================================
// fibonacci_action_server.cpp
// 목적:
//   - ROS2 C++ Action Server의 기본 구조를 이해한다.
//   - /fibonacci Action Server를 제공한다.
//   - Client가 order 값을 보내면 Fibonacci 수열을 계산한다.
//   - 계산 중간마다 Feedback을 보내고, 완료 후 Result를 반환한다.
// =========================================================

#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "example_interfaces/action/fibonacci.hpp"

using namespace std::chrono_literals;

// ---------------------------------------------------------
// FibonacciActionServer 클래스
// ---------------------------------------------------------
class FibonacciActionServer : public rclcpp::Node
{
public:
    using Fibonacci = example_interfaces::action::Fibonacci;
    using GoalHandleFibonacci = rclcpp_action::ServerGoalHandle<Fibonacci>;

    FibonacciActionServer() : Node("cpp_fibonacci_action_server")
    {
        // -------------------------------------------------
        // Action Server 생성
        // Action 이름: fibonacci
        // Goal 처리 콜백: handle_goal
        // Cancel 처리 콜백: handle_cancel
        // Goal 수락 후 처리 콜백: handle_accepted
        // -------------------------------------------------
        action_server_ = rclcpp_action::create_server<Fibonacci>(
            this,
            "fibonacci",
            std::bind(
                &FibonacciActionServer::handle_goal,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            ),
            std::bind(
                &FibonacciActionServer::handle_cancel,
                this,
                std::placeholders::_1
            ),
            std::bind(
                &FibonacciActionServer::handle_accepted,
                this,
                std::placeholders::_1
            )
        );

        RCLCPP_INFO(this->get_logger(), "Fibonacci Action Server has started.");
    }

private:
    rclcpp_action::Server<Fibonacci>::SharedPtr action_server_;

    // -----------------------------------------------------
    // Goal 요청을 받을지 거절할지 판단하는 콜백
    // -----------------------------------------------------
    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID & uuid,
        std::shared_ptr<const Fibonacci::Goal> goal)
    {
        (void)uuid;

        RCLCPP_INFO(
            this->get_logger(),
            "Received goal request: order=%d",
            goal->order
        );

        // 실무에서는 여기서 목표값 유효성 검사를 한다.
        // 예: 목표 좌표가 맵 밖인지, 로봇 모드가 실행 가능한 상태인지 등.
        if (goal->order <= 0)
        {
            RCLCPP_WARN(
                this->get_logger(),
                "Goal rejected. order must be greater than 0."
            );

            return rclcpp_action::GoalResponse::REJECT;
        }

        if (goal->order > 50)
        {
            RCLCPP_WARN(
                this->get_logger(),
                "Goal rejected. order is too large for this training example."
            );

            return rclcpp_action::GoalResponse::REJECT;
        }

        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    // -----------------------------------------------------
    // Cancel 요청을 받을지 판단하는 콜백
    // -----------------------------------------------------
    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleFibonacci> goal_handle)
    {
        (void)goal_handle;

        RCLCPP_INFO(this->get_logger(), "Received request to cancel goal.");

        return rclcpp_action::CancelResponse::ACCEPT;
    }

    // -----------------------------------------------------
    // Goal이 수락되었을 때 실행되는 콜백
    // -----------------------------------------------------
    void handle_accepted(
        const std::shared_ptr<GoalHandleFibonacci> goal_handle)
    {
        // 중요한 실무 포인트:
        // execute를 직접 호출하면 현재 executor 흐름을 오래 잡아먹을 수 있다.
        // 그래서 별도 thread로 실행한다.
        std::thread{
            std::bind(&FibonacciActionServer::execute, this, std::placeholders::_1),
            goal_handle
        }.detach();
    }

    // -----------------------------------------------------
    // 실제 Fibonacci 계산을 수행하는 함수
    // Feedback과 Result를 여기서 보낸다.
    // -----------------------------------------------------
    void execute(
        const std::shared_ptr<GoalHandleFibonacci> goal_handle)
    {
        RCLCPP_INFO(this->get_logger(), "Executing goal...");

        const auto goal = goal_handle->get_goal();

        auto feedback = std::make_shared<Fibonacci::Feedback>();
        auto result = std::make_shared<Fibonacci::Result>();

        // Fibonacci 초기값
        feedback->sequence.push_back(0);
        feedback->sequence.push_back(1);

        rclcpp::Rate loop_rate(1);

        for (int i = 1; i < goal->order; ++i)
        {
            // 취소 요청 확인
            if (goal_handle->is_canceling())
            {
                result->sequence = feedback->sequence;
                goal_handle->canceled(result);

                RCLCPP_WARN(this->get_logger(), "Goal canceled.");

                return;
            }

            // Fibonacci 다음 값 계산
            int32_t next_number =
                feedback->sequence[i] +
                feedback->sequence[i - 1];

            feedback->sequence.push_back(next_number);

            // Feedback 전송
            goal_handle->publish_feedback(feedback);

            RCLCPP_INFO(
                this->get_logger(),
                "Publishing feedback. Current sequence size: %zu",
                feedback->sequence.size()
            );

            loop_rate.sleep();
        }

        // ROS2가 정상 동작 중이면 성공 결과 반환
        if (rclcpp::ok())
        {
            result->sequence = feedback->sequence;
            goal_handle->succeed(result);

            RCLCPP_INFO(this->get_logger(), "Goal succeeded.");
        }
    }
};

// ---------------------------------------------------------
// main 함수
// ---------------------------------------------------------
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<FibonacciActionServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}