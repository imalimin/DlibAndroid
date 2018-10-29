#include "dlib.h"

void Dlib::detect(const std::vector<int> src, int width, int height, int *rect, int *points) {
    array2d<unsigned char> image;
    image.set_size(height, width);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int clr = src[i * width + j];
            int red = (clr & 0x00ff0000) >> 16; // 取高两位
            int green = (clr & 0x0000ff00) >> 8; // 取中两位
            int blue = clr & 0x000000ff; // 取低两位
            unsigned char gray = static_cast<unsigned char>(red * 0.299 + green * 0.587 +
                                                            blue * 0.114);
            //rgb_pixel pt(red,green,blue);
            image[i][j] = gray;
        }
    }
    try {
        deserialize("/sdcard/shape_predictor_68_face_landmarks.dat") >> model;
    } catch (serialization_error &e) {
        std::cout << "You need dlib's default face landmarking model file to run this example."
                  << std::endl;
        std::cout << "You can get it from the following URL: " << std::endl;
        std::cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2"
                  << std::endl;
        std::cout << std::endl << e.what() << std::endl;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
    std::vector<rectangle> dest = detector(image, 1);
    std::vector<full_object_detection> shapes;
    for (unsigned long i = 0; i < dest.size(); ++i)
        shapes.push_back(model(image, dest[i]));

    if (!dest.empty()) {
        rect[0] = dest[0].left();
        rect[1] = dest[0].top();
        rect[2] = dest[0].right();
        rect[3] = dest[0].bottom();
    }
    if (!shapes.empty()) {
        for (int i = 0; i < 68; i++) {
            points[i * 2] = shapes[0].part(i).x();
            points[i * 2 + 1] = shapes[0].part(i).y();
        }
    }
}
