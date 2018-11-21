// IBluetoothServiceCallback.aidl
package com.jhk.user.androidstudy;

// Declare any non-default types here with import statements

interface IBluetoothServiceCallback {
    void callStatusChanged(int status);
    void callNumberChanged(String number);
}
