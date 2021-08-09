package com.cfox.ffplayvideo

import android.Manifest
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import com.cfox.espermission.EsPermissions
import com.cfox.ffplayvideo.databinding.ActivityMainBinding
import java.io.File
import java.util.ArrayList

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    private val playManager = PlayManager()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        if (!checkPermissions()) {
            finish()
        }

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        val filePath = File(Environment.getExternalStorageDirectory(), "input.mp4").absolutePath
        playManager.setData(filePath)
        playManager.prepare()

        binding.encoderInfo.text = playManager.getEncoderInfo();
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

}