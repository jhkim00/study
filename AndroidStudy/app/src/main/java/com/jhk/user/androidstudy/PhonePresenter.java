package com.jhk.user.androidstudy;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import com.jhk.user.androidstudy.service.BluetoothService;
import com.jhk.user.androidstudy.IBluetoothServiceCallback;

public class PhonePresenter implements PhoneContract.Presenter  {

    private static final String TAG = "PhonePresenter";
    private PhoneContract.View mView;
    private int mCallStatus = BtCallStatus.BT_CALL_IDLE;
    private String mCallNumber = new String();
    private ServiceConnection mBtSvcConnection;
    private BroadcastReceiver mReceiver;
    private IBluetoothService mIBluetoothService;

    private IBluetoothServiceCallback mIBluetoothServiceCallback
            = new IBluetoothServiceCallback.Stub() {
        @Override
        public void callStatusChanged(int status) throws RemoteException {
            Log.d(TAG, "callStatusChanged(" + status + ")");
        }

        @Override
        public void callNumberChanged(String number) throws RemoteException {
            Log.d(TAG, "callNumberChanged(" + number + ")");
        }
    };

    public PhonePresenter(PhoneContract.View v) {
        mView = v;

        __init();
    }

    @Override
    public void makeCallByNumber(String number) {
        try {
            mIBluetoothService.makeCall(number);
        }
        catch (RemoteException e) {

        }
    }

    @Override
    public void endCall() {
        try {
            mIBluetoothService.endCall();
        }
        catch (RemoteException e) {

        }
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
                mIBluetoothService = IBluetoothService.Stub.asInterface(service);
                try {
                    mIBluetoothService.registerCallback(mIBluetoothServiceCallback);
                }
                catch(RemoteException e) {

                }
            }

            @Override
            public void onServiceDisconnected(ComponentName name) {
                mIBluetoothService = null;
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
                                mCallStatus =  b.getInt("arg");
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
        PhoneApp.getAppContext().bindService(i, mBtSvcConnection, Context.BIND_AUTO_CREATE);
    }
}
