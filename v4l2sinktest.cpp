#include "v4l2sink.hpp"
#include "opencv2/opencv.hpp"
#include <stdio.h>

int main(int argc, char**argv)
{
	if(argc < 3) 
	{
		printf("test needs: device image\n");
		return -1;
	}

	v4l2sink sink;

	if(!sink.open(argv[1]))
	{
		printf("Failed opening\n");
		return -1;
	}

	cv::Mat img = cv::imread(argv[2],1); // >0 RGB, <0 AS IS, 0 GREY
	if(img.rows == 0)
	{
		printf("Cannot open %s\n",argv[2]);
		return -1;
	}
	printf("Opened %s with %d %d channels %d\n",argv[2],img.cols,img.rows,(int)img.channels());

	int r = sink.init(img.cols,img.rows,img.channels() == 1 ? v4l2sink::GRAY: v4l2sink::RGB);
	printf("init %d\n",r);
	r = sink.write((const char*)img.data,img.cols*img.rows*img.channels());
	printf("Written %d\n",r);
	getchar();
	return 0;
}
