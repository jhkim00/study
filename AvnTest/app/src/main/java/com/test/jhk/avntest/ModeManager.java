package com.test.jhk.avntest;

import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.widget.Toast;

public class ModeManager extends Service {
    private Handler mHandler = new Handler();
    private IKeyService mKeyService;
    private IKeyServiceCallback mKeyServiceCallback = new IKeyServiceCallback.Stub() {
        @Override
        public boolean onKey(String key, String type) throws RemoteException {
            Intent intent = null;
            switch (key) {
                case "RADIO":
                    intent = getPackageManager().getLaunchIntentForPackage("com.test.jhk.radioapp");
                    startActivity(intent);
                    break;

                case "MEDIA":
                    intent = getPackageManager().getLaunchIntentForPackage("com.test.jhk.mediaapp");
                    startActivity(intent);
                    break;
            }
            return false;
        }
    };
    private ServiceConnection mKeySvcConnection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mKeyService = IKeyService.Stub.asInterface(service);
            try {
                mKeyService.registerCallback(mKeyServiceCallback);
            }
            catch(RemoteException e) {

            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mKeyService = null;
        }
    };

    public ModeManager() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        Intent i = new Intent(getApplicationContext(), KeyService.class);
        getApplicationContext().bindService(i, mKeySvcConnection, Context.BIND_AUTO_CREATE);

        return super.onStartCommand(intent, flags, startId);
    }
}
