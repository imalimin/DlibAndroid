package com.lmy.dlib.sample

import android.graphics.*
import android.os.Bundle
import android.os.Environment
import android.support.v7.app.AppCompatActivity
import com.lmy.dlib.Dlib
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File
import java.util.*

class MainActivity : AppCompatActivity() {

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
        val rect = Dlib().detectFaceRect(pixels, width, height)
        showResult(bitmap, rect)
        sample_text.text = Arrays.toString(rect)
        bitmap.recycle()
    }

    private fun showResult(src: Bitmap, faceRect: IntArray) {
        val rect = Rect(0, 0, src.width, src.height)
        val dest = Bitmap.createBitmap(rect.width(), rect.height(), Bitmap.Config.ARGB_8888)
        val canvas = Canvas(dest)
        val paint = Paint(Paint.ANTI_ALIAS_FLAG)
        paint.style = Paint.Style.STROKE
        paint.strokeWidth = 3f
        paint.color = Color.RED
        canvas.drawBitmap(src, rect, rect, null)
        canvas.drawRect(Rect(faceRect[0], faceRect[1], faceRect[2], faceRect[3]), paint)
        imageView.setImageBitmap(dest)
    }
}
