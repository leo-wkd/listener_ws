#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include<iostream>
#include <stdio.h>
#include <string.h>
#define camInfo
using namespace cv;
using namespace std;
bool is_save = false;
void imageCallback1(const sensor_msgs::ImageConstPtr& msg)
{
/*    ***********************************To set frame quality.*****************************************
    std::vector<int> compression_params;
    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compression_params.push_back(0);
*/
    if(is_save)
    {
        std::string prefix="left";
        std::string suffix=".jpg";
        std::string path1="/home/leo/Datasets/zed2/left/";
        char file1[5];
        char cc1[100];
        static int cnt1=0;
        try 
        {
            sprintf(file1,"%04d",cnt1);
            std::string name = path1 + prefix + file1 + suffix;
            strcpy(cc1, name.c_str());
            cv::imwrite(name, cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::TYPE_8UC4)->image);
            cnt1++;
            ROS_INFO("Successfully save left to '%s' ", cc1); 
        }
        catch(cv_bridge::Exception& e)
        {
            ROS_ERROR("Could not convert for '%s' to 'bgr8'.", msg->encoding.c_str());
        }
    }
    else
    {
        try
        {
            cv::namedWindow("Left_Camera");
            cv::imshow("Left_Camera",cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_8UC4)->image);
            cv::waitKey(1);
        }
        catch(cv_bridge::Exception& e)
        {
            ROS_ERROR("Could not display left image!!!");
        }
    }
    

    
}
void imageCallback2(const sensor_msgs::ImageConstPtr& msg)
{
    if(is_save)
    {
        std::string prefix="right";
        std::string suffix=".jpg";
        std::string path2="/home/leo/Datasets/zed2/right/";
        char file2[5];
        char cc2[100];
        static int cnt2=0;
        try 
        {
            sprintf(file2,"%04d",cnt2);
            std::string name = path2 + prefix + file2 + suffix;
            strcpy(cc2, name.c_str());
            cv::imwrite(name, cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::TYPE_8UC4)->image);
            cnt2++;
            ROS_INFO("Successfully save right to '%s' ", cc2); 
        }
        catch(cv_bridge::Exception& e)
        {
            ROS_ERROR("Could not convert for '%s' to 'bgr8'.", msg->encoding.c_str());
        }
    }
    else
    {
        try
        {
            cv::namedWindow("Right_Camera");
            cv::imshow("Right_Camera",cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_8UC4)->image);
            cv::waitKey(1);
        }
        catch(cv_bridge::Exception& e)
        {
            ROS_ERROR("Could not display right image!!!");
        }
    }
    

    
}

void depthCallback(const sensor_msgs::ImageConstPtr& msg)
{
    /***************************************** save depth to image with normalization
    std::string prefix="frame";
    std::string suffix=".jpg";
    std::string path3="/home/leo/Datasets/zed2/depth/";
    char file3[5];
    char cc3[100];
    static int cnt3=0;
    cv_bridge::CvImagePtr depth_ptr;
    cv::Mat matTemp = cv::Mat::zeros(1080,1920,CV_32FC1);
    try  
    {	
	sprintf(file3,"%04d",cnt3);
	std::string name = path3 + prefix + file3 + suffix;
	strcpy(cc3, name.c_str());

	//depth_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1); 
	depth_ptr = cv_bridge::toCvShare(msg, sensor_msgs::image_encodings::TYPE_32FC1);
	cv::Mat matTemp = depth_ptr->image;

	if(cnt3==2)
	{
		FileStorage cp("/home/leo/Datasets/zed2/frame.txt",FileStorage::WRITE); 
		cp<<"frame"<<matTemp;
		cp.release();
	}

	//cv::Mat result;
	//depth_img.copyTo(result);
	//cv::imshow("depth",depth_img);
	//cv::waitKey(1);
	cv::imwrite(name, matTemp);
	cnt3++;
	ROS_INFO("Successfully save image to '%s' ", cc3);    
    }
    catch(cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert for '%s' to '32fc1'.", msg->encoding.c_str());
    }
    *******************************************/
    if(is_save)
    {
        FILE *fp;
        static int cnt3=0;
        char file3[5];
        char cc3[100];
        std::string prefix3="depth";
        std::string suffix3=".txt";
        std::string path3="/home/leo/Datasets/zed2/depth/";

    #ifdef camInfo
        int u = msg->width;
        int v = msg->height;
        printf("%d ",u);printf("%c",' ');
        printf("%d",v);printf("%s",'\n');
    #endif

        float* depths = (float*)(&msg->data[0]);
        int u ;
        int v ;
        int depthIdx ;
        try
        {
            sprintf(file3,"%04d",cnt3);
            std::string address = path3 + prefix3 + file3 + suffix3;
            strcpy(cc3, address.c_str());
            fp = fopen(cc3,"wb");
            for (v  = 0; v < msg->height; v++)
            {
                for ( u = 0; u < msg->width; u++)
                {
                    depthIdx = u + msg->width * v;
                    fprintf(fp,"%g",depths[depthIdx]);
                    fprintf(fp,"%s","   ");
                }
                fprintf(fp,"%c",'\n');
            }
            fclose(fp);
            ROS_INFO("Successfully save depthImg to '%s' ", cc3);
            cnt3++;
        }
        catch(const std::exception& e)
        {
            ROS_ERROR("Could not save depthImg");
        }
    }
    else
    {
        try
        {
            cv::namedWindow("Depth_Sensor");
            cv::imshow("Depth_Sensor",cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1)->image);
            cv::waitKey(1);
        }
        catch(cv_bridge::Exception& e)
        {
            ROS_ERROR("Could not display depth!!!");
        }
    }
 
}


int main(int argc, char** argv)
{
    
    if(argc != 5)
    {
        cout<<"usage:\n";
        cout<<"   source ./devel/setup.bash\n";
        cout<<"   rosrun zed_listener zed_listener_node left_flag right_flag depth_flag mode_flag\n";
        cout<<"      -left_flag     0-Turn off      1-Turn on\n";
        cout<<"      -right_flag    0-Turn off      1-Turn on\n";
        cout<<"      -depth_flag    0-Turn off      1-Turn on\n";
        cout<<"      -mode_flag     0-Display       1-Save\n";
        cout<<"Example:\n";
        cout<<"rosrun zed_listener zed_listener_node 1 1 0 0\n";
        cout << "\nPress [Enter] to continue";
        cin.ignore();
        return 1;
    }

    ros::init(argc, argv, "imageSub_node");   
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber imageSub1;
    image_transport::Subscriber imageSub2;
    image_transport::Subscriber imageSub3;

    if(!strcmp(argv[1],"1"))
    {
        imageSub1 = it.subscribe("/zed2/zed_node/left/image_rect_color", 1000, imageCallback1);
        cout<<"Left_Camera  ON\n" ;
    }
    else if(!strcmp(argv[1],"0"))
    {
        cout<<"Left_Camera  OFF\n" ;
    }
    else
    {
        cout<<"Bad left_flag!!!!!!!!!\n";
        return 1;
    }
       
    if(!strcmp(argv[2],"1"))
    {
        imageSub2 = it.subscribe("/zed2/zed_node/right/image_rect_color", 1000, imageCallback2);
        cout<<"Right_Camera ON\n" ;
    }
    else if(!strcmp(argv[2],"0"))
    {
        cout<<"Right_Camera OFF\n" ;
    }
    else
    {
        cout<<"Bad right_flag!!!!!!!!!\n";
        return 1;
    }

    if(!strcmp(argv[3],"1"))
    {
        imageSub3 = it.subscribe("/zed2/zed_node/depth/depth_registered", 1000, depthCallback); 
        cout<<"Depth_Sensor ON\n" ;
    }
    else if(!strcmp(argv[3],"0"))
    {
        cout<<"Depth_Sensor OFF\n" ;
    }
    else
    {
        cout<<"Bad depth_flag!!!!!!!!!\n";
        return 1;
    }
    

    if(!strcmp(argv[4],"1"))
    {
        is_save = true;
        cout<<"Save_Mode ON\n" ;
    }
    else if(!strcmp(argv[4],"0"))
    {
        cout<<"Display_Mode ON\n" ;
    }
    else
    {
        cout<<"Bad mode_flag!!!!!!!!!\n";
        return 1;
    }

    ros::spin();
}
