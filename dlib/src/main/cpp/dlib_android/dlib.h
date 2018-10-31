#include <string>
#include <vector>
#include <iostream>
#include <ctime>
#include <GLES2/gl2.h>

#include <dlib/image_io.h>
#include <dlib/image_processing.h>
#include <dlib/image_processing/frontal_face_detector.h>

using namespace dlib;

#ifndef DLIBANDROID_DLIB_H
#define DLIBANDROID_DLIB_H

class Dlib {
public:
    Dlib(std::string filename);

    ~Dlib();

    void detectIntArray(int *src, int width, int height, int *rect, int *points);

    void detectTexture(int *texture, int width, int height, int *rect, int *points);

    void trackTexture(int *texture, int width, int height, int *rect, int *points);

private:
    frontal_face_detector detector;
    shape_predictor featureModel;
    correlation_tracker tracker;
    array2d<unsigned char> *frame = new array2d<unsigned char>();
    int *sample = NULL;
    int textureBufferSize = 0;
    char *textureBuffer = NULL;
    int *faceTrackRect = NULL;

    void detect(array2d<unsigned char> *image, int *rect);

    void findFeature(array2d<unsigned char> *image, int *rect, int *points);

    void sampling(int *src, int width, int height, int *sample,
                  array2d<unsigned char> *dest);

    void samplingTexture(char *buffer, int width, int height, int *sample,
                         array2d<unsigned char> *dest);

    int *calculateSample(int width, int height, int *sample);

    void reduction(int *rect, int *points, int sample);

    bool found();
};

#endif //DLIBANDROID_DLIB_H
