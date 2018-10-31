#include "dlib.h"
#include "log.h"
#include <math.h>

#define MAX_SIZE 240

static bool checkTextureBuffer(char **buffer, int srcSize, int destSize) {
    if (srcSize != destSize) {
        if (buffer) {
            free(*buffer);
            *buffer = static_cast<char *>(malloc(sizeof(char) * destSize));
        }
        return true;
    }
    return false;
}

void Dlib::detectTexture(int *texture, int width, int height, int *rect, int *points) {
    clock_t t0, t1;
    int size = width * height * 4;
    if (checkTextureBuffer(&textureBuffer, textureBufferSize, size)) {
        textureBufferSize = size;
    }
    t0 = clock();
    glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(texture[0]));
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
    t1 = clock();
    LOGI("Read cost %f", (t1 - t0) / (double) CLOCKS_PER_SEC);
    samplingTexture(textureBuffer, width, height, sample, frame);
    detect(frame, rect);
    findFeature(frame, rect, points);
//    reduction(rect, points, sample[0]);
}

void Dlib::detectIntArray(int *src, int width, int height, int *rect, int *points) {
    //correlation_tracker
    sampling(src, width, height, sample, frame);
    detect(frame, rect);
    findFeature(frame, rect, points);
//    reduction(rect, points, sample[0]);
}

void Dlib::detect(array2d<unsigned char> *image, int *rect) {
    std::vector<rectangle> faceRect = detector(*image, 1);
    if (!faceRect.empty()) {
        rect[0] = faceRect[0].left();
        rect[1] = faceRect[0].top();
        rect[2] = faceRect[0].right();
        rect[3] = faceRect[0].bottom();
        LOGI("Find face (%d,%d)-(%d,%d)", rect[0], rect[1], rect[2], rect[3]);
    } else {
        rect[0] = 0;
        rect[1] = 0;
        rect[2] = 0;
        rect[3] = 0;
    }
}

void Dlib::findFeature(array2d<unsigned char> *image, int *rect, int *points) {
    std::vector<full_object_detection> shapes;
    shapes.push_back(featureModel(*image, rectangle(rect[0], rect[1], rect[2], rect[3])));
    if (!shapes.empty()) {
        for (int i = 0; i < 68; i++) {
            points[i * 2] = shapes[0].part(i).x();
            points[i * 2 + 1] = shapes[0].part(i).y();
        }
    }
}

void Dlib::reduction(int *rect, int *points, int sample) {
    rect[0] *= sample;
    rect[1] *= sample;
    rect[2] *= sample;
    rect[3] *= sample;
    for (int i = 0; i < 136; i++) {
        points[i] *= sample;
    }
}

void Dlib::samplingTexture(char *buffer, int width, int height, int *sample,
                           array2d<unsigned char> *dest) {
    sample[0] = 1;
    int *size = calculateSample(width, height, sample);
    int dest_width = size[0];
    int dest_height = size[1];
    dest->set_size(dest_height, dest_width);
    array2d<unsigned char>::iterator itr = dest->begin();
    for (int i = dest_height - 1; i >= 0; i--) {
        for (int j = 0; j < dest_width; j++) {
            int index = i * 4 * sample[0] * width + j * 4 * sample[0];
            int red = buffer[index];
            int green = buffer[index + 1];
            int blue = buffer[index + 2];
            unsigned char gray = static_cast<unsigned char>(red * 0.299 + green * 0.587 +
                                                            blue * 0.114);
            *itr = gray;
            itr++;
        }
    }
}

