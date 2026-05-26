// =========================================================
// simple_node.cpp
// 목적:
//   - ROS2 C++ 노드의 가장 기본 구조를 이해한다.
//   - Publisher/Subscriber 없이 노드 실행과 로그 출력만 확인한다.
// =========================================================

#include "rclcpp/rclcpp.hpp"

// ---------------------------------------------------------
// SimpleNode 클래스
// rclcpp::Node를 상속받아 ROS2 노드로 동작하게 만든다.
// ---------------------------------------------------------
class SimpleNode : public rclcpp::Node
{
public:
    // 생성자
    // Node("simple_node")는 ROS2 그래프에 표시될 노드 이름이다.
    SimpleNode() : Node("simple_node")
    {
        RCLCPP_INFO(this->get_logger(), "Simple ROS2 C++ node has started.");
    }
};

// ---------------------------------------------------------
// main 함수
// 모든 C++ 프로그램의 시작점이다.
// ---------------------------------------------------------
int main(int argc, char * argv[])
{
    // ROS2 클라이언트 라이브러리 초기화
    rclcpp::init(argc, argv);

    // SimpleNode 객체 생성
    auto node = std::make_shared<SimpleNode>();

    // 노드 실행
    // spin은 노드가 종료되지 않고 이벤트를 기다리게 한다.
    rclcpp::spin(node);

    // ROS2 종료 처리
    rclcpp::shutdown();

    return 0;
}