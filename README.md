# gstreamer-tutorial
gstreamer 例程学习

一些常用的指令：
```bash
gst-launch-1.0 rtspsrc location=rtsp://admin:abcd1234@192.168.111.64:554/h264/ch1/main/av_stream ! udpsink sync=false host=127.0.0.1 port=5600
gst-launch-1.0 udpsrc port=5600 caps="application/x-rtp" ! rtph264depay ! avdec_h264 ! clockoverlay valignment=bottom ! autovideosink fps-update-interval=1000 sync=false
```
