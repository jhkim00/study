// IBluetoothService.aidl
package com.jhk.user.androidstudy;

// Declare any non-default types here with import statements
import com.jhk.user.androidstudy.IBluetoothServiceCallback;

interface IBluetoothService {
    void makeCall(String number);
    void endCall();
    void registerCallback(IBluetoothServiceCallback cb);
}
