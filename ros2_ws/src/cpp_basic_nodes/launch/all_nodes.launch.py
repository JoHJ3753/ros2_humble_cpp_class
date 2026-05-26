# =========================================================
# all_nodes.launch.py
# 목적:
#   - 지금까지 만든 주요 ROS2 C++ 노드를 한 번에 실행한다.
#   - Topic, Service, Action 구조를 동시에 확인한다.
# =========================================================

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        # -------------------------------------------------
        # 단순 노드
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='simple_node',
            name='simple_node',
            output='screen'
        ),

        # -------------------------------------------------
        # Topic Publisher
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='cpp_talker',
            name='cpp_talker',
            output='screen'
        ),

        # -------------------------------------------------
        # Topic Subscriber
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='cpp_listener',
            name='cpp_listener',
            output='screen'
        ),

        # -------------------------------------------------
        # Service Server
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='add_two_ints_server',
            name='cpp_add_two_ints_server',
            output='screen'
        ),

        # -------------------------------------------------
        # Action Server
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='fibonacci_action_server',
            name='cpp_fibonacci_action_server',
            output='screen'
        ),
    ])