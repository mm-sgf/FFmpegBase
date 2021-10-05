package com.cfox.ffplayvideo

import android.util.Log
import android.view.Surface
import java.lang.RuntimeException

class PlayManager(videoPath : String = "") {


    private var configListener : ConfigListener ? = null
    private var vPath = videoPath
    private var playerHandler = 0

    private var width = 0
    private var height = 0

    companion object {
        private const val TAG = "PlayManager"
        init {
            System.loadLibrary("native-lib")
        }
    }

    private external fun native_prepare(path:String) : Int
    private external fun natvie_get_encoder_info() : String
    private external fun native_play(surface: Surface)
    private external fun native_close()


    private fun videoWH(width : Int, height: Int) {
        this.width = width
        this.height = height

        configListener?.onWHChange(width,height)

        Log.d(TAG, "videoWH: w:$width  h:$height")
    }

    fun getWidth() : Int {
        return width
    }

    fun getHeight() : Int {
        return height
    }

    fun setConfigListener(configListener: ConfigListener) {
        this.configListener = configListener
    }

    fun setData(videoPath : String ) {
        vPath = videoPath
    }

    fun prepare() {
        checkPath()
        playerHandler = native_prepare(vPath)
        Log.d(TAG, "prepare: player handler id : $playerHandler")
    }

    fun getEncoderInfo() : String {
        Log.d(TAG, "getInfo: ${natvie_get_encoder_info()}")
        return natvie_get_encoder_info()
    }

    private fun checkPath() {
        if (vPath.isEmpty()) {
            throw RuntimeException("file path is empty ......")
        }
    }

    private fun checkPrepare()  {
        if (playerHandler == 0) {
            throw RuntimeException("ffmpeg not prepare ......")
        }
    }

    fun play(surface: Surface) {
        checkPrepare()
        native_play(surface)
    }

}