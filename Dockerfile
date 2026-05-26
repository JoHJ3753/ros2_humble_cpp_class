# =========================================================
# ROS2 Humble C++ 개발용 Dockerfile
# 기준 이미지: osrf/ros:humble-desktop
# 목적:
#   - ROS2 Humble 기본 환경 제공
#   - C++ 빌드 도구 설치
#   - colcon 빌드 도구 설치
#   - ROS2 C++ 패키지 실습 준비
# =========================================================

FROM osrf/ros:humble-desktop

# ---------------------------------------------------------
# 1. 기본 쉘 설정
# ---------------------------------------------------------
SHELL ["/bin/bash", "-c"]

# ---------------------------------------------------------
# 2. 환경 변수 설정
# ---------------------------------------------------------
ENV DEBIAN_FRONTEND=noninteractive
ENV ROS_DISTRO=humble
ENV LANG=C.UTF-8
ENV LC_ALL=C.UTF-8

# ---------------------------------------------------------
# 3. 개발 도구 설치
# ---------------------------------------------------------
# build-essential : g++, make 등 C++ 빌드 도구
# cmake           : CMake 기반 빌드 도구
# git             : 소스 코드 다운로드
# nano/vim        : 컨테이너 내부 간단 편집용
# python3-colcon-common-extensions : ROS2 colcon 빌드 도구
# python3-rosdep  : ROS 패키지 의존성 관리 도구
# ros-humble-demo-nodes-cpp : C++ demo talker/listener 실습용
# ros-humble-demo-nodes-py  : Python demo 비교용
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    nano \
    terminator \
    vim \
    tree \
    net-tools \
    iputils-ping \
    python3-pip \
    python3-colcon-common-extensions \
    python3-rosdep \
    ros-humble-demo-nodes-cpp \
    ros-humble-demo-nodes-py \
    && rm -rf /var/lib/apt/lists/*

# ---------------------------------------------------------
# 4. rosdep 초기화
# ---------------------------------------------------------
# rosdep init은 이미 되어 있거나 권한/네트워크 상태에 따라 실패할 수 있으므로
# 실패해도 이미지 빌드가 멈추지 않도록 처리합니다.
RUN rosdep init || true
RUN rosdep update || true

# ---------------------------------------------------------
# 5. 작업 디렉터리 생성
# ---------------------------------------------------------
WORKDIR /root/ros2_ws

# ---------------------------------------------------------
# 6. bash 실행 시 ROS2 환경 자동 로드
# ---------------------------------------------------------
RUN echo "source /opt/ros/humble/setup.bash" >> /root/.bashrc
RUN echo "if [ -f /root/ros2_ws/install/setup.bash ]; then source /root/ros2_ws/install/setup.bash; fi" >> /root/.bashrc

# ---------------------------------------------------------
# 7. 기본 실행 명령
# ---------------------------------------------------------
CMD ["/bin/bash"]