#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <geometry_msgs/Twist.h>

using namespace cv;
using namespace std;


int const thresh = 100;
int const max_binary_value = 255;
int threshold_type = 1;
Mat src_gray, roi;
RNG rng(12345);
Vec3f colour;
bool first = true;

ros::Publisher twist_publisher;


void thresh_callback(int, void* );

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	cv_bridge::CvImagePtr cv_ptr;

    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, "");
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
	transpose(cv_ptr->image, cv_ptr->image);
	flip(cv_ptr->image, cv_ptr->image, +1);
	Mat img = cv_ptr->image;


	roi = Mat(img, Rect(10, 3*cv_ptr->image.rows/4, cv_ptr->image.cols-20, cv_ptr->image.rows/6));

	
	cvtColor( roi, src_gray, COLOR_BGR2GRAY );
    blur( src_gray, src_gray, Size(3,3) );
  
    const int max_thresh = 255;
    thresh_callback( 0, 0 );
}
void thresh_callback(int, void* )
{
	Mat dst;
	threshold( src_gray, dst, thresh, max_binary_value, threshold_type ); //1:Binary Inverted.
    // Mat canny_output;
    // Canny( src_gray, canny_output, thresh, thresh*2 );
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    
    findContours( dst, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE );
    
    if (contours.size() > 0) {
    	vector<Point> maxContour = contours[0];
    	double maxArea = contourArea(maxContour);

    	for (int i = 0; i < contours.size(); i++) {
    		double area = contourArea(contours[i]);

    		if (area > maxArea) {
    			maxContour = contours[i];
    			maxArea = area;
    		}

    	}

    	Moments m = moments(maxContour);
for( int i = 0; i< contours.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,0), rng.uniform(0,0) );
       drawContours( dst, contours, i, color, 2, 8, hierarchy, 0, Point() );
     }	imshow("window", dst);
    waitKey(10);
    	 
    	int cx = m.m10/m.m00;
    	int cy = m.m01/m.m00;

    	if (first == true) {
    		colour = roi.at<Vec3b>(Point(cx, cy));
    		first = false;
    	}

    	geometry_msgs::Twist message;
    	message.linear.x = 1;
    	message.linear.y = 0;
    	message.linear.z = 0;

    	message.angular.x = 0;
    	message.angular.y = 0;

    	int turn = 0;
    	if (cx >= 300 && cx <= 400) {
    		turn = 0;
    	} else if (cx > 400) {
    		turn = -1;
    	} else if (cx < 300) {
    		turn = 1;
    	}

    	message.angular.z = turn;

    	twist_publisher.publish(message);

    }



}
  

int main(int argc, char** argv)
{
  ros::init(argc, argv, "linefollower_53");

  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);
  image_transport::TransportHints hints("compressed");
  image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback, hints);
  namedWindow("window", WINDOW_AUTOSIZE);

  twist_publisher = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);

  ros::spin();
  return 0;
}