#include "opencv2/opencv.hpp"
#include <stdio.h>

int main(int argc, char**argv)
{
	if(argc < 2) 
	{
		printf("test needs: device\n");
		return -1;
	}

	int id = atoi(argv[1]);
	cv::VideoCapture cap(id);
	if(!cap.isOpened())
	{
		printf("not opened %d\n",id);
		return -1;
	}
	else
		printf("opened %d\n",id);

	while(true)
	{
		cv::Mat img;
		cap >> img;
		printf("loaded %d %d %d\n",img.cols,img.rows,img.channels());
		if(img.rows != 0)
		{
			cv::imshow("ciao",img);
			int k = cv::waitKey(10);
			if(k != 0)
				break;
		}
	}
	return 0;
}
