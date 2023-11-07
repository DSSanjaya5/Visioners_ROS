#include <bits/stdc++.h>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include "hackathon/location.h"
#include "hackathon/radar_data.h"
using namespace std;
typedef pcl::PointCloud<pcl::PointXYZRGB> PointCloud;

// Publisher declaration to publish points
ros::Publisher points_pub;


/*
	processingCallback - Transform data from spherical coordinate system to cartesian coordinate system
	Append the data to a PointCloud points and publish it over RadarPointCloud topic 
*/
 
void processingCallback(const hackathon::radar_data r_data)
{
    PointCloud coordinates;
    coordinates.header.frame_id = "points";
    coordinates.height = 1;
    coordinates.width = r_data.location_list.size();
    coordinates.points.resize(coordinates.height * coordinates.width);
  
    for(int i=0; i<r_data.location_list.size(); i++)
    {
  
        float x = r_data.location_list[i].radius*sin((M_PI/180)*(90-r_data.location_list[i].elevationAngle))*cos((M_PI/180)*r_data.location_list[i].azimuthAngle);  
        float y = r_data.location_list[i].radius*sin((M_PI/180)*(90-r_data.location_list[i].elevationAngle))*sin((M_PI/180)*r_data.location_list[i].azimuthAngle); 
        float z = r_data.location_list[i].radius*cos((M_PI/180)*(90-r_data.location_list[i].elevationAngle));
    
        ROS_INFO_STREAM("x = "<<x<<", "<<"y = "<<y<<", "<<"z = "<<z<<endl);
    
        float r = 255.0 * (float) rand()/RAND_MAX;
        float g = 255.0 * (float) rand()/RAND_MAX;
        float b = 255.0 * (float) rand()/RAND_MAX;
    
        coordinates.points[i].x = x;
        coordinates.points[i].y = y;
        coordinates.points[i].z = z;
        coordinates.points[i].r = r;
        coordinates.points[i].g = g;
        coordinates.points[i].b = b;
    
        points_pub.publish(coordinates);
        
    }
}

int main(int argc, char **argv)
{
    // Initialization of the node and declaration of node handle	
    ros::init(argc, argv, "pc2");
    ros::NodeHandle n;
  
    // Subscribe to RadarData topic
    ros::Subscriber sub = n.subscribe("RadarData", 1000, processingCallback);
  
    // Advertise the Publisher Node
    points_pub = n.advertise<PointCloud>("RadarPointCloud", 1000);
  
    // spin() will not return until the node has been shutdown
    ros::spin();      
  
    ros::shutdown();	
    return 0;
}