void Dlib::sampling(int *src, int width, int height, int *sample,
                    array2d<unsigned char> *dest) {
    sample[0] = 1;
    int *size = calculateSample(width, height, sample);
    int dest_width = size[0];
    int dest_height = size[1];
    dest->set_size(dest_height, dest_width);
    array2d<unsigned char>::iterator itr = dest->begin();
    for (int i = 0; i < dest_height; i++) {
        for (int j = 0; j < dest_width; j++) {
            int index = i * sample[0] * width + j * sample[0];
            int clr = src[index];
            int red = (clr & 0x00ff0000) >> 16; // 取高两位
            int green = (clr & 0x0000ff00) >> 8; // 取中两位
            int blue = clr & 0x000000ff; // 取低两位
            unsigned char gray = static_cast<unsigned char>(red * 0.299 + green * 0.587 +
                                                            blue * 0.114);
            *itr = gray;
            itr++;
        }
    }
}

int *Dlib::calculateSample(int width, int height, int *sample) {
    int max = width;
    sample[0] = max / MAX_SIZE;
    int dest_width = width / sample[0];
    int dest_height = height / sample[0];
    if (height > max) {
        max = height;
        sample[0] = max / MAX_SIZE;
        dest_width = width / sample[0];
        dest_height = height / sample[0];
    }
//    LOGI("sample=%d, %dx%d -> %dx%d", sample[0], width, height, dest_width, dest_height);
    if (sample[0] < 1) {
        sample[0] = 1;
        dest_width = width;
        dest_height = height;
//        LOGI("correct sample=%d, %dx%d -> %dx%d", sample, width, height, dest_width, dest_height);
    }
    int *result = new int[2];
    result[0] = dest_width;
    result[1] = dest_height;
    return result;
}

Dlib::Dlib(std::string filename) {
    sample = static_cast<int *>(malloc(sizeof(int)));
    faceTrackRect = static_cast<int *>(malloc(sizeof(int) * 4));
    memset(faceTrackRect, 0, 4);
    detector = get_frontal_face_detector();
    try {
        deserialize(filename) >> featureModel;
    } catch (serialization_error &e) {
        LOGE("You need dlib's default face landmarking model file to run this example.");
        LOGE("You can get it from the following URL: ");
        LOGE(" http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2");
        LOGE("%s", e.what());
    }
    catch (std::exception &e) {
        LOGE("%s", e.what());
    }
}

Dlib::~Dlib() {
    LOGI("release");
    if (sample) {
        free(sample);
        sample = NULL;
    }
    if (frame) {
        delete frame;
        frame = NULL;
    }
    if (faceTrackRect) {
        free(faceTrackRect);
        faceTrackRect = NULL;
    }
    textureBufferSize = 0;
    if (textureBuffer) {
        free(textureBuffer);
        textureBuffer = NULL;
    }
//    free(&detector);
//    free(&model);
}

bool Dlib::found() {
    return 0 != faceTrackRect[0]
           && 0 != faceTrackRect[1]
           && 0 != faceTrackRect[2]
           && 0 != faceTrackRect[3];
}

void Dlib::trackTexture(int *texture, int width, int height, int *rect, int *points) {
//    if (found()) {//如果找到人脸，则进行跟踪
//        glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(texture[0]));
//        glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);
//        glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
//        samplingTexture(textureBuffer, width, height, sample, frame);
//        tracker.update(*frame);
//        drectangle result = tracker.get_position();
//        rect[0] = result.left();
//        rect[1] = result.top();
//        rect[2] = result.right();
//        rect[3] = result.bottom();
//        findFeature(frame, rect, points);
//        reduction(rect, points, sample[0]);
////        LOGI("Track(%d,%d,%d,%d)", rect[0], rect[1], rect[2], rect[3]);
//        return;
//    }
    //否则搜索人脸位置
    detectTexture(texture, width, height, faceTrackRect, points);
    if (found()) {//一旦搜索到人脸，立刻进行跟踪
        rect[0] = faceTrackRect[0];
        rect[1] = faceTrackRect[1];
        rect[2] = faceTrackRect[2];
        rect[3] = faceTrackRect[3];
//        tracker.start_track(*frame, drectangle(faceTrackRect[0], faceTrackRect[1],
//                                               faceTrackRect[2], faceTrackRect[3]));
        reduction(rect, points, sample[0]);
    }
}
