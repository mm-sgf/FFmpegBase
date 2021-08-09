package com.cfox.espermission

import android.content.pm.PackageManager
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import androidx.fragment.app.Fragment

class EsPermissionFragment : Fragment() {

    companion object {
        private const val PERMISSION_REQUEST_CODE = 1000
    }

    private val permissionsMap = mutableMapOf<String, PermissionState>()
    private val needRequestPermissionsMap = mutableMapOf<String, PermissionState>()
    private var callbackListener : PermissionCallbackListener ? = null

    fun isGranted(permissions :List<String>) : Boolean {
        var  result = true
        activity?.let { activity->
            run out@ {
                permissions.forEach {
                    if (ContextCompat.checkSelfPermission(activity, it) != PackageManager.PERMISSION_GRANTED) {
                        result = false
                        return@out
                    }
                }
            }
        }
        return result
    }

    fun request(permissions :List<String>, listener : PermissionCallbackListener) {
        callbackListener = listener
        activity?.let { activity->
            permissions.forEach {
                val state = PermissionState(it)
                permissionsMap[it] = state
                state.requestShouldShowRequestPermissionRationaleState = ActivityCompat.shouldShowRequestPermissionRationale(activity, it)
                if (ContextCompat.checkSelfPermission(activity, it) == PackageManager.PERMISSION_GRANTED) {
                    state.permissionState = PackageManager.PERMISSION_GRANTED
                } else {
                    state.permissionState = PackageManager.PERMISSION_DENIED
                    needRequestPermissionsMap[it] = state
                }
            }

            if (needRequestPermissionsMap.isNotEmpty()) {
                requestPermissions(needRequestPermissionsMap.keys.toTypedArray(), PERMISSION_REQUEST_CODE)
            } else {
                callbackListener?.onSuccess(permissionsMap.keys.toList())
            }
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)

        activity?.let {activity ->
            if (requestCode == PERMISSION_REQUEST_CODE) {
                needRequestPermissionsMap.clear()
                permissions.forEach {
                    val state = permissionsMap[it]!!
                    state.resultShouldShowRequestPermissionRationaleState = ActivityCompat.shouldShowRequestPermissionRationale(activity, it)
                    if (ContextCompat.checkSelfPermission(activity, it) == PackageManager.PERMISSION_GRANTED) {
                        state.permissionState = PackageManager.PERMISSION_GRANTED
                    } else {
                        state.permissionState = PackageManager.PERMISSION_DENIED
                        needRequestPermissionsMap[it] = state
                    }
                }

                if (needRequestPermissionsMap.isEmpty()) {
                    callbackListener?.onSuccess(permissionsMap.keys.toList())
                } else {
                    val deniedPermissions = mutableListOf<String>()
                    val permanentlyDeniedPermissions = mutableListOf<String>()
                    needRequestPermissionsMap.forEach {
                        if (!it.value.requestShouldShowRequestPermissionRationaleState && !it.value.resultShouldShowRequestPermissionRationaleState) {
                            permanentlyDeniedPermissions.add(it.key)
                        } else {
                            deniedPermissions.add(it.key)
                        }
                    }

                    callbackListener?.onFail(permissionsMap.keys.toList(), deniedPermissions, permanentlyDeniedPermissions)
                }
            }
        }
    }

}