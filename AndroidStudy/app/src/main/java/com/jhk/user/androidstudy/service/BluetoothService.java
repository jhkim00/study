package com.jhk.user.androidstudy.service;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.util.Log;

import com.jhk.user.androidstudy.BtCallStatus;
import com.jhk.user.androidstudy.IBluetoothService;
import com.jhk.user.androidstudy.IBluetoothServiceCallback;

public class BluetoothService extends Service {

    private static final String TAG = "BluetoothService";

    private final IBluetoothService.Stub mBinder = new IBluetoothService.Stub() {
        @Override
        public void makeCall(String number) throws RemoteException {
            Log.d(TAG, "makeCall(" + number + ")");

            Intent i = new Intent("com.jhk.user.androidstudy.action.BTEVT");

            i.putExtra("type", "call status");
            i.putExtra("arg", BtCallStatus.BT_CALL_OUTGOING);
            sendBroadcast(i);

            i.putExtra("type", "call number");
            i.putExtra("arg", number);
            sendBroadcast(i);

            mCallbackList.beginBroadcast();
            mCallbackList.getBroadcastItem(0).callStatusChanged(BtCallStatus.BT_CALL_OUTGOING);
            mCallbackList.getBroadcastItem(0).callNumberChanged(number);
            mCallbackList.finishBroadcast();
        }

        @Override
        public void endCall() throws RemoteException {
            Log.d(TAG, "endCall()");

            Intent i = new Intent("com.jhk.user.androidstudy.action.BTEVT");

            i.putExtra("type", "call status");
            i.putExtra("arg", BtCallStatus.BT_CALL_IDLE);
            sendBroadcast(i);

            mCallbackList.beginBroadcast();
            mCallbackList.getBroadcastItem(0).callStatusChanged(BtCallStatus.BT_CALL_IDLE);
            mCallbackList.finishBroadcast();
        }

        @Override
        public void registerCallback(IBluetoothServiceCallback cb) throws RemoteException {
            mCallbackList.register(cb);
        }
    };

    private RemoteCallbackList<IBluetoothServiceCallback> mCallbackList
            = new RemoteCallbackList<IBluetoothServiceCallback>();

    public BluetoothService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }
}
