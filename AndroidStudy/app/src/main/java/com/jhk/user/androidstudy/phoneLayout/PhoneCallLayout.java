package com.jhk.user.androidstudy.phoneLayout;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.jhk.user.androidstudy.BtCallStatus;
import com.jhk.user.androidstudy.R;

public class PhoneCallLayout extends PhoneLayout {

    private String mCallNumber = new String();

    public PhoneCallLayout(Context context) {
        super(context);

        __init(context);
    }

    public PhoneCallLayout(Context context, AttributeSet attrs) {
        super(context);

        __init(context);
    }

    public void updateCallNumber(String number) {
        TextView v = (TextView)findViewById(R.id.text_call_number);
        v.setText(number);
    }

    public void updateCallStatus(BtCallStatus status) {

    }

    public void updateCallTime(int time) {

    }

    private void __init(Context context) {
        LayoutInflater inflater =(LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View v = inflater.inflate(R.layout.layout_phone_call,this,false);

        addView(v);

        findViewById(R.id.btn_mic).setOnClickListener(mBtnMicClickListener);
        findViewById(R.id.btn_keypad).setOnClickListener(mBtnKeypadClickListener);
        findViewById(R.id.btn_private).setOnClickListener(mBtnPrivateClickListener);
        findViewById(R.id.btn_call_end).setOnClickListener(mBtnCallEndClickListener);
    }

    Button.OnClickListener mBtnMicClickListener = new View.OnClickListener() {
        public void onClick(View v) {

        }
    };

    Button.OnClickListener mBtnKeypadClickListener = new View.OnClickListener() {
        public void onClick(View v) {

        }
    };

    Button.OnClickListener mBtnPrivateClickListener = new View.OnClickListener() {
        public void onClick(View v) {

        }
    };

    Button.OnClickListener mBtnCallEndClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            mPresenter.endCall();
        }
    };
}
