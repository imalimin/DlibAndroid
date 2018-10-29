//
// Created by lmyooyo@gmail.com on 2018/10/29.
//
#include "Java_com_lmy_dlib_Dlib.h"
#include "dlib.h"

static Dlib *detector = NULL;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_lmy_dlib_Dlib_detect(
        JNIEnv *env,
        jobject /* this */,
        jintArray image,
        jint width,
        jint height,
        jintArray rect,
        jintArray points) {
    if (!detector) {
        detector = new Dlib();
    }
    jsize len = env->GetArrayLength(image);
    jint *imagePtr = env->GetIntArrayElements(image, 0);
    jint *rectPtr = env->GetIntArrayElements(rect, 0);
    jint *pointsPtr = env->GetIntArrayElements(points, 0);

    std::vector<int> image_datacpp(static_cast<unsigned int>(height * width));
    for (jsize i = 0; i < len; i++) {
        image_datacpp[i] = (int) imagePtr[i];
    }

    detector->detect(image_datacpp, width, height, rectPtr, pointsPtr);
    env->ReleaseIntArrayElements(image, imagePtr, NULL);
    env->ReleaseIntArrayElements(rect, rectPtr, NULL);
    env->ReleaseIntArrayElements(points, pointsPtr, NULL);
}

JNIEXPORT void JNICALL
Java_com_lmy_dlib_Dlib_release(
        JNIEnv *env,
        jobject /* this */) {
    if (detector)
        delete detector;
}

#ifdef __cplusplus
}
#endif