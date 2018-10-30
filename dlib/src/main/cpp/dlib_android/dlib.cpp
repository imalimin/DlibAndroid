#include "dlib.h"
#include "log.h"
#include <math.h>

#define MAX_SIZE 240

void Dlib::detect(int *src, int width, int height, int *rect, int *points) {
    sample[0] = 1;
    clock_t t0, t1;
    t0 = clock();
    array2d<unsigned char> image = sampling(src, width, height, sample);
    t1 = clock();
    LOGI("Sample cost %f", (t1 - t0) / (double) CLOCKS_PER_SEC);
    std::vector<rectangle> dest = detector(image, 1);
    std::vector<full_object_detection> shapes;
    for (unsigned long i = 0; i < dest.size(); ++i)
        shapes.push_back(model(image, dest[i]));

    if (!dest.empty()) {
        rect[0] = dest[0].left() * sample[0];
        rect[1] = dest[0].top() * sample[0];
        rect[2] = dest[0].right() * sample[0];
        rect[3] = dest[0].bottom() * sample[0];
    }
    if (!shapes.empty()) {
        for (int i = 0; i < 68; i++) {
            points[i * 2] = shapes[0].part(i).x() * sample[0];
            points[i * 2 + 1] = shapes[0].part(i).y() * sample[0];
        }
    }
}

array2d<unsigned char> Dlib::sampling(int *src, int width, int height, int *sample) {
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
    LOGI("sample=%d, %dx%d -> %dx%d", sample[0], width, height, dest_width, dest_height);
    if (sample[0] < 1) {
        sample[0] = 1;
        dest_width = width;
        dest_height = height;
        LOGI("correct sample=%d, %dx%d -> %dx%d", sample, width, height, dest_width, dest_height);
    }
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

Dlib::Dlib() {
    sample = static_cast<int *>(malloc(sizeof(int)));
    detector = get_frontal_face_detector();
    try {
        deserialize("/sdcard/shape_predictor_68_face_landmarks.dat") >> model;
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
    }
//    free(&detector);
//    free(&model);
}
