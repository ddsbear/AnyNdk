package com.dds.opengl.utils;

import android.Manifest;
import android.content.Context;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CameraMetadata;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.os.Handler;
import android.support.annotation.RequiresPermission;
import android.text.TextUtils;
import android.util.Size;

/**
 * Created by dds on 2019/6/22.
 * android_shuai@163.com
 */
public class Camera2Helper {
    private CameraManager mManager;
    private String mBackCameraId;

    public Camera2Helper(Context context) {
        mManager = (CameraManager) context.getSystemService(Context.CAMERA_SERVICE);
    }

    public String getBackCameraId() {
        if (!TextUtils.isEmpty(mBackCameraId)) {
            return mBackCameraId;
        }
        try {
            String[] ids = mManager.getCameraIdList();
            for (String cameraId : ids) {
                CameraCharacteristics characteristics = mManager.getCameraCharacteristics(cameraId);
                Integer facing = characteristics.get(CameraCharacteristics.LENS_FACING);
                // 根据摄像头的朝向判断是否是后置摄像头
                if (null != facing && CameraMetadata.LENS_FACING_BACK == facing) {
                    mBackCameraId = cameraId;
                    return cameraId;
                }
            }
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
        return "";
    }

    public int getSensorOrientation(String cameraId) {
        try {
            CameraCharacteristics characteristics = mManager.getCameraCharacteristics(cameraId);
            return characteristics.get(CameraCharacteristics.SENSOR_ORIENTATION);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
        return -1;
    }

    public Size[] getSupportSize(String cameraId, Class klass) {
        try {
            CameraCharacteristics characteristics = mManager.getCameraCharacteristics(cameraId);
            StreamConfigurationMap map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
            if (null != map) {
                return map.getOutputSizes(klass);
//                map.getOutputSizes(MediaRecorder.class);// 支持的录像视频size
//                map.getOutputSizes(MediaRecorder.class);// 支持的录像视频size
//                map.getOutputSizes(ImageReader.class);// 支持的拍照照片size
            }
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
        return null;
    }

    // 注意，camera、recorder权限都是隐私权限，6.0以后需要动态权限配置
    @RequiresPermission(Manifest.permission.CAMERA)
    public void openCamera(String cameraId, CameraDevice.StateCallback callback, Handler handler) {
        try {
            mManager.openCamera(cameraId, callback, handler);
        } catch (CameraAccessException e) {
            e.printStackTrace();
        }
    }
}