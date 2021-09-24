package com.cfox.ffplayvideo

import android.Manifest
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.view.Surface
import android.view.SurfaceHolder
import android.view.View
import com.cfox.espermission.EsPermissions
import com.cfox.ffplayvideo.databinding.ActivityMainBinding
import java.io.File
import java.util.ArrayList

class MainActivity : AppCompatActivity() , ConfigListener {
    private lateinit var binding: ActivityMainBinding

    private val playManager = PlayManager()

    private var surface : Surface ? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (!checkPermissions()) {
            finish()
        }

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        val filePath = File(Environment.getExternalStorageDirectory(), "input.mp4").absolutePath
        playManager.setData(filePath)
        playManager.setConfigListener(this)
        playManager.prepare()

//        binding.encoderInfo.text = playManager.getEncoderInfo()
        binding.surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceCreated(holder: SurfaceHolder?) {
                surface = holder?.surface
            }

            override fun surfaceChanged(
                holder: SurfaceHolder?,
                format: Int,
                width: Int,
                height: Int
            ) {

            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {

            }

        })


    }

    private fun checkPermissions() : Boolean {
        val permissions: MutableList<String> = ArrayList()
        permissions.add(Manifest.permission.WRITE_EXTERNAL_STORAGE)
        var permissionResult = EsPermissions(this).isGranted(permissions)
        if (!permissionResult) {
            EsPermissions(this).request(permissions, {
                permissionResult = true
            }, { _, _, _ ->
                permissionResult = false
            })
        }

        return permissionResult
    }

    fun play(view: View) {
        surface?.let {
            playManager.play(it)

        }
    }

    override fun onWHChange(width: Int, height: Int) {
        val screenWidth = resources.displayMetrics.widthPixels
        val param = binding.surfaceView.layoutParams
        param.width = screenWidth
        param.height = ((height.toFloat() / width.toFloat()) * screenWidth).toInt()
        binding.surfaceView.layoutParams = param

    }
}