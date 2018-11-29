package com.test.jhk.avntest;

import android.app.Service;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.PixelFormat;
import android.os.IBinder;
import android.util.Log;
import android.util.TypedValue;
import android.view.LayoutInflater;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.test.jhk.jcomponent.JButton;

import java.util.ArrayList;

public class ControlPanelService extends Service {
    private LinearLayout mView;

    public ControlPanelService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");


    }

    @Override
    public void onCreate() {
        super.onCreate();

        /*mView = (LinearLayout) LayoutInflater.from(getApplicationContext()).inflate(R.layout.control_panel_layout, null);
        if (mView != null) {
            ArrayList<JButton> btns = new ArrayList<JButton>(2);
            JButton btnRadio = (JButton)mView.findViewById(R.id.btn_radio);
            JButton btnMedia = (JButton)mView.findViewById(R.id.btn_media);

            btns.add(btnRadio);
            btns.add(btnMedia);

            for (JButton btn : btns) {
                btn.setBackgroundColor(Color.parseColor("#000000"));
                btn.setTextColor(Color.parseColor("#ffffff"));
                btn.setTextSize(24);
            }

            WindowManager.LayoutParams lp =
                new WindowManager.LayoutParams(400, 1000,
                                               WindowManager.LayoutParams.TYPE_TOAST,
                                               WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL,
                                               PixelFormat.TRANSLUCENT);

            WindowManager wm = ((WindowManager) getSystemService(WINDOW_SERVICE));
            wm.addView(mView, lp);
        }*/
    }
}
