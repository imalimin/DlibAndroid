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
    glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(3));
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
//    LOGI("Read cost %f, (%d, %d, %d, %d)", (t1 - t0) / (double) CLOCKS_PER_SEC,
//         textureBuffer[0], textureBuffer[1], textureBuffer[2], textureBuffer[3]);
    sample[0] = 1;
    detect(samplingTexture(textureBuffer, width, height, sample), sample[0], rect, points);
}

void Dlib::detectIntArray(int *src, int width, int height, int *rect, int *points) {
    //correlation_tracker
    sample[0] = 1;
    detect(sampling(src, width, height, sample), sample[0], rect, points);
}

array2d<unsigned char> Dlib::samplingTexture(char *buffer, int width, int height, int *sample) {
    int *size = calculateSample(width, height, sample);
    int dest_width = size[0];
    int dest_height = size[1];
    array2d<unsigned char> image;
    image.set_size(dest_height, dest_width);
    for (int i = dest_height - 1; i <= 0; i++) {
        for (int j = dest_width - 1; j <= 0; j++) {
            int index = i * 4 * sample[0] * width + j * sample[0];
            int red = buffer[index - 3];
            int green = buffer[index - 2];
            int blue = buffer[index - 1];
            unsigned char gray = static_cast<unsigned char>(red * 0.299 + green * 0.587 +
                                                            blue * 0.114);
            image[i][j] = gray;
        }
    }
    return image;
}

void Dlib::detect(array2d<unsigned char> image, int sample, int *rect, int *points) {
    clock_t t0, t1;
    t1 = clock();
    std::vector<rectangle> faceRect = detector(image, 1);
    t0 = clock();
//    LOGI("Face rect cost %f", (t0 - t1) / (double) CLOCKS_PER_SEC);
    std::vector<full_object_detection> shapes;
    for (unsigned long i = 0; i < faceRect.size(); ++i)
        shapes.push_back(model(image, faceRect[i]));

    if (!faceRect.empty()) {
        rect[0] = faceRect[0].left() * sample;
        rect[1] = faceRect[0].top() * sample;
        rect[2] = faceRect[0].right() * sample;
        rect[3] = faceRect[0].bottom() * sample;
        LOGI("Find face (%d,%d)-(%d,%d)", rect[0], rect[1], rect[2], rect[3]);
    }
    if (!shapes.empty()) {
        for (int i = 0; i < 68; i++) {
            points[i * 2] = shapes[0].part(i).x() * sample;
            points[i * 2 + 1] = shapes[0].part(i).y() * sample;
        }
    }
}

array2d<unsigned char> Dlib::sampling(int *src, int width, int height, int *sample) {
    int *size = calculateSample(width, height, sample);
    int dest_width = size[0];
    int dest_height = size[1];
    array2d<unsigned char> image;
    image.set_size(dest_height, dest_width);
    for (int i = 0; i < dest_height; i++) {
        for (int j = 0; j < dest_width; j++) {
            int index = i * sample[0] * width + j * sample[0];
            int clr = src[index];
            int red = (clr & 0x00ff0000) >> 16; // 取高两位
            int green = (clr & 0x0000ff00) >> 8; // 取中两位
            int blue = clr & 0x000000ff; // 取低两位
            unsigned char gray = static_cast<unsigned char>(red * 0.299 + green * 0.587 +
                                                            blue * 0.114);
            //rgb_pixel pt(red,green,blue);
            image[i][j] = gray;
        }
    }
    return image;
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
    detector = get_frontal_face_detector();
    try {
        deserialize(filename) >> model;
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
    textureBufferSize = 0;
    if (textureBuffer) {
        free(textureBuffer);
        textureBuffer = NULL;
    }
//    free(&detector);
//    free(&model);
}
