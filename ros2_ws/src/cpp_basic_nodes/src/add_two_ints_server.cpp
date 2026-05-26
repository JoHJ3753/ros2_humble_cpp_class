// =========================================================
// add_two_ints_server.cpp
// 목적:
//   - ROS2 C++ Service Server의 기본 구조를 이해한다.
//   - /add_two_ints Service를 제공한다.
//   - Client가 a, b 값을 요청하면 sum = a + b를 응답한다.
// =========================================================

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

// ---------------------------------------------------------
// AddTwoIntsServer 클래스
// rclcpp::Node를 상속받아 Service Server 노드로 동작한다.
// ---------------------------------------------------------
class AddTwoIntsServer : public rclcpp::Node
{
public:
    AddTwoIntsServer() : Node("cpp_add_two_ints_server")
    {
        // -------------------------------------------------
        // Service Server 생성
        // Service 이름: add_two_ints
        // Service 타입: example_interfaces/srv/AddTwoInts
        // 요청이 들어오면 handle_service 함수 실행
        // -------------------------------------------------
        service_ = this->create_service<example_interfaces::srv::AddTwoInts>(
            "add_two_ints",
            std::bind(
                &AddTwoIntsServer::handle_service,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );

        RCLCPP_INFO(this->get_logger(), "AddTwoInts Service Server has started.");
    }

private:
    // -----------------------------------------------------
    // Service 요청이 들어왔을 때 실행되는 콜백 함수
    // request  : Client가 보낸 요청 데이터
    // response : Server가 Client에게 돌려줄 응답 데이터
    // -----------------------------------------------------
    void handle_service(
        const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,
        std::shared_ptr<example_interfaces::srv::AddTwoInts::Response> response)
    {
        response->sum = request->a + request->b;

        RCLCPP_INFO(
            this->get_logger(),
            "Request received: a=%ld, b=%ld, sum=%ld",
            request->a,
            request->b,
            response->sum
        );
    }

    // Service Server 객체
    rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service_;
};

// ---------------------------------------------------------
// main 함수
// ---------------------------------------------------------
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<AddTwoIntsServer>();

    rclcpp::spin(node);

    rclcpp::shutdown();

    return 0;
}