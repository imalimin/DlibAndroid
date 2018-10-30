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
    clock_t t0, t1;
    t0 = clock();
    if (!detector) {
        detector = new Dlib();
    }
    t1 = clock();
    LOGI("Init cost %f", (t1 - t0) / (double) CLOCKS_PER_SEC);
    jint *imagePtr = env->GetIntArrayElements(image, 0);
    jint *rectPtr = env->GetIntArrayElements(rect, 0);
    jint *pointsPtr = env->GetIntArrayElements(points, 0);

    detector->detect(imagePtr, width, height, rectPtr, pointsPtr);

    env->ReleaseIntArrayElements(image, imagePtr, NULL);
    env->ReleaseIntArrayElements(rect, rectPtr, NULL);
    env->ReleaseIntArrayElements(points, pointsPtr, NULL);
    t0 = clock();
    LOGI("Detect cost %f", (t0 - t1) / (double) CLOCKS_PER_SEC);
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