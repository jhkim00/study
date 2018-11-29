package com.test.jhk.avntest;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.IBinder;
import android.content.Context;
import android.util.Log;

public class KeyService extends Service {
    private static final String TAG = "KeyService";
    private BroadcastReceiver mReceiver;

    public KeyService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        mReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                switch (intent.getAction()) {
                    // adb shell broadcast -a com.test.jhkim.avnstudy.action.KEY_EVT -e|--es type RADIO
                    case "com.test.jhkim.avnstudy.action.KEY_EVT":
                        Bundle b = intent.getExtras();
                        String type = b.getString("type");

                        switch (type) {
                        case "RADIO":
                            Log.d(TAG, "RADIO");
                            break;

                        case "MEDIA":
                            Log.d(TAG, "MEDIA");
                            break;
                        }
                        break;
                }
            }
        };

        IntentFilter iFilter = new IntentFilter("com.test.jhkim.avnstudy.action.KEY_EVT");
        AvnTestApp.getAppContext().registerReceiver(mReceiver, iFilter);

        return super.onStartCommand(intent, flags, startId);
    }
}
