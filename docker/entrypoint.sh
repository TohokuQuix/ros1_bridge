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

ROUTER_LISTEN_DEFAULT='["tcp/0.0.0.0:7447"]'
ROUTER_CONNECT_DEFAULT='[]'

listen_override="${ZENOH_ROUTER_LISTEN_ENDPOINTS:-$ROUTER_LISTEN_DEFAULT}"
connect_override="${ZENOH_ROUTER_CONNECT_ENDPOINTS:-$ROUTER_CONNECT_DEFAULT}"
start_router="yes"

declare -a cmd_args=()

while [[ $# -gt 0 ]]; do
  case "$1" in
    --router-listen)
      listen_override="$2"
      shift 2
      ;;
    --router-connect)
      connect_override="$2"
      shift 2
      ;;
    --router-listen=*)
      listen_override="${1#*=}"
      shift
      ;;
    --router-connect=*)
      connect_override="${1#*=}"
      shift
      ;;
    --router-disable|--no-router)
      start_router="no"
      shift
      ;;
    --router-enable|--router)
      start_router="yes"
      shift
      ;;
    --)
      shift
      break
      ;;
    *)
      break
      ;;
  esac
done

if [[ $# -gt 0 ]]; then
  cmd_args=("$@")
else
  cmd_args=("ros2" "run" "ros1_bridge" "dynamic_bridge" "--bridge-all-topics")
fi

normalize_endpoints() {
  RAW="$1" python3 - <<'PY'
import json
import os
raw = os.environ.get("RAW", "")
if not raw.strip():
    print("[]")
elif raw.lstrip().startswith('['):
    json.loads(raw)
    print(raw)
else:
    entries = [segment.strip() for segment in raw.split(',') if segment.strip()]
    print(json.dumps(entries))
PY
}

source "/opt/ros/${ROS1_DISTRO}/setup.bash"
source "/opt/ros/jazzy/setup.bash"
source "/opt/ros_ws/install/setup.bash"

router_pid=""

if [[ "$start_router" == "yes" ]]; then
  listen_json="$(normalize_endpoints "$listen_override")"
  connect_json="$(normalize_endpoints "$connect_override")"

  router_template="/etc/ros/zenoh-router.json5"
  router_config="/tmp/zenoh-router.json5"

  cp "$router_template" "$router_config"

  python3 - "$router_config" "$listen_json" "$connect_json" <<'PY'
import json
import sys

config_path, listen_raw, connect_raw = sys.argv[1:4]

with open(config_path, "r", encoding="utf-8") as handle:
    data = json.load(handle)

data.setdefault("listen", {})["endpoints"] = json.loads(listen_raw)
data.setdefault("connect", {})["endpoints"] = json.loads(connect_raw)

with open(config_path, "w", encoding="utf-8") as handle:
    json.dump(data, handle, indent=2)
    handle.write("\n")
PY

  export ZENOH_ROUTER_CONFIG_URI="${router_config}"
  export ZENOH_ROUTER_CONFIG="${router_config}"

  ros2 run rmw_zenoh_cpp rmw_zenohd --config "${router_config}" &
  router_pid=$!
fi

cleanup() {
  if [[ -n "${router_pid:-}" ]] && kill -0 "$router_pid" 2>/dev/null; then
    kill "$router_pid" 2>/dev/null || true
    wait "$router_pid" 2>/dev/null || true
  fi
}
trap cleanup EXIT

exec "${cmd_args[@]}"
