package com.jhk.user.androidstudy;

public interface PhoneContract {
    interface View {
        void updateCallStatus(int status);
        void updateCallNumber(String number);
        void updateCallTime(int time);
    }

    interface Presenter {
        void makeCallByNumber(String number);
        void endCall();

        void update();
    }
}
