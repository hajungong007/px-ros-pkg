#include <ros/ros.h>

#include "px4flow/SerialComm.h"

void
printTopicInfo(void)
{
    // print published/subscribed topics
    std::string nodeName = ros::this_node::getName();

    ros::V_string topics;
    ros::this_node::getSubscribedTopics(topics);

    std::string topicsStr = nodeName + ":\n\tsubscribed to topics:\n";
    for (unsigned int i = 0; i < topics.size(); ++i)
    {
        topicsStr += ("\t\t" + topics.at(i) + "\n");
    }

    topicsStr += "\tadvertised topics:\n";
    ros::this_node::getAdvertisedTopics(topics);
    for (unsigned int i = 0; i < topics.size(); ++i)
    {
        topicsStr += ("\t\t" + topics.at(i) + "\n");
    }

    ROS_INFO("%s", topicsStr.c_str());
}

int
main(int argc, char** argv)
{
    ros::init(argc, argv, "px4flow");
    ros::NodeHandle nh("px4flow");

    ros::NodeHandle pnh("~");

    std::string portStr;
    pnh.param("serial_port", portStr, std::string("/dev/ttyUSB0"));

    int baudrate;
    pnh.param("baudrate", baudrate, 115200);

    std::string frameId;
    pnh.param("frame_id", frameId, std::string("/px4flow"));

    bool output_filtered;
    pnh.param("output_filtered", output_filtered, false);

    int filter_window_size;
    pnh.param("filter_window_size", filter_window_size, 5);

    px::SerialComm comm(frameId);
    if (!comm.open(portStr, baudrate, output_filtered, filter_window_size))
    {
        return -1;
    }

    printTopicInfo();

    ros::spin();

    return 0;
}

