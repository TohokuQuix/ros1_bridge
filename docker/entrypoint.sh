#!/bin/bash
set -eo pipefail

: "${ROS_MASTER_URI:=http://localhost:11311}"
export ROS_MASTER_URI

if [[ -n "${ROS_IP:-}" ]]; then
  export ROS_IP
fi

if [[ -n "${ROS_HOSTNAME:-}" ]]; then
  export ROS_HOSTNAME
fi

export RMW_IMPLEMENTATION="${RMW_IMPLEMENTATION:-rmw_zenoh_cpp}"

source "/opt/ros/${ROS1_DISTRO}/setup.bash"
source "/opt/ros/jazzy/setup.bash"
source "/opt/ros_ws/install/setup.bash"

exec "$@"
