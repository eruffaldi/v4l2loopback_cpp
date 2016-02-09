/// Emanuele Ruffaldi 2015
#pragma once

#include <memory>

struct v4l2sinkpimpl;

/// easy access to v4l2loopback sink
class v4l2sink
{
public:
	~v4l2sink();

	enum Format { GRAY, RGB,RGBA, YUYV,YUV420};

	/// open device
	bool open(const char * name);

	/// setup device format
	bool init(int w, int h, Format format);

	/// writes frame given size
	bool write(const char * data, int size);

	/// closes
	void close();

	v4l2sinkpimpl * pimpl_ = 0;
};