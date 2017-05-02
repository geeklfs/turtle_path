/*************************************************************************
	> File Name: display_trajectory.cpp
	> Author: 
	> Mail: 
	> Created Time: Sat 29 Apr 2017 06:18:43 PM CST
 ************************************************************************/

#include <ros/ros.h>
#include <tf/tf.h>
#include <turtlesim/Pose.h>
#include <nav_msgs/Path.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/PolygonStamped.h>

ros::Publisher* robot_pose_publisher_ptr {nullptr};
ros::Publisher* robot_path_publisher_ptr {nullptr};
ros::Publisher* robot_boundary_publisher_ptr {nullptr};
nav_msgs::Path path_;

geometry_msgs::PolygonStamped showRobotBoundary()
{
  geometry_msgs::PolygonStamped boundary;
  boundary.header.frame_id = "robot";
  geometry_msgs::Point32 pts;
  pts.x = 0.0;
  pts.y = 0.0;
  boundary.polygon.points.push_back(pts);
  pts.x = 0.0;
  pts.y = 11.0;
  boundary.polygon.points.push_back(pts);
  pts.x = 11.0;
  pts.y = 11.0;
  boundary.polygon.points.push_back(pts);
  pts.x = 11.0;
  pts.y = 0.0;
  boundary.polygon.points.push_back(pts);
  return boundary;
}

void poseCallback(const turtlesim::Pose& msg)
{
    geometry_msgs::PoseStamped robotposition;
    robotposition.header.frame_id = "robot";
    tf2::Quaternion q;
    q.setRPY(0, 0, msg.theta);
    robotposition.pose.position.x = msg.x;
    robotposition.pose.position.y = msg.y;
    robotposition.pose.orientation.x = q.x();
    robotposition.pose.orientation.y = q.y();
    robotposition.pose.orientation.z = q.z();
    robotposition.pose.orientation.w = q.w();
    path_.poses.push_back(robotposition);
    path_.header.frame_id = "robot";
    if (robot_pose_publisher_ptr)
    {
      robot_pose_publisher_ptr->publish(robotposition);
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "display");

  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("turtle1/pose", 10, poseCallback);

  ros::Publisher robot_path_pub = n.advertise<nav_msgs::Path>("robot_path", 10);
  robot_path_publisher_ptr = &robot_path_pub;

  ros::Publisher robot_pose_pub = n.advertise<geometry_msgs::PoseStamped>("robot_pose", 10);
  robot_pose_publisher_ptr =  &robot_pose_pub;

  ros::Publisher robot_boundary_pub = n.advertise<geometry_msgs::PolygonStamped>("robot_boundary", 10);
  robot_boundary_publisher_ptr =  &robot_boundary_pub;

  // set the comunacation frequency as 10Hz
  ros::Rate loop_rate(10);

  while (ros::ok())
  {

    robot_path_publisher_ptr->publish(path_);
    robot_boundary_publisher_ptr->publish(showRobotBoundary());
    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}





