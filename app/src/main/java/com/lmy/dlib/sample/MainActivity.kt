package com.lmy.dlib.sample

import android.graphics.*
import android.os.Bundle
import android.os.Environment
import android.support.v7.app.AppCompatActivity
import com.lmy.dlib.Dlib
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File

class MainActivity : AppCompatActivity() {
    private val dlib = Dlib()
    private val rect = IntArray(4)
    private val points = IntArray(68 * 2)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val file = File(Environment.getExternalStorageDirectory(), "test.jpg").absolutePath
        val bitmap = BitmapFactory.decodeFile(file)
        if (null == bitmap || bitmap.isRecycled) {
            sample_text.text = "$file Image is not found"
            return
        }
        val width = bitmap.width
        val height = bitmap.height
        val pixels = IntArray(width * height)
        bitmap.getPixels(pixels, 0, width, 0, 0, width, height)
        dlib.detect(pixels, width, height, rect, points)
        showResult(bitmap)
        sample_text.text = "(${rect[0]},${rect[1]})-(${rect[2]},${rect[3]})"
        bitmap.recycle()
    }

    private fun showResult(src: Bitmap) {
        val rect = Rect(0, 0, src.width, src.height)
        val dest = Bitmap.createBitmap(rect.width(), rect.height(), Bitmap.Config.ARGB_8888)
        val canvas = Canvas(dest)
        val paint = Paint(Paint.ANTI_ALIAS_FLAG)
        paint.style = Paint.Style.STROKE
        paint.strokeWidth = 3f
        paint.color = Color.RED
        canvas.drawBitmap(src, rect, rect, null)
        canvas.drawRect(Rect(this.rect[0], this.rect[1], this.rect[2], this.rect[3]), paint)
        val points = FloatArray(this.points.size)
        for (i in 4 until this.points.size) {
            points[i - 4] = this.points[i].toFloat()
        }
        paint.color = Color.GREEN
        paint.style = Paint.Style.FILL
        canvas.drawPoints(points, paint)
        imageView.setImageBitmap(dest)
    }

    override fun onDestroy() {
        super.onDestroy()
        dlib.release()
    }

    companion object {
        const val TAG = "MainActivity"
    }
}
