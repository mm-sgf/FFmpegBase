package com.cfox.ffplayvideo

import android.util.Log
import java.lang.RuntimeException

class PlayManager(videoPath : String = "") {

    private var vPath = videoPath

    private var playerHandler = 0

    companion object {
        private const val TAG = "PlayManager"
        init {
            System.loadLibrary("native-lib")
        }
    }

    private external fun native_prepare(path:String) : Int
    private external fun natvie_get_encoder_info() : String
    private external fun native_close()


    private fun videoWH(width : Int, height: Int) {
        Log.d(TAG, "videoWH: w:$width  h:$height")
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

}