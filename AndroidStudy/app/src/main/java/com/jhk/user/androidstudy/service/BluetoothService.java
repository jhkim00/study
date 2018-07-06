package com.jhk.user.androidstudy.service;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

import com.jhk.user.androidstudy.BtCallStatus;

public class BluetoothService extends Service {

    private static final String TAG = "BluetoothService";
    private IBinder mBinder = null;

    class BluetoothBinder extends Binder {
    }

    public BluetoothService() {
        mBinder = new BluetoothBinder();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        switch (intent.getAction()) {
            case "com.jhk.user.androidstudy.action.BTREQ":
                Bundle b = intent.getExtras();
                String cmd = b.getString("cmd");
                String arg = b.getString("arg");

                Log.d(TAG, "cmd[" + cmd + "], arg[" + arg + "]");

                Intent i = new Intent("com.jhk.user.androidstudy.action.BTEVT");

                switch (cmd) {
                    case "makeCall":
                        i.putExtra("type", "call status");
                        i.putExtra("arg", BtCallStatus.BT_CALL_OUTGOING);
                        sendBroadcast(i);

                        i.putExtra("type", "call number");
                        i.putExtra("arg", arg);
                        sendBroadcast(i);
                        break;

                    case "endCall":
                        i.putExtra("type", "call status");
                        i.putExtra("arg", BtCallStatus.BT_CALL_IDLE);
                        sendBroadcast(i);
                        break;
                }
                break;
        }
        return super.onStartCommand(intent, flags, startId);
    }
}
