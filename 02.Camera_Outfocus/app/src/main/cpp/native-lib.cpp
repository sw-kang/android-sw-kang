#include <jni.h>
#include <string>


jint* getOutFocus( jint image[], int width, int height);

extern "C"
JNIEXPORT jintArray JNICALL
Java_test_example_com_ConnectNdkCamera_MainActivity_decodeYUV420SPndk(JNIEnv *env, jobject instance, jbyteArray image_, jint width, jint height) {
    jbyte *image = env->GetByteArrayElements(image_, NULL);
    env->ReleaseByteArrayElements(image_, image, 0);

    int frameSize = width * height;
    jint rgb[frameSize] ;

    for (int j = 0, yp = 0; j < height; j++) {
        int uvp = frameSize + (j >> 1) * width, u = 0, v = 0;
        for (int i = 0; i < width; i++, yp++) {
            int y = (0xff & ((int) image[yp])) - 16;
            if (y < 0) y = 0;
            if ((i & 1) == 0) {
                v = (0xff & image[uvp++]) - 128;
                u = (0xff & image[uvp++]) - 128;
            }

            int y1192 = 1192 * y;
            int r = (y1192 + 1634 * v);
            int g = (y1192 - 833 * v - 400 * u);
            int b = (y1192 + 2066 * u);

            if (r < 0) r = 0; else if (r > 262143) r = 262143;
            if (g < 0) g = 0; else if (g > 262143) g = 262143;
            if (b < 0) b = 0; else if (b > 262143) b = 262143;

            rgb[(i*height)+(height-1-j)] = 0xff000000 | ((r << 6) & 0xff0000) | ((g >> 2) & 0xff00) | ((b >> 10) & 0xff);
        }
    }



    jintArray rgbarray = env->NewIntArray(frameSize);
    env->SetIntArrayRegion(rgbarray, 0, frameSize, getOutFocus( rgb, height, width));
    return rgbarray;
}


jint* getOutFocus( jint image[], jint width, jint height){

    int frameSize = width * height;
    jint rgb[frameSize];


    int table[ 9 ] = {  0 , -1 ,   0
                      , -1 , 5  , -1
                      ,  0 , -1 ,  0 };

    jint result = 0;
    int count = 0;

    for(int y = 0 ; y < height ; y ++)
    {
        for (int x = 0; x < width; x++)
        {
            if(  (((y- height/2) *(y - height/2) + (x - width/2) *( x - width/2) ) > ((width/2)*(width/2)) ))
            {
                jint r = 0;
                jint g = 0;
                jint b = 0;
                count = 0;

                if (y - 1 >= 0) {
                    result = image[x + ((y - 1) * width)];
                    r += (result & 0x00ff0000) >> 16;
                    g += (result & 0x0000ff00) >> 8;
                    b += result & 0x000000ff;
                    count++;
                }
                if (x - 1 >= 0) {
                    result = image[(x - 1) + (y * width)];
                    r += (result & 0x00ff0000) >> 16;
                    g += (result & 0x0000ff00) >> 8;
                    b += result & 0x000000ff;
                    count++;
                }

                if (y + 1 < height) {
                    result = image[x + ((y + 1) * width)];
                    r += (result & 0x00ff0000) >> 16;
                    g += (result & 0x0000ff00) >> 8;
                    b += result & 0x000000ff;
                    count++;
                }
                if (x + 1 < width) {
                    result = image[(x + 1) + (y * width)];
                    r += (result & 0x00ff0000) >> 16;
                    g += (result & 0x0000ff00) >> 8;
                    b += result & 0x000000ff;
                    count++;
                }
                result = image[x + (y * width)];
                r += ((result & 0x00ff0000) >> 16);
                r /=  (count + 1);
                g += ((result & 0x0000ff00) >> 8);
                g /=  (count + 1);
                b += (result & 0x000000ff);
                b /=  (count + 1);

                if (r < 0) r = 0;
                if (g < 0) g = 0;
                if (b < 0) b = 0;
                if (r > 255) r = 255;
                if (g > 255) g = 255;
                if (b > 255) b = 255;

                rgb[x + (y * width)] = 0xff000000 | (r << 16) | (g << 8) | b;


            }

            else if(  (((y- height/2) *(y - height/2) + (x - width/2) *( x - width/2) ) > ((width/2 - 5)*(width/2 -5)) ))
            {
                //rgb[x + (y*width)] =  image[x + (y*width)];
                rgb[x + (y*width)] = 0xffff0000;

            }
            else {


                jint r = 0;
                jint g = 0;
                jint b = 0;
                count = 0;

                if (y - 1 >= 0) {
                    result = image[x + ((y - 1) * width)];
                    r -= (result & 0x00ff0000) >> 16;
                    g -= (result & 0x0000ff00) >> 8;
                    b -= result & 0x000000ff;
                    count++;
                }
                if (x - 1 >= 0) {
                    result = image[(x - 1) + (y * width)];
                    r -= (result & 0x00ff0000) >> 16;
                    g -= (result & 0x0000ff00) >> 8;
                    b -= result & 0x000000ff;
                    count++;
                }

                if (y + 1 < height) {
                    result = image[x + ((y + 1) * width)];
                    r -= (result & 0x00ff0000) >> 16;
                    g -= (result & 0x0000ff00) >> 8;
                    b -= result & 0x000000ff;
                    count++;
                }
                if (x + 1 < width) {
                    result = image[(x + 1) + (y * width)];
                    r -= (result & 0x00ff0000) >> 16;
                    g -= (result & 0x0000ff00) >> 8;
                    b -= result & 0x000000ff;
                    count++;
                }
                result = image[x + (y * width)];
                r += ((result & 0x00ff0000) >> 16) * (count + 1);
                g += ((result & 0x0000ff00) >> 8) * (count + 1);
                b += (result & 0x000000ff) * (count + 1);

                if (r < 0) r = 0;
                if (g < 0) g = 0;
                if (b < 0) b = 0;
                if (r > 255) r = 255;
                if (g > 255) g = 255;
                if (b > 255) b = 255;

                rgb[x + (y * width)] = 0xff000000 | (r << 16) | (g << 8) | b;

            }

            }
    }

    return rgb;

}