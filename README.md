
#Objective

Virtual Camera using v4l2loopback


#Direct to v4l2

https://github.com/umlaeute/v4l2loopback/blob/master/examples/test.c


#Installation

sudo apt-get install v4l2loopback-dkms
sudo apt-get install v4l2loopback-utils

modprobe v4l2loopback devices=4
chmod a+rwx /dev/video0

#Test

v4l2-ctl --list-devices
ffmpeg -f v4l2 -list_formats all -i /dev/video0

Play
xawtv -c /dev/video0
