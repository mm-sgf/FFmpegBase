package com.cfox.espermission

interface PermissionCallbackListener {

    fun onSuccess(permissions: List<String>)

    fun onFail(permissions: List<String>, deniedPermissions: List<String>, permanentlyDeniedPermissions : List<String>)
}