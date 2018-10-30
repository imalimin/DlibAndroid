package com.lmy.dlib.sample

import android.graphics.RectF
import android.os.AsyncTask
import android.os.Environment
import com.lmy.codec.texture.impl.sticker.BaseSticker
import com.lmy.dlib.Dlib
import java.io.File

/**
 * Created by lmyooyo@gmail.com on 2018/10/30.
 */
class FaceFilter(width: Int = 0,
                 height: Int = 0,
                 textureId: IntArray = IntArray(1)) : BaseSticker(width, height, textureId) {
    private val rect = RectF()
    private var inited = false
    private val dlib = Dlib()
    private val faceRect = IntArray(4)
    private val points = IntArray(68 * 2)
    private var initTask: InitTask? = null

    init {
        initTask = InitTask(this)
        initTask?.execute()
    }

    private fun initDlib() {
        if (inited) return
        dlib.init(File(Environment.getExternalStorageDirectory(),
                "shape_predictor_68_face_landmarks.dat").absolutePath)
        inited = true
    }

    override fun draw(transformMatrix: FloatArray?) {

    }

    override fun getRect(): RectF = rect
    override fun release() {
        super.release()
        dlib.release()
        initTask?.cancel(true)
    }

    private class InitTask(private val filter: FaceFilter) : AsyncTask<Void, Void, Void?>() {
        override fun doInBackground(vararg params: Void?): Void? {
            filter.initDlib()
            return null
        }

        override fun onPostExecute(result: Void?) {
            super.onPostExecute(result)
        }
    }
}