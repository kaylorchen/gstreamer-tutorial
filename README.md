# gstreamer-tutorial
gstreamer 例程学习

一些常用的指令：
```bash
gst-launch-1.0 rtspsrc location=rtsp://admin:abcd1234@192.168.111.64:554/h264/ch1/main/av_stream ! udpsink sync=false host=127.0.0.1 port=5600
gst-launch-1.0 udpsrc port=5600 caps="application/x-rtp" ! rtph264depay ! avdec_h264 ! clockoverlay valignment=bottom ! autovideosink fps-update-interval=1000 sync=false
raspivid -t 0 -w 640 -h 480 -fps 20 -g 5 -b 2000000  -n -o - | gst-launch-1.0 -v fdsrc ! h264parse ! rtph264pay config-interval=1 pt=96 ! udpsink sync=false host=127.0.0.1 port=5600
```
启动服务
```bash
[Unit]
Description=WFB push rtp viedo stream
After=wifibroadcast@drone.service

[Service]
ExecStart=/bin/sh /usr/bin/video.sh
#ExecStop=killall video.sh

[Install]
WantedBy=wifibroadcast.service
```
