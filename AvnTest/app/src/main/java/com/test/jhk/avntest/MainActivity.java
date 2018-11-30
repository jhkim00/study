package com.test.jhk.avntest;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    private Handler mHandler = new Handler();
    private IKeyService mKeyService;
    private IKeyServiceCallback mKeyServiceCallback = new IKeyServiceCallback.Stub() {
        @Override
        public boolean onKey(String key, String type) throws RemoteException {
            switch (key) {
                case "RADIO":
                    mHandler.post(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(), "RADIO", Toast.LENGTH_SHORT).show();
                        }
                    });
                    break;

                case "MEDIA":
                    break;
            }
            return false;
        }
    };
    private ServiceConnection mKeySvcConnection;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mKeySvcConnection = new ServiceConnection() {
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
    }

    @Override
    protected void onStart() {
        super.onStart();

        //Intent intent = new Intent(getApplicationContext(), ControlPanelService.class);
        //startService(intent);
        //Intent intent = new Intent(MainActivity.this, KeyService.class);
        //startService(intent);

        Intent i = new Intent(MainActivity.this, KeyService.class);
        MainActivity.this.bindService(i, mKeySvcConnection, Context.BIND_AUTO_CREATE);
    }
}
