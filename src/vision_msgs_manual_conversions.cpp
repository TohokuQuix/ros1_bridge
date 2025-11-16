#include "ros1_bridge/factory.hpp"

#include <cstddef>
#include <string>

#include <ros/serialization.h>

#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/PoseWithCovariance.h"
#include "sensor_msgs/Image.h"
#include "sensor_msgs/PointCloud2.h"
#include "std_msgs/Header.h"
#include "vision_msgs/BoundingBox2D.h"
#include "vision_msgs/BoundingBox3D.h"
#include "vision_msgs/Detection2D.h"
#include "vision_msgs/Detection3D.h"
#include "vision_msgs/ObjectHypothesis.h"
#include "vision_msgs/ObjectHypothesisWithPose.h"

#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "std_msgs/msg/header.hpp"
#include "vision_msgs/msg/bounding_box2_d.hpp"
#include "vision_msgs/msg/bounding_box3_d.hpp"
#include "vision_msgs/msg/detection2_d.hpp"
#include "vision_msgs/msg/detection3_d.hpp"
#include "vision_msgs/msg/object_hypothesis.hpp"
#include "vision_msgs/msg/object_hypothesis_with_pose.hpp"
#include "vision_msgs/msg/point2_d.hpp"
#include "vision_msgs/msg/pose2_d.hpp"
#include "vision_msgs/msg/pose2_d.hpp"

