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
Java_com_lmy_dlib_Dlib_init(
        JNIEnv *env,
        jobject /* this */,
        jstring filename) {
    clock_t t0, t1;
    t0 = clock();
    char *filenamePtr = (char *) env->GetStringUTFChars(filename, NULL);
    if (!detector) {
        detector = new Dlib(filenamePtr);
    }
    env->ReleaseStringUTFChars(filename, filenamePtr);
    t1 = clock();
    LOGI("Init cost %f", (t1 - t0) / (double) CLOCKS_PER_SEC);
}

JNIEXPORT void JNICALL
Java_com_lmy_dlib_Dlib_detectTexture(
        JNIEnv *env,
        jobject /* this */,
        jintArray texture,
        jint width,
        jint height,
        jintArray rect,
        jintArray points) {
    jint *texturePtr = env->GetIntArrayElements(texture, 0);
    jint *rectPtr = env->GetIntArrayElements(rect, 0);
    jint *pointsPtr = env->GetIntArrayElements(points, 0);

    detector->detectTexture(texturePtr, width, height, rectPtr, pointsPtr);

    env->ReleaseIntArrayElements(texture, texturePtr, NULL);
    env->ReleaseIntArrayElements(rect, rectPtr, NULL);
    env->ReleaseIntArrayElements(points, pointsPtr, NULL);
}

JNIEXPORT void JNICALL
Java_com_lmy_dlib_Dlib_trackTexture(
        JNIEnv *env,
        jobject /* this */,
        jintArray texture,
        jint width,
        jint height,
        jintArray rect,
        jintArray points){
    clock_t t0, t1;
    t0 = clock();
    jint *texturePtr = env->GetIntArrayElements(texture, 0);
    jint *rectPtr = env->GetIntArrayElements(rect, 0);
    jint *pointsPtr = env->GetIntArrayElements(points, 0);

    detector->trackTexture(texturePtr, width, height, rectPtr, pointsPtr);

    env->ReleaseIntArrayElements(texture, texturePtr, NULL);
    env->ReleaseIntArrayElements(rect, rectPtr, NULL);
    env->ReleaseIntArrayElements(points, pointsPtr, NULL);
    t1 = clock();
    LOGI("Track cost %f", (t1 - t0) / (double) CLOCKS_PER_SEC);
}

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
    jint *imagePtr = env->GetIntArrayElements(image, 0);
    jint *rectPtr = env->GetIntArrayElements(rect, 0);
    jint *pointsPtr = env->GetIntArrayElements(points, 0);

    detector->detectIntArray(imagePtr, width, height, rectPtr, pointsPtr);

    env->ReleaseIntArrayElements(image, imagePtr, NULL);
    env->ReleaseIntArrayElements(rect, rectPtr, NULL);
    env->ReleaseIntArrayElements(points, pointsPtr, NULL);
    t1 = clock();
    LOGI("Detect cost %f", (t1 - t0) / (double) CLOCKS_PER_SEC);
}

JNIEXPORT void JNICALL
Java_com_lmy_dlib_Dlib_samplingTexture(
        JNIEnv *env,
        jobject /* this */,
        jintArray texture,
        jint width,
        jint height,
        jintArray dest,
        jintArray dw,
        jintArray dh) {
    jint *texturePtr = env->GetIntArrayElements(texture, 0);
    jint *destPtr = env->GetIntArrayElements(dest, 0);
    jint *dwPtr = env->GetIntArrayElements(dw, 0);
    jint *dhPtr = env->GetIntArrayElements(dh, 0);

//    array2d<unsigned char> image = detector->detectTexture(texturePtr, width, height, NULL, NULL);
//    array2d<unsigned char>::iterator itr;
//    int i = 0;
//    for (itr = image.begin(); itr != image.end(); itr++) {
//        destPtr[i] = *itr;
//        ++i;
//    }

    env->ReleaseIntArrayElements(texture, texturePtr, NULL);
    env->ReleaseIntArrayElements(dest, destPtr, NULL);
    env->ReleaseIntArrayElements(dw, dwPtr, NULL);
    env->ReleaseIntArrayElements(dh, dhPtr, NULL);
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