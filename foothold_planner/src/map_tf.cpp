#include <ros/ros.h>
#include <tf2_ros/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>

ros::Publisher tfPub;
ros::Publisher gtPub;


void gtCallback(const nav_msgs::OdometryConstPtr &msg){
    // recieved pose msg.

    // map2base tf.
    geometry_msgs::TransformStamped tf;
    static tf2_ros::TransformBroadcaster br;

    tf.header.stamp = ros::Time::now();
    tf.header.frame_id = "world";
    tf.child_frame_id = "base";
    tf.transform.translation.x = msg->pose.pose.position.x;
    tf.transform.translation.y = msg->pose.pose.position.y;
    tf.transform.translation.z = msg->pose.pose.position.z;
    tf.transform.rotation.x = msg->pose.pose.orientation.x;
    tf.transform.rotation.y = msg->pose.pose.orientation.y;
    tf.transform.rotation.z = msg->pose.pose.orientation.z;
    tf.transform.rotation.w = msg->pose.pose.orientation.w;

    br.sendTransform(tf);

    // Odometry 2 PoseWithCovarrianceStampted 
    geometry_msgs::PoseWithCovarianceStamped gt;
    gt.header.stamp = ros::Time::now();
    gt.header.frame_id = "world";
    gt.pose = msg->pose;

    // pub
    tfPub.publish(tf);
    gtPub.publish(gt);
}


int main(int argc, char** argv){
    ros::init(argc, argv, "map2base_tf");

    ros::NodeHandle node;

    ros::Subscriber gtSub = node.subscribe("/laikago_gazebo/ground_truth", 500, &gtCallback);

    tfPub = node.advertise<geometry_msgs::TransformStamped>("/map2base_tf", 500);
    gtPub = node.advertise<geometry_msgs::PoseWithCovarianceStamped>("/laikago_gazebo/gt_pose", 500);

    ros::spin();

    return 0;
}