// =========================================================
// cpp_listener.cpp
// 목적:
//   - ROS2 C++ Subscriber 노드의 기본 구조를 이해한다.
//   - /cpp_chatter 토픽에서 std_msgs/msg/String 메시지를 수신한다.
//   - 메시지가 도착하면 콜백 함수가 자동 실행된다.
// =========================================================

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// ---------------------------------------------------------
// CppListener 클래스
// rclcpp::Node를 상속받아 ROS2 Subscriber 노드로 동작한다.
// ---------------------------------------------------------
class CppListener : public rclcpp::Node
{
public:
    CppListener() : Node("cpp_listener")
    {
        // -------------------------------------------------
        // Subscriber 생성
        // 토픽 이름: cpp_chatter
        // 메시지 타입: std_msgs::msg::String
        // 큐 크기: 10
        // 메시지 도착 시 topic_callback 함수 실행
        // -------------------------------------------------
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "cpp_chatter",
            10,
            std::bind(&CppListener::topic_callback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "CppListener subscriber node has started.");
    }

private:
    // -----------------------------------------------------
    // 메시지가 도착할 때마다 자동 실행되는 콜백 함수
    // -----------------------------------------------------
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const
    {
        RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
    }

    // Subscriber 객체
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

// ---------------------------------------------------------
// main 함수
// ---------------------------------------------------------
int main(int argc, char * argv[])
{
    // ROS2 초기화
    rclcpp::init(argc, argv);

    // CppListener 노드 생성
    auto node = std::make_shared<CppListener>();

    // 노드 실행
    // 메시지가 들어오면 topic_callback이 실행된다.
    rclcpp::spin(node);

    // ROS2 종료
    rclcpp::shutdown();

    return 0;
}