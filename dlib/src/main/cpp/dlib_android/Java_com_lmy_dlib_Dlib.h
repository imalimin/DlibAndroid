//
// Created by lmyooyo@gmail.com on 2018/10/29.
//
#include <jni.h>
#include <vector>

#ifndef DLIBANDROID_JAVA_COM_LMY_DLIB_DLIB_H
#define DLIBANDROID_JAVA_COM_LMY_DLIB_DLIB_H
#endif //DLIBANDROID_JAVA_COM_LMY_DLIB_DLIB_H

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_lmy_dlib_Dlib_detect(
        JNIEnv *,
        jobject,
        jintArray,
        jint,
        jint,
        jintArray,
        jintArray);

JNIEXPORT void JNICALL
Java_com_lmy_dlib_Dlib_release(
        JNIEnv *,
        jobject);

#ifdef __cplusplus
}
#endif
