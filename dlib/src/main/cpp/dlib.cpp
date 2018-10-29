#include <jni.h>
#include <string>
#include <vector>
#include <iostream>
#include <ctime>

#include <dlib/image_io.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

#ifdef __cplusplus
extern "C" {
#endif

dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

//获取人脸框
std::vector<int> get_face_rect(const std::vector<int> img, int width, int height) {
    dlib::array2d<unsigned char> image;
    image.set_size(height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int clr = img[i * width + j];
            int red = (clr & 0x00ff0000) >> 16; // 取高两位
            int green = (clr & 0x0000ff00) >> 8; // 取中两位
            int blue = clr & 0x000000ff; // 取低两位
            unsigned char gray = static_cast<unsigned char>(red * 0.299 + green * 0.587 +
                                                            blue * 0.114);
            //dlib::rgb_pixel pt(red,green,blue);
            image[i][j] = gray;
        }
    }


    clock_t begin = clock();
    std::vector<dlib::rectangle> dest = detector(image, 1);


    std::vector<int> rect;
    if (!dest.empty()) {
        rect.push_back(dest[0].left());
        rect.push_back(dest[0].top());
        rect.push_back(dest[0].right());
        rect.push_back(dest[0].bottom());
    }
    return rect;

}

JNIEXPORT jintArray JNICALL
Java_com_lmy_dlib_Dlib_detectFaceRect(
        JNIEnv *env,
        jobject /* this */,
        jintArray image,
        jint width,
        jint height) {
    std::vector<int> image_datacpp(static_cast<unsigned int>(height * width));
    jsize len = env->GetArrayLength(image);
    jint *body = env->GetIntArrayElements(image, 0);
    for (jsize i = 0; i < len; i++) {
        image_datacpp[i] = (int) body[i];
    }

    std::vector<int> rect = get_face_rect(image_datacpp, width, height);
    jintArray result = env->NewIntArray(4);
    env->SetIntArrayRegion(result, 0, 4, &rect[0]);
    env->ReleaseIntArrayElements(image, body, NULL);
    return result;
}

JNIEXPORT jstring JNICALL
Java_com_lmy_dlib_Dlib_helloWorld(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

#ifdef __cplusplus
}
#endif
