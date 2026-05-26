// =========================================================
// cpp_talker.cpp
// 목적:
//   - ROS2 C++ Publisher 노드의 기본 구조를 이해한다.
//   - Timer를 이용해 1초마다 문자열 메시지를 발행한다.
//   - /cpp_chatter 토픽으로 std_msgs/msg/String 메시지를 보낸다.
// =========================================================

#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

// ---------------------------------------------------------
// CppTalker 클래스
// rclcpp::Node를 상속받아 ROS2 Publisher 노드로 동작한다.
// ---------------------------------------------------------
class CppTalker : public rclcpp::Node
{
public:
    CppTalker() : Node("cpp_talker"), count_(0)
    {
        // -------------------------------------------------
        // Publisher 생성
        // 토픽 이름: cpp_chatter
        // 메시지 타입: std_msgs::msg::String
        // 큐 크기: 10
        // -------------------------------------------------
        publisher_ = this->create_publisher<std_msgs::msg::String>(
            "cpp_chatter",
            10
        );

        // -------------------------------------------------
        // Timer 생성
        // 1초마다 timer_callback 함수 실행
        // -------------------------------------------------
        timer_ = this->create_wall_timer(
            1s,
            std::bind(&CppTalker::timer_callback, this)
        );

        RCLCPP_INFO(this->get_logger(), "CppTalker publisher node has started.");
    }

private:
    // -----------------------------------------------------
    // 1초마다 실행되는 콜백 함수
    // -----------------------------------------------------
    void timer_callback()
    {
        // 메시지 객체 생성
        auto message = std_msgs::msg::String();

        // 메시지 내용 작성
        message.data = "Hello ROS2 C++ Publisher: " + std::to_string(count_++);

        // Topic으로 메시지 발행
        publisher_->publish(message);

        // 로그 출력
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    }

    // Publisher 객체
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    // Timer 객체
    rclcpp::TimerBase::SharedPtr timer_;

    // 발행 횟수 카운터
    size_t count_;
};

// ---------------------------------------------------------
// main 함수
// ---------------------------------------------------------
int main(int argc, char * argv[])
{
    // ROS2 초기화
    rclcpp::init(argc, argv);

    // CppTalker 노드 생성
    auto node = std::make_shared<CppTalker>();

    // 노드 실행
    rclcpp::spin(node);

    // ROS2 종료
    rclcpp::shutdown();

    return 0;
}