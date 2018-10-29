package com.lmy.dlib

/**
 * Created by lmyooyo@gmail.com on 2018/10/29.
 */
class Dlib {
    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun helloWorld(): String

    external fun detect(image: IntArray, width: Int, height: Int,
                                rect: IntArray, points: IntArray)

    external fun release()

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("dlib-android")
        }
    }
}