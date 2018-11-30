package com.test.jhk.avntest;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.content.Context;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.util.Log;
import android.widget.Toast;
import com.test.jhk.avntest.IKeyService;
import com.test.jhk.avntest.IKeyServiceCallback;

public class KeyService extends Service {
    private static final String TAG = "KeyService";

    private RemoteCallbackList<IKeyServiceCallback> mCallbackList
            = new RemoteCallbackList<IKeyServiceCallback>();

    private final IKeyService.Stub mBinder = new IKeyService.Stub() {
        @Override
        public void registerCallback(IKeyServiceCallback cb) throws RemoteException {
            mCallbackList.register(cb);
        }
    };

    private BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            switch (intent.getAction()) {
                case "com.test.jhkim.avnstudy.action.KEY_EVT":
                    Bundle b = intent.getExtras();
                    String type = b.getString("type");

                    Log.d(TAG, type);

                    switch (type) {
                        case "RADIO":
                            Log.d(TAG, "RADIO");
                            mCallbackList.beginBroadcast();
                            try {
                                mCallbackList.getBroadcastItem(0).onKey("RADIO", "short");
                            } catch (RemoteException e) {
                                e.printStackTrace();
                            }
                            mCallbackList.finishBroadcast();
                            break;

                        case "MEDIA":
                            Log.d(TAG, "MEDIA");
                            mCallbackList.beginBroadcast();
                            try {
                                mCallbackList.getBroadcastItem(0).onKey("MEDIA", "short");
                            } catch (RemoteException e) {
                                e.printStackTrace();
                            }
                            mCallbackList.finishBroadcast();
                            break;
                    }
                    break;
            }
        }
    };

    public KeyService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public void onCreate() {
        super.onCreate();

        IntentFilter iFilter = new IntentFilter("com.test.jhkim.avnstudy.action.KEY_EVT");
        AvnTestApp.getAppContext().registerReceiver(mReceiver, iFilter);
    }
}