namespace ros1_bridge
{
namespace
{
template<typename Ros1T, typename Ros2T>
void serialize_ros2_as_ros1(ros::serialization::OStream & stream, const Ros2T & ros2_msg)
{
  Ros1T ros1_msg;
  Factory<Ros1T, Ros2T>::convert_2_to_1(ros2_msg, ros1_msg);
  ros::serialization::serialize(stream, ros1_msg);
}

template<typename Ros1T, typename Ros2T>
void serialize_ros2_length_as_ros1(ros::serialization::LStream & stream, const Ros2T & ros2_msg)
{
  Ros1T ros1_msg;
  Factory<Ros1T, Ros2T>::convert_2_to_1(ros2_msg, ros1_msg);
  ros::serialization::serialize(stream, ros1_msg);
}

template<typename Ros1T, typename Ros2T>
void deserialize_ros1_into_ros2(ros::serialization::IStream & stream, Ros2T & ros2_msg)
{
  Ros1T ros1_msg;
  ros::serialization::deserialize(stream, ros1_msg);
  Factory<Ros1T, Ros2T>::convert_1_to_2(ros1_msg, ros2_msg);
}

inline int64_t parse_class_id(const std::string & class_id)
{
  try {
    size_t processed = 0;
    auto value = std::stoll(class_id, &processed, 0);
    if (processed == class_id.size()) {
      return value;
    }
  } catch (...) {
  }
  return 0;
}
}  // namespace

using Ros1Detection2D = vision_msgs::Detection2D;
using Ros2Detection2D = vision_msgs::msg::Detection2D;
using Ros1Detection3D = vision_msgs::Detection3D;
using Ros2Detection3D = vision_msgs::msg::Detection3D;
using Ros1BoundingBox2D = vision_msgs::BoundingBox2D;
using Ros2BoundingBox2D = vision_msgs::msg::BoundingBox2D;
using Ros1ObjectHypothesisWithPose = vision_msgs::ObjectHypothesisWithPose;
using Ros2ObjectHypothesisWithPose = vision_msgs::msg::ObjectHypothesisWithPose;
using Ros1Pose2D = geometry_msgs::Pose2D;
using Ros2Pose2D = vision_msgs::msg::Pose2D;

template<>
void
Factory<Ros1Pose2D, Ros2Pose2D>::convert_1_to_2(
  const Ros1Pose2D & ros1_msg,
  Ros2Pose2D & ros2_msg)
{
  ros2_msg.position.x = ros1_msg.x;
  ros2_msg.position.y = ros1_msg.y;
  ros2_msg.theta = ros1_msg.theta;
}

template<>
void
Factory<Ros1Pose2D, Ros2Pose2D>::convert_2_to_1(
  const Ros2Pose2D & ros2_msg,
  Ros1Pose2D & ros1_msg)
{
  ros1_msg.x = ros2_msg.position.x;
  ros1_msg.y = ros2_msg.position.y;
  ros1_msg.theta = ros2_msg.theta;
}

template<>
void
Factory<Ros1Pose2D, Ros2Pose2D>::internal_stream_translate_helper(
  ros::serialization::OStream & stream,
  const Ros2Pose2D & ros2_msg)
{
  serialize_ros2_as_ros1<Ros1Pose2D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1Pose2D, Ros2Pose2D>::internal_stream_translate_helper(
  ros::serialization::IStream & stream,
  Ros2Pose2D & ros2_msg)
{
  deserialize_ros1_into_ros2<Ros1Pose2D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1Pose2D, Ros2Pose2D>::internal_stream_translate_helper(
  ros::serialization::LStream & stream,
  const Ros2Pose2D & ros2_msg)
{
  serialize_ros2_length_as_ros1<Ros1Pose2D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1Pose2D, Ros2Pose2D>::convert_2_to_1(
  const Ros2Pose2D & ros2_msg,
  ros::serialization::OStream & out_stream)
{
  internal_stream_translate_helper(out_stream, ros2_msg);
}

template<>
void
Factory<Ros1Pose2D, Ros2Pose2D>::convert_1_to_2(
  ros::serialization::IStream & in_stream,
  Ros2Pose2D & ros2_msg)
{
  internal_stream_translate_helper(in_stream, ros2_msg);
}

template<>
uint32_t
Factory<Ros1Pose2D, Ros2Pose2D>::length_2_as_1_stream(
  const Ros2Pose2D & ros2_msg)
{
  ros::serialization::LStream len_stream;
  internal_stream_translate_helper(len_stream, ros2_msg);
  return len_stream.getLength();
}

template<>
void
Factory<Ros1BoundingBox2D, Ros2BoundingBox2D>::convert_1_to_2(
  const Ros1BoundingBox2D & ros1_msg,
  Ros2BoundingBox2D & ros2_msg)
{
  Factory<
    geometry_msgs::Pose2D,
    vision_msgs::msg::Pose2D
  >::convert_1_to_2(ros1_msg.center, ros2_msg.center);
  ros2_msg.size_x = ros1_msg.size_x;
  ros2_msg.size_y = ros1_msg.size_y;
}

template<>
void
Factory<Ros1BoundingBox2D, Ros2BoundingBox2D>::convert_2_to_1(
  const Ros2BoundingBox2D & ros2_msg,
  Ros1BoundingBox2D & ros1_msg)
{
  Factory<
    geometry_msgs::Pose2D,
    vision_msgs::msg::Pose2D
  >::convert_2_to_1(ros2_msg.center, ros1_msg.center);
  ros1_msg.size_x = ros2_msg.size_x;
  ros1_msg.size_y = ros2_msg.size_y;
}

template<>
void
Factory<Ros1BoundingBox2D, Ros2BoundingBox2D>::internal_stream_translate_helper(
  ros::serialization::OStream & stream,
  const Ros2BoundingBox2D & ros2_msg)
{
  serialize_ros2_as_ros1<Ros1BoundingBox2D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1BoundingBox2D, Ros2BoundingBox2D>::internal_stream_translate_helper(
  ros::serialization::IStream & stream,
  Ros2BoundingBox2D & ros2_msg)
{
  deserialize_ros1_into_ros2<Ros1BoundingBox2D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1BoundingBox2D, Ros2BoundingBox2D>::internal_stream_translate_helper(
  ros::serialization::LStream & stream,
  const Ros2BoundingBox2D & ros2_msg)
{
  serialize_ros2_length_as_ros1<Ros1BoundingBox2D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1BoundingBox2D, Ros2BoundingBox2D>::convert_2_to_1(
  const Ros2BoundingBox2D & ros2_msg,
  ros::serialization::OStream & out_stream)
{
  internal_stream_translate_helper(out_stream, ros2_msg);
}

template<>
void
Factory<Ros1BoundingBox2D, Ros2BoundingBox2D>::convert_1_to_2(
  ros::serialization::IStream & in_stream,
  Ros2BoundingBox2D & ros2_msg)
{
  internal_stream_translate_helper(in_stream, ros2_msg);
}

template<>
uint32_t
Factory<Ros1BoundingBox2D, Ros2BoundingBox2D>::length_2_as_1_stream(
  const Ros2BoundingBox2D & ros2_msg)
{
  ros::serialization::LStream len_stream;
  internal_stream_translate_helper(len_stream, ros2_msg);
  return len_stream.getLength();
}

template<>
void
Factory<Ros1ObjectHypothesisWithPose, Ros2ObjectHypothesisWithPose>::convert_1_to_2(
  const Ros1ObjectHypothesisWithPose & ros1_msg,
  Ros2ObjectHypothesisWithPose & ros2_msg)
{
  ros2_msg.hypothesis.class_id = std::to_string(ros1_msg.id);
  ros2_msg.hypothesis.score = ros1_msg.score;
  Factory<
    geometry_msgs::PoseWithCovariance,
    geometry_msgs::msg::PoseWithCovariance
  >::convert_1_to_2(ros1_msg.pose, ros2_msg.pose);
}

template<>
void
Factory<Ros1ObjectHypothesisWithPose, Ros2ObjectHypothesisWithPose>::convert_2_to_1(
  const Ros2ObjectHypothesisWithPose & ros2_msg,
  Ros1ObjectHypothesisWithPose & ros1_msg)
{
  ros1_msg.id = parse_class_id(ros2_msg.hypothesis.class_id);
  ros1_msg.score = ros2_msg.hypothesis.score;
  Factory<
    geometry_msgs::PoseWithCovariance,
    geometry_msgs::msg::PoseWithCovariance
  >::convert_2_to_1(ros2_msg.pose, ros1_msg.pose);
}

template<>
void
Factory<Ros1ObjectHypothesisWithPose, Ros2ObjectHypothesisWithPose>::internal_stream_translate_helper(
  ros::serialization::OStream & stream,
  const Ros2ObjectHypothesisWithPose & ros2_msg)
{
  serialize_ros2_as_ros1<Ros1ObjectHypothesisWithPose>(stream, ros2_msg);
}

template<>
void
Factory<Ros1ObjectHypothesisWithPose, Ros2ObjectHypothesisWithPose>::internal_stream_translate_helper(
  ros::serialization::IStream & stream,
  Ros2ObjectHypothesisWithPose & ros2_msg)
{
  deserialize_ros1_into_ros2<Ros1ObjectHypothesisWithPose>(stream, ros2_msg);
}

template<>
void
Factory<Ros1ObjectHypothesisWithPose, Ros2ObjectHypothesisWithPose>::internal_stream_translate_helper(
  ros::serialization::LStream & stream,
  const Ros2ObjectHypothesisWithPose & ros2_msg)
{
  serialize_ros2_length_as_ros1<Ros1ObjectHypothesisWithPose>(stream, ros2_msg);
}

template<>
void
Factory<Ros1ObjectHypothesisWithPose, Ros2ObjectHypothesisWithPose>::convert_2_to_1(
  const Ros2ObjectHypothesisWithPose & ros2_msg,
  ros::serialization::OStream & out_stream)
{
  internal_stream_translate_helper(out_stream, ros2_msg);
}

template<>
void
Factory<Ros1ObjectHypothesisWithPose, Ros2ObjectHypothesisWithPose>::convert_1_to_2(
  ros::serialization::IStream & in_stream,
  Ros2ObjectHypothesisWithPose & ros2_msg)
{
  internal_stream_translate_helper(in_stream, ros2_msg);
}

template<>
uint32_t
Factory<Ros1ObjectHypothesisWithPose, Ros2ObjectHypothesisWithPose>::length_2_as_1_stream(
  const Ros2ObjectHypothesisWithPose & ros2_msg)
{
  ros::serialization::LStream len_stream;
  internal_stream_translate_helper(len_stream, ros2_msg);
  return len_stream.getLength();
}

template<>
void
Factory<Ros1Detection2D, Ros2Detection2D>::convert_1_to_2(
  const Ros1Detection2D & ros1_msg,
  Ros2Detection2D & ros2_msg)
{
  Factory<std_msgs::Header, std_msgs::msg::Header>::convert_1_to_2(
    ros1_msg.header, ros2_msg.header);

  ros2_msg.results.resize(ros1_msg.results.size());
  for (size_t i = 0; i < ros1_msg.results.size(); ++i) {
    Factory<
      vision_msgs::ObjectHypothesisWithPose,
      vision_msgs::msg::ObjectHypothesisWithPose
    >::convert_1_to_2(ros1_msg.results[i], ros2_msg.results[i]);
  }

  Factory<
    vision_msgs::BoundingBox2D,
    vision_msgs::msg::BoundingBox2D
  >::convert_1_to_2(ros1_msg.bbox, ros2_msg.bbox);

  ros2_msg.id.clear();
}

template<>
void
Factory<Ros1Detection2D, Ros2Detection2D>::convert_2_to_1(
  const Ros2Detection2D & ros2_msg,
  Ros1Detection2D & ros1_msg)
{
  Factory<std_msgs::Header, std_msgs::msg::Header>::convert_2_to_1(
    ros2_msg.header, ros1_msg.header);

  ros1_msg.results.resize(ros2_msg.results.size());
  for (size_t i = 0; i < ros2_msg.results.size(); ++i) {
    Factory<
      vision_msgs::ObjectHypothesisWithPose,
      vision_msgs::msg::ObjectHypothesisWithPose
    >::convert_2_to_1(ros2_msg.results[i], ros1_msg.results[i]);
  }

  Factory<
    vision_msgs::BoundingBox2D,
    vision_msgs::msg::BoundingBox2D
  >::convert_2_to_1(ros2_msg.bbox, ros1_msg.bbox);

  ros1_msg.source_img = sensor_msgs::Image();
}

template<>
void
Factory<Ros1Detection2D, Ros2Detection2D>::internal_stream_translate_helper(
  ros::serialization::OStream & stream,
  const Ros2Detection2D & ros2_msg)
{
  serialize_ros2_as_ros1<Ros1Detection2D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1Detection2D, Ros2Detection2D>::internal_stream_translate_helper(
  ros::serialization::IStream & stream,
  Ros2Detection2D & ros2_msg)
{
  deserialize_ros1_into_ros2<Ros1Detection2D>(stream, ros2_msg);
  ros2_msg.id.clear();
}

template<>
void
Factory<Ros1Detection2D, Ros2Detection2D>::internal_stream_translate_helper(
  ros::serialization::LStream & stream,
  const Ros2Detection2D & ros2_msg)
{
  serialize_ros2_length_as_ros1<Ros1Detection2D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1Detection2D, Ros2Detection2D>::convert_2_to_1(
  const Ros2Detection2D & ros2_msg,
  ros::serialization::OStream & out_stream)
{
  internal_stream_translate_helper(out_stream, ros2_msg);
}

template<>
void
Factory<Ros1Detection2D, Ros2Detection2D>::convert_1_to_2(
  ros::serialization::IStream & in_stream,
  Ros2Detection2D & ros2_msg)
{
  internal_stream_translate_helper(in_stream, ros2_msg);
}

template<>
uint32_t
Factory<Ros1Detection2D, Ros2Detection2D>::length_2_as_1_stream(
  const Ros2Detection2D & ros2_msg)
{
  ros::serialization::LStream len_stream;
  internal_stream_translate_helper(len_stream, ros2_msg);
  return len_stream.getLength();
}

template<>
void
Factory<Ros1Detection3D, Ros2Detection3D>::convert_1_to_2(
  const Ros1Detection3D & ros1_msg,
  Ros2Detection3D & ros2_msg)
{
  Factory<std_msgs::Header, std_msgs::msg::Header>::convert_1_to_2(
    ros1_msg.header, ros2_msg.header);

  ros2_msg.results.resize(ros1_msg.results.size());
  for (size_t i = 0; i < ros1_msg.results.size(); ++i) {
    Factory<
      vision_msgs::ObjectHypothesisWithPose,
      vision_msgs::msg::ObjectHypothesisWithPose
    >::convert_1_to_2(ros1_msg.results[i], ros2_msg.results[i]);
  }

  Factory<
    vision_msgs::BoundingBox3D,
    vision_msgs::msg::BoundingBox3D
  >::convert_1_to_2(ros1_msg.bbox, ros2_msg.bbox);

  ros2_msg.id.clear();
}

template<>
void
Factory<Ros1Detection3D, Ros2Detection3D>::convert_2_to_1(
  const Ros2Detection3D & ros2_msg,
  Ros1Detection3D & ros1_msg)
{
  Factory<std_msgs::Header, std_msgs::msg::Header>::convert_2_to_1(
    ros2_msg.header, ros1_msg.header);

  ros1_msg.results.resize(ros2_msg.results.size());
  for (size_t i = 0; i < ros2_msg.results.size(); ++i) {
    Factory<
      vision_msgs::ObjectHypothesisWithPose,
      vision_msgs::msg::ObjectHypothesisWithPose
    >::convert_2_to_1(ros2_msg.results[i], ros1_msg.results[i]);
  }

  Factory<
    vision_msgs::BoundingBox3D,
    vision_msgs::msg::BoundingBox3D
  >::convert_2_to_1(ros2_msg.bbox, ros1_msg.bbox);

  ros1_msg.source_cloud = sensor_msgs::PointCloud2();
}

template<>
void
Factory<Ros1Detection3D, Ros2Detection3D>::internal_stream_translate_helper(
  ros::serialization::OStream & stream,
  const Ros2Detection3D & ros2_msg)
{
  serialize_ros2_as_ros1<Ros1Detection3D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1Detection3D, Ros2Detection3D>::internal_stream_translate_helper(
  ros::serialization::IStream & stream,
  Ros2Detection3D & ros2_msg)
{
  deserialize_ros1_into_ros2<Ros1Detection3D>(stream, ros2_msg);
  ros2_msg.id.clear();
}

template<>
void
Factory<Ros1Detection3D, Ros2Detection3D>::internal_stream_translate_helper(
  ros::serialization::LStream & stream,
  const Ros2Detection3D & ros2_msg)
{
  serialize_ros2_length_as_ros1<Ros1Detection3D>(stream, ros2_msg);
}

template<>
void
Factory<Ros1Detection3D, Ros2Detection3D>::convert_2_to_1(
  const Ros2Detection3D & ros2_msg,
  ros::serialization::OStream & out_stream)
{
  internal_stream_translate_helper(out_stream, ros2_msg);
}

template<>
void
Factory<Ros1Detection3D, Ros2Detection3D>::convert_1_to_2(
  ros::serialization::IStream & in_stream,
  Ros2Detection3D & ros2_msg)
{
  internal_stream_translate_helper(in_stream, ros2_msg);
}

template<>
uint32_t
Factory<Ros1Detection3D, Ros2Detection3D>::length_2_as_1_stream(
  const Ros2Detection3D & ros2_msg)
{
  ros::serialization::LStream len_stream;
  internal_stream_translate_helper(len_stream, ros2_msg);
  return len_stream.getLength();
}

}  // namespace ros1_bridge
