//
// Created by kaylor on 2022/1/31.
//

#ifndef SRC_RTPPIPELINE_H
#define SRC_RTPPIPELINE_H
#include "libavutil/frame.h"
int initRTPPipeline(const char *ip, int port);
int testPipeline(const char *ip, int port, int iwidth, int iheight, int owidth, int oheight);
void pushRTPStreamRGB(unsigned char *streamdata, int dataLen);
void pushRTPStreamI420(AVFrame* pFrameYUV, int size);
void pushTestI420Data(unsigned char *I420data, int size);
#endif //SRC_RTPPIPELINE_H
