# =========================================================
# pubsub.launch.py
# 목적:
#   - cpp_talker와 cpp_listener를 동시에 실행한다.
#   - /cpp_chatter Topic 통신을 launch로 확인한다.
# =========================================================

from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        # -------------------------------------------------
        # C++ Publisher 노드 실행
        # ros2 run cpp_basic_nodes cpp_talker 와 같은 의미
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='cpp_talker',
            name='cpp_talker',
            output='screen'
        ),

        # -------------------------------------------------
        # C++ Subscriber 노드 실행
        # ros2 run cpp_basic_nodes cpp_listener 와 같은 의미
        # -------------------------------------------------
        Node(
            package='cpp_basic_nodes',
            executable='cpp_listener',
            name='cpp_listener',
            output='screen'
        ),
    ])