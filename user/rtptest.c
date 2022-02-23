#include "rtppipeline.h"
#include "stdio.h" 

#define WIDTH 1920
#define HEIGHT 1440

int main(void)
{
    unsigned char buf[WIDTH * HEIGHT * 3 / 2];
    int size = WIDTH * HEIGHT;
    int count = 0;
    testPipeline("127.0.0.1", 5600, 1920, 1440, 960, 720);
    // initRTPPipeline("127.0.0.1", 5600);
    while (1)
    {
        // printf("count = %d\n", count);
        if (count % 30 == 0)
        {
            if (count / 30 % 2 != 0)
            {
                memset(buf, 32, size);
                memset(&buf[size], 240, size / 4);
                memset(&buf[size + size / 4], 118, size / 4);
                printf("Blue.....\n");
            }
            else
            {
                memset(buf, 219, size);
                memset(&buf[size], 16, size / 4);
                memset(&buf[size + size / 4], 138, size / 4);
                printf("Yellow.....\n");
            }
        }
        count++;
        pushTestI420Data(buf, size);
        usleep(33*2000);
    }
    return 0;
}