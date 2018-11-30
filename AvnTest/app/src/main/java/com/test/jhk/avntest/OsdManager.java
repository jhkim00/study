package com.test.jhk.avntest;

import android.Manifest;
import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PermissionInfo;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.test.jhk.jcomponent.JButton;

import java.util.ArrayList;
import java.util.List;

public class OsdManager extends Service {
    private static final String                     TAG = "OsdManager";
    private BroadcastReceiver                       mReceiver;
    private Handler                                 mHandler;
    private LinearLayout                            mView;

    {
        mReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                switch (intent.getAction()) {
                    case "com.test.jhkim.avnstudy.action.OSD_EVT":
                        __showOsd();
                        break;
                }
            }
        };
        mHandler = new Handler();
    }

    public OsdManager() {
    }

    private void __showOsd() {
        mHandler.post(new Runnable() {
            @Override
            public void run() {
                int alertPermission = ContextCompat.checkSelfPermission(getApplicationContext(), "android.permission.SYSTEM_ALERT_WINDOW");

                if (alertPermission == PackageManager.PERMISSION_GRANTED) {

                    WindowManager.LayoutParams lp =
                            new WindowManager.LayoutParams(
                                    WindowManager.LayoutParams.WRAP_CONTENT,
                                    WindowManager.LayoutParams.WRAP_CONTENT,
                                    WindowManager.LayoutParams.TYPE_APPLICATION_OVERLAY,
                                    WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE,
                                    PixelFormat.TRANSLUCENT);

                    WindowManager wm = ((WindowManager) getSystemService(WINDOW_SERVICE));
                    wm.addView(mView, lp);
                }
                else {
                }
            }
        });
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public void onCreate() {
        super.onCreate();

        IntentFilter iFilter = new IntentFilter("com.test.jhkim.avnstudy.action.OSD_EVT");
        AvnTestApp.getAppContext().registerReceiver(mReceiver, iFilter);

        mView = (LinearLayout) LayoutInflater.from(getApplicationContext()).inflate(R.layout.control_panel_layout, null);
        if (mView != null) {
            /*ArrayList<JButton> btns = new ArrayList<JButton>(2);
            JButton btnRadio = (JButton)mView.findViewById(R.id.btn_radio);
            JButton btnMedia = (JButton)mView.findViewById(R.id.btn_media);

            btns.add(btnRadio);
            btns.add(btnMedia);

            for (JButton btn : btns) {
                btn.setBackgroundColor(Color.parseColor("#000000"));
                btn.setTextColor(Color.parseColor("#ffffff"));
                btn.setTextSize(24);
            }*/
        }
    }
}
