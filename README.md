
# Objective

Virtual Camera for V4L using the v4l2loopback driver.

# Direct to v4l2 Example

https://github.com/umlaeute/v4l2loopback/blob/master/examples/test.c

# Installation

``` 
 sudo apt-get install v4l2loopback-dkms
 sudo apt-get install v4l2loopback-utils

 modprobe v4l2loopback devices=4
 chmod a+rwx /dev/video0

 v4l2-ctl -d /dev/video0 -l
 v4l2loopback-ctl set-fps 25 /dev/video0
```

# Test Write

``` 
gstreamer: gst-launch-1.0 videotestsrc ! v4l2sink device=/dev/video0
```
# Test Play

* Listing of device: v4l2-ctl --list-devices
* ffmpeg: ffmpeg -f v4l2 -list_formats all -i /dev/video0
* xawtv:  xawtv -c /dev/video0
