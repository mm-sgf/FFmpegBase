package com.cfox.espermission

import android.content.pm.PackageManager

class PermissionState (private val permission: String) {
    var permissionState = PackageManager.PERMISSION_DENIED
    var requestShouldShowRequestPermissionRationaleState = false
    var resultShouldShowRequestPermissionRationaleState = false

    override fun equals(other: Any?): Boolean {
        val that = other as? PermissionState ?: return false
        return permission == that.permission
    }

    override fun hashCode(): Int {
        return permission.hashCode()
    }

}