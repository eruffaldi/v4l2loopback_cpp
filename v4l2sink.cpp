#include "v4l2sink.hpp"
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>



#define ROUND_UP_2(num)  (((num)+1)&~1)
#define ROUND_UP_4(num)  (((num)+3)&~3)
#define ROUND_UP_8(num)  (((num)+7)&~7)
#define ROUND_UP_16(num) (((num)+15)&~15)
#define ROUND_UP_32(num) (((num)+31)&~31)
#define ROUND_UP_64(num) (((num)+63)&~63)


#if 0
# define CHECK_REREAD
#endif
void print_format(struct v4l2_format*vid_format) {
  printf("	vid_format->type                =%d\n",	vid_format->type );
  printf("	vid_format->fmt.pix.width       =%d\n",	vid_format->fmt.pix.width );
  printf("	vid_format->fmt.pix.height      =%d\n",	vid_format->fmt.pix.height );
  printf("	vid_format->fmt.pix.pixelformat =%d\n",	vid_format->fmt.pix.pixelformat);
  printf("	vid_format->fmt.pix.sizeimage   =%d\n",	vid_format->fmt.pix.sizeimage );
  printf("	vid_format->fmt.pix.field       =%d\n",	vid_format->fmt.pix.field );
  printf("	vid_format->fmt.pix.bytesperline=%d\n",	vid_format->fmt.pix.bytesperline );
  printf("	vid_format->fmt.pix.colorspace  =%d\n",	vid_format->fmt.pix.colorspace );
}

//# define FRAME_FORMAT V4L2_PIX_FMT_YUYV
//# define FRAME_FORMAT V4L2_PIX_FMT_YVU420
//# define FRAME_FORMAT V4L2_


static bool format_properties(v4l2sink::Format f,
		const unsigned int width,
		const unsigned int height,
		size_t& format,
		size_t &linewidth,
		size_t &framewidth) {
	
	switch(f) {
		case v4l2sink::GRAY:
			format = V4L2_PIX_FMT_GREY;
			linewidth = width; /* ??? */
			framewidth = width*height;
			break;
		case v4l2sink::YUV420:
			format = V4L2_PIX_FMT_YUV420;
			linewidth = width; /* ??? */
			framewidth = ROUND_UP_4 (width) * ROUND_UP_2 (height);
			framewidth += 2 * ((ROUND_UP_8 (width) / 2) * (ROUND_UP_2 (height) / 2));
			break;
		case v4l2sink::YUYV:
			/*	case V4L2_PIX_FMT_UYVY: 
			case V4L2_PIX_FMT_Y41P: 
			case V4L2_PIX_FMT_YUYV: 
			case V4L2_PIX_FMT_YVYU:
			*/
			format = V4L2_PIX_FMT_YUYV;
			linewidth = (ROUND_UP_2 (width) * 2);
			framewidth = linewidth * height;
			break;
		case v4l2sink::RGB:
			format = V4L2_PIX_FMT_RGB24;
			linewidth = width;
			framewidth = linewidth * height * 3;
			break;
			/*
		case v4l2sink::RGBA:
			format = V4L2_PIX_FMT_ARGB32;
			lw = width;
			fw = lw * height * 4;
			break;
			*/
		default:
			return false;
	}
	return true;
}

struct v4l2sinkpimpl
{
	v4l2sinkpimpl(int fd): fdwr(fd) {}
	struct v4l2_format vid_format;
	size_t framesize;
	size_t linewidth;

	int fdwr = 0;

	void reset()
	{
		if(fdwr)
		{
			close(fdwr);
			fdwr = 0;
		}
	}

	~v4l2sinkpimpl() { if(fdwr) close(fdwr); }
};


bool v4l2sink::open(const char * name)
{
	if(pimpl_)
	{
		delete pimpl_;
	}
	int fdwr = ::open(name, O_RDWR);
	if(fdwr)
	{
		pimpl_ = new v4l2sinkpimpl(fdwr);
		return true;
	}
	else
		return false;
}

bool v4l2sink::init(int w, int h, Format format)
{
	struct v4l2_capability vid_caps;
	if(!pimpl_)
		return false;
	int ret_code = ioctl(pimpl_->fdwr, VIDIOC_QUERYCAP, &vid_caps);
	if(ret_code == -1)
		return false;
	size_t framesize = 0,linewidth = 0,oformat = 0;
	if(!format_properties(format,w,h,  oformat, linewidth,framesize))
		return false;
	printf("Asked %d %d with %d -> fmt:%d lw:%d fs:%d\n",w,h,format,(int)oformat,(int)linewidth,(int)framesize);
	struct v4l2_format vid_format;
	if(ioctl(pimpl_->fdwr, VIDIOC_G_FMT, &vid_format) != -1)
	{
		printf("Before\n");
		print_format(&vid_format);
	}
	memset(&vid_format,0,sizeof(vid_format));
	vid_format.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	vid_format.fmt.pix.width = w;
	vid_format.fmt.pix.height = h;
	vid_format.fmt.pix.pixelformat = oformat;
	vid_format.fmt.pix.sizeimage = framesize;
	vid_format.fmt.pix.field = V4L2_FIELD_NONE;
	vid_format.fmt.pix.bytesperline = linewidth;
	vid_format.fmt.pix.colorspace = V4L2_COLORSPACE_SRGB;
	printf("Asked\n");
	print_format(&vid_format);

	if(ioctl(pimpl_->fdwr, VIDIOC_S_FMT, &vid_format)==-1)
		return false;

	if(ioctl(pimpl_->fdwr, VIDIOC_G_FMT, &vid_format) != -1)
	{
		printf("After\n");
		print_format(&vid_format);
	}

	return true;
}

v4l2sink::~v4l2sink()
{
	delete pimpl_;
}

bool v4l2sink::write(const char * data, int size)
{
	if(pimpl_)
	{
		return ::write(pimpl_->fdwr, data, size) == size;
	}
	else
		return false;
}

void v4l2sink::close()
{
	pimpl_->reset();
}
