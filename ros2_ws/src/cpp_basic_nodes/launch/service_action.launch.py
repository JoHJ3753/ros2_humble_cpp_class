# =========================================================
# service_action.launch.py
# 목적:
#   - add_two_ints_server와 fibonacci_action_server를 동시에 실행한다.
#   - Service와 Action Server를 한 번에 준비한다.
# =========================================================

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        # -------------------------------------------------
        # AddTwoInts Service Server 실행
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='add_two_ints_server',
            name='cpp_add_two_ints_server',
            output='screen'
        ),

        # -------------------------------------------------
        # Fibonacci Action Server 실행
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='fibonacci_action_server',
            name='cpp_fibonacci_action_server',
            output='screen'
        ),
    ])