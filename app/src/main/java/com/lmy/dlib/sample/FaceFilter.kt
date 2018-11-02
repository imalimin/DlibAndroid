package com.lmy.dlib.sample

import android.graphics.*
import android.os.AsyncTask
import android.os.Environment
import android.widget.ImageView
import com.lmy.codec.texture.impl.sticker.BaseSticker
import com.lmy.dlib.Dlib
import java.io.File
import java.io.FileOutputStream

/**
 * Created by lmyooyo@gmail.com on 2018/10/30.
 */
class FaceFilter(private val imageView: ImageView,
                 width: Int = 0,
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

    override fun init() {
        super.init()
        bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
    }

    override fun draw(transformMatrix: FloatArray?) {
        if (inited) {
            dlib.trackTexture(textureId, width, height, faceRect, points)
            show()
        }
    }

    private var bitmap: Bitmap? = null
    private var canvas: Canvas? = null
    private lateinit var paint: Paint
    private fun show() {
        imageView.post {
            if (null == canvas) {
                canvas = Canvas(bitmap)
                paint = Paint(Paint.ANTI_ALIAS_FLAG)
                paint.color = Color.RED
                paint.style = Paint.Style.STROKE
                paint.strokeWidth = 3f
            }
            paint.xfermode = PorterDuffXfermode(PorterDuff.Mode.CLEAR)
            paint.color = Color.RED
            paint.style = Paint.Style.STROKE
            canvas?.drawPaint(paint)
            paint.xfermode = PorterDuffXfermode(PorterDuff.Mode.SRC)
            canvas?.drawRect(Rect(faceRect[0], faceRect[1], faceRect[2], faceRect[3]), paint)
            canvas?.drawRect(Rect(0, 0, bitmap!!.width, bitmap!!.height), paint)
            val points = FloatArray(this.points.size)
            for (i in 4 until this.points.size) {
                points[i - 4] = this.points[i].toFloat()
            }
            paint.color = Color.GREEN
            paint.style = Paint.Style.FILL
            canvas?.drawPoints(points, paint)
            imageView.setImageBitmap(bitmap)
        }
    }

    private var count = 0
    private fun shoot() {
        ++count
        if (count != 10) return
        val buffer = ByteArray(width * height * 4)
        dlib.samplingTexture(textureId, width, height, buffer, IntArray(1), IntArray(1))

        val pixels = IntArray(width * height)
        for (i in 0 until height) {
            for (j in 0 until width) {
                val index = i * width * 4 + j * 4
                pixels[i * width + j] = Color.argb(buffer[index + 3].toInt(), buffer[index].toInt(),
                        buffer[index + 1].toInt(), buffer[index + 2].toInt())
            }
        }

        val bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
        bitmap.setPixels(pixels, 0, width, 0, 0, width, height)
        val fos = FileOutputStream(File(Environment.getExternalStorageDirectory(), "gray.jpg"))
        bitmap.compress(Bitmap.CompressFormat.JPEG, 80, fos)
        fos.close()
        bitmap.recycle()
    }

    override fun getRect(): RectF = rect
    override fun release() {
        super.release()
        if (null != bitmap && !bitmap!!.isRecycled) {
            bitmap?.recycle()
        }
        dlib.release()
        initTask?.cancel(true)
    }

    private class InitTask(private val filter: FaceFilter) : AsyncTask<Void, Void, Void?>() {
        override fun doInBackground(vararg params: Void?): Void? {
            filter.initDlib()
            return null
        }
    }
}