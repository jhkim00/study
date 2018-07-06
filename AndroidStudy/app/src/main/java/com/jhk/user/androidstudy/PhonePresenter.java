package com.jhk.user.androidstudy;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;

import com.jhk.user.androidstudy.service.BluetoothService;

public class PhonePresenter implements PhoneContract.Presenter  {

    private static final String TAG = "PhonePresenter";
    private PhoneContract.View mView = null;
    private BtCallStatus mCallStatus = BtCallStatus.BT_CALL_IDLE;
    private String mCallNumber = new String();
    private ServiceConnection mBtSvcConnection = null;
    private BroadcastReceiver mReceiver = null;

    public PhonePresenter(PhoneContract.View v) {
        mView = v;

        __init();
    }

    @Override
    public void makeCallByNumber(String number) {
        Intent i = new Intent(PhoneApp.getAppContext(), BluetoothService.class);
        i.setAction("com.jhk.user.androidstudy.action.BTREQ");
        i.putExtra("cmd", "makeCall");
        i.putExtra("arg", number);

        PhoneApp.getAppContext().startService(i);
    }

    @Override
    public void endCall() {
        Intent i = new Intent(PhoneApp.getAppContext(), BluetoothService.class);
        i.setAction("com.jhk.user.androidstudy.action.BTREQ");
        i.putExtra("cmd", "endCall");

        PhoneApp.getAppContext().startService(i);
    }

    @Override
    public void update() {
        mView.updateCallStatus(mCallStatus);
        mView.updateCallNumber(mCallNumber);
    }

    private void __init() {
        mBtSvcConnection = new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName name, IBinder service) {

            }

            @Override
            public void onServiceDisconnected(ComponentName name) {

            }
        };

        mReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                switch (intent.getAction()) {
                    case "com.jhk.user.androidstudy.action.BTEVT":
                        Bundle b = intent.getExtras();
                        String type = b.getString("type");

                        switch (type) {
                            case "call status":
                                mCallStatus = (BtCallStatus) b.getSerializable("arg");
                                mView.updateCallStatus(mCallStatus);

                                if (mCallStatus == BtCallStatus.BT_CALL_IDLE) {
                                    mCallNumber = "";
                                    mView.updateCallNumber(mCallNumber);
                                }
                                break;

                            case "call number":
                                mCallNumber = b.getString("arg");
                                mView.updateCallNumber(mCallNumber);
                                break;
                        }

                        break;
                }
            }
        };

        IntentFilter iFilter = new IntentFilter("com.jhk.user.androidstudy.action.BTEVT");
        PhoneApp.getAppContext().registerReceiver(mReceiver, iFilter);

        Intent i = new Intent(PhoneApp.getAppContext(), BluetoothService.class);
        PhoneApp.getAppContext().bindService(i, mBtSvcConnection, 0);
    }
}
