// =========================================================
// add_two_ints_client.cpp
// 목적:
//   - ROS2 C++ Service Client의 기본 구조를 이해한다.
//   - /add_two_ints Service Server에 a, b 값을 요청한다.
//   - Server가 응답한 sum 값을 출력한다.
// =========================================================

#include <chrono>
#include <cstdlib>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using namespace std::chrono_literals;

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    // -----------------------------------------------------
    // 명령행 인자 확인
    // 예:
    //   ros2 run cpp_basic_nodes add_two_ints_client 3 5
    // -----------------------------------------------------
    if (argc != 3)
    {
        RCLCPP_ERROR(
            rclcpp::get_logger("cpp_add_two_ints_client"),
            "Usage: add_two_ints_client <a> <b>"
        );

        rclcpp::shutdown();
        return 1;
    }

    // -----------------------------------------------------
    // Client 노드 생성
    // -----------------------------------------------------
    auto node = rclcpp::Node::make_shared("cpp_add_two_ints_client");

    // -----------------------------------------------------
    // Service Client 생성
    // Service 이름은 Server와 동일해야 한다.
    // -----------------------------------------------------
    auto client = node->create_client<example_interfaces::srv::AddTwoInts>(
        "add_two_ints"
    );

    // -----------------------------------------------------
    // 요청 객체 생성
    // -----------------------------------------------------
    auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();

    request->a = std::atoll(argv[1]);
    request->b = std::atoll(argv[2]);

    // -----------------------------------------------------
    // Service Server가 준비될 때까지 대기
    // -----------------------------------------------------
    while (!client->wait_for_service(1s))
    {
        if (!rclcpp::ok())
        {
            RCLCPP_ERROR(
                rclcpp::get_logger("cpp_add_two_ints_client"),
                "Interrupted while waiting for the service."
            );

            rclcpp::shutdown();
            return 1;
        }

        RCLCPP_INFO(
            rclcpp::get_logger("cpp_add_two_ints_client"),
            "Service not available, waiting again..."
        );
    }

    // -----------------------------------------------------
    // 비동기 요청 전송
    // -----------------------------------------------------
    auto result_future = client->async_send_request(request);

    // -----------------------------------------------------
    // 응답이 올 때까지 대기
    // -----------------------------------------------------
    if (rclcpp::spin_until_future_complete(node, result_future) ==
        rclcpp::FutureReturnCode::SUCCESS)
    {
        auto response = result_future.get();

        RCLCPP_INFO(
            rclcpp::get_logger("cpp_add_two_ints_client"),
            "Result: %ld + %ld = %ld",
            request->a,
            request->b,
            response->sum
        );
    }
    else
    {
        RCLCPP_ERROR(
            rclcpp::get_logger("cpp_add_two_ints_client"),
            "Failed to call service add_two_ints"
        );

        rclcpp::shutdown();
        return 1;
    }

    rclcpp::shutdown();

    return 0;
}