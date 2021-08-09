package com.cfox.espermission

import android.os.Build
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentActivity
import androidx.fragment.app.FragmentManager
import java.lang.RuntimeException

class EsPermissions {

    companion object {
        private const val FRAGMENT_NAME = "EsPermissionFragment"
    }

    private lateinit var fragmentManager: FragmentManager

    constructor(activity: FragmentActivity) {
        fragmentManager = activity.supportFragmentManager
    }

    constructor(fragment: Fragment) {
        fragmentManager = fragment.childFragmentManager
    }

    private val isMarshmallow: Boolean by lazy {
        Build.VERSION.SDK_INT >= Build.VERSION_CODES.M
    }

    private val permissionsFragment by lazy {
        fragmentManager.findFragmentByTag(FRAGMENT_NAME) as EsPermissionFragment?
            ?: EsPermissionFragment().also {
                fragmentManager
                    .beginTransaction()
                    .add(it, FRAGMENT_NAME)
                    .commitNowAllowingStateLoss()
            }
    }

    /**
     * Returns true if the permission is already granted.
     *
     * Always true if SDK &lt; 23.
     */
    fun isGranted(permissions: List<String>): Boolean {

        if (permissions.isEmpty()) {
            throw RuntimeException("request permissions is empty")
        }

        if (!isMarshmallow) {
            return true
        }

        return permissionsFragment.isGranted(permissions)
    }

    /**
     * permissions: 返回申请的所有权限
     * deniedPermissions： 返回被拒绝的权限
     * permanentlyDeniedPermissions ： 返回被拒绝并设置了不在询问的权限
     *
     * Always true if SDK &lt; 23.
     */
    fun request(permissions: List<String>, onSuccess: (List<String>) -> Unit,
                onFail: (permissions: List<String>, deniedPermissions: List<String>, permanentlyDeniedPermissions : List<String>) -> Unit) {

        if (permissions.isEmpty()) {
            throw RuntimeException("request permissions is empty")
        }

        if (!isMarshmallow) {
            onSuccess(permissions)
            return
        }

        permissionsFragment.request(permissions.toList() , object : PermissionCallbackListener {
            override fun onSuccess(permissions: List<String>) {
                onSuccess(permissions)
            }

            override fun onFail(
                permissions: List<String>,
                deniedPermissions: List<String>,
                permanentlyDeniedPermissions: List<String>
            ) {
                onFail(permissions, deniedPermissions, permanentlyDeniedPermissions)
            }
        })
    }


}