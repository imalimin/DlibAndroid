package com.lmy.dlib

/**
 * Created by lmyooyo@gmail.com on 2018/10/29.
 */
class Dlib {
    external fun init(filename: String)

    /**
     * For test
     */
    external fun samplingTexture(texture: IntArray, width: Int, height: Int,
                                 dest: IntArray, dw: IntArray, dh: IntArray)

    external fun trackTexture(texture: IntArray, width: Int, height: Int,
                               rect: IntArray, points: IntArray)

    external fun detect(image: IntArray, width: Int, height: Int,
                        rect: IntArray, points: IntArray)

    external fun detectTexture(texture: IntArray, width: Int, height: Int,
                               rect: IntArray, points: IntArray)

    external fun release()

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("dlib-android")
        }
    }
}