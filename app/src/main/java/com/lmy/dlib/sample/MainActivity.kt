package com.lmy.dlib.sample

import android.os.Bundle
import android.os.Environment
import android.support.v7.app.AppCompatActivity
import android.view.TextureView
import android.widget.FrameLayout
import com.lmy.codec.presenter.impl.VideoRecorderImpl
import com.lmy.codec.texture.impl.filter.BaseFilter
import com.lmy.codec.texture.impl.filter.BeautyV4Filter
import com.lmy.codec.texture.impl.filter.GroupFilter
import com.lmy.codec.wrapper.CameraWrapper
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {
    private lateinit var mRecorder: VideoRecorderImpl
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val mTextureView = TextureView(this)
        mTextureView.keepScreenOn = true
        container.addView(mTextureView, FrameLayout.LayoutParams(FrameLayout.LayoutParams.MATCH_PARENT,
                FrameLayout.LayoutParams.MATCH_PARENT))
        mRecorder = VideoRecorderImpl(this).apply {
            reset()
            setCameraIndex(CameraWrapper.CameraIndex.FRONT)
            setOutputUri("${Environment.getExternalStorageDirectory().absolutePath}/test.mp4")
            setOutputSize(720, 1280)//Default 720x1280
            setFps(30)
            setFilter(getDefaultFilter())
            setPreviewDisplay(mTextureView)
        }
        mRecorder.prepare()
    }

    private fun getDefaultFilter(): BaseFilter = GroupFilter.create(BeautyV4Filter())
            .addSticker(FaceFilter(imageView))

    override fun onDestroy() {
        super.onDestroy()
        mRecorder.release()
    }

    companion object {
        const val TAG = "MainActivity"
    }
}
