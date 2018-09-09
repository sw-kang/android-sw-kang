#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring

JNICALL
Java_test_example_com_ConnectNdkCamera_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    int test[1234567];
    for(int i = 0 ; i < 1234567 ; i++)
    {
        test[i] = 0;
    }
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT jintArray JNICALL
Java_test_example_com_ConnectNdkCamera_MainActivity_decodeYUV420SPndk(JNIEnv *env, jobject instance, jbyteArray image_, jint width, jint height) {
    jbyte *image = env->GetByteArrayElements(image_, NULL);
    env->ReleaseByteArrayElements(image_, image, 0);

    int frameSize = width * height;
    jint rgb[frameSize];

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

    for(int y = 0 ; y < height ; y ++){
        for (int x = 0; x < width; x++)
        {
            int r = (rgb[x + (y*width)] & 0x00ff0000) >> 16;
            int g = (rgb[x + (y*width)] & 0x0000ff00) >> 8;
            int b = (rgb[x + (y*width)] & 0x000000ff);

            if( r > 200 && g < 100 && b < 100)
                rgb[x + (y*width)] = 0xffff00ff;
        }

    }



        jintArray rgbarray = env->NewIntArray(frameSize);
    env->SetIntArrayRegion(rgbarray, 0, frameSize, rgb);
    return rgbarray;
}