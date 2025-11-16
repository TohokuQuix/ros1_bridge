ARG ROS1_DISTRO=one
ARG UBUNTU_CODENAME=noble
ARG ROS1_APT_URL=https://ros.packages.techfak.net
ARG ROS1_APT_KEY_PATH=docker/gpg.key

FROM ros:jazzy-ros-base AS builder
ARG ROS1_DISTRO
ARG UBUNTU_CODENAME
ARG ROS1_APT_URL
ARG ROS1_APT_KEY_PATH

SHELL ["/bin/bash", "-c"]

# Add ROS One apt repository (per https://ros.packages.techfak.net/)
COPY ${ROS1_APT_KEY_PATH} /tmp/ros-one.key
RUN apt-get update && \
    apt-get install -y --no-install-recommends ca-certificates gnupg2 && \
    gpg --dearmor /tmp/ros-one.key && \
    mv /tmp/ros-one.key.gpg /etc/apt/keyrings/ros-one-keyring.gpg && \
    echo "deb [arch=amd64 signed-by=/etc/apt/keyrings/ros-one-keyring.gpg] ${ROS1_APT_URL} ${UBUNTU_CODENAME} main" \
      > /etc/apt/sources.list.d/ros-one.list && \
    rm -rf /var/lib/apt/lists/*

# Install tools, ROS 1 base (specified via ROS1_DISTRO), vision_msgs, and build deps
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      build-essential \
      git \
      python3-colcon-common-extensions \
      ros-${ROS1_DISTRO}-ros-base \
      ros-${ROS1_DISTRO}-vision-msgs \
      ros-jazzy-vision-msgs \
      ros-jazzy-example-interfaces \
      ros-jazzy-rmw-zenoh-cpp && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /opt/ros_ws/src
COPY . ros1_bridge

WORKDIR /opt/ros_ws
RUN source /opt/ros/${ROS1_DISTRO}/setup.bash && \
    source /opt/ros/jazzy/setup.bash && \
    colcon build --packages-select ros1_bridge --cmake-args -DCMAKE_BUILD_TYPE=Release

# Runtime image
FROM ros:jazzy-ros-base
ARG ROS1_APT_KEY_PATH
ARG ROS1_DISTRO
ARG UBUNTU_CODENAME
ARG ROS1_APT_URL
ENV ROS1_DISTRO=${ROS1_DISTRO} \
    RMW_IMPLEMENTATION=rmw_zenoh_cpp \
    RMW_ZENOH_CONFIG=/etc/ros/zenoh.json5

SHELL ["/bin/bash", "-c"]

# Add ROS One apt repository
COPY ${ROS1_APT_KEY_PATH} /tmp/ros-one.key
RUN apt-get update && \
    apt-get install -y --no-install-recommends ca-certificates gnupg2 && \
    gpg --dearmor /tmp/ros-one.key && \
    mv /tmp/ros-one.key.gpg /etc/apt/keyrings/ros-one-keyring.gpg && \
    echo "deb [arch=amd64 signed-by=/etc/apt/keyrings/ros-one-keyring.gpg] ${ROS1_APT_URL} ${UBUNTU_CODENAME} main" \
      > /etc/apt/sources.list.d/ros-one.list && \
    rm -rf /var/lib/apt/lists/*

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
      ros-${ROS1_DISTRO}-ros-base \
      ros-${ROS1_DISTRO}-vision-msgs \
      ros-jazzy-vision-msgs \
      ros-jazzy-example-interfaces \
      ros-jazzy-rmw-zenoh-cpp && \
    rm -rf /var/lib/apt/lists/*

COPY --from=builder /opt/ros_ws /opt/ros_ws
COPY docker/zenoh.json5 /etc/ros/zenoh.json5
COPY docker/entrypoint.sh /entrypoint.sh

RUN chmod +x /entrypoint.sh

WORKDIR /opt/ros_ws

ENTRYPOINT ["/entrypoint.sh"]
CMD ["ros2", "run", "ros1_bridge", "dynamic_bridge", "--bridge-all-topics"]
