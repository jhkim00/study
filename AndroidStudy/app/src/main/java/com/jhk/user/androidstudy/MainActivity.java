package com.jhk.user.androidstudy;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.ViewGroup;
import com.jhk.user.androidstudy.phoneLayout.*;

public class MainActivity extends AppCompatActivity implements PhoneContract.View {

    PhoneContract.Presenter mPresenter = null;
    PhoneDialLayout mPhoneDialView = null;
    PhoneCallLayout mPhoneCallView = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mPresenter = new PhonePresenter(this);

        mPhoneDialView = new PhoneDialLayout(getApplicationContext());
        mPhoneCallView = new PhoneCallLayout(getApplicationContext());

        mPhoneDialView.setPresenter(mPresenter);
        mPhoneCallView.setPresenter(mPresenter);

        ViewGroup contentView = (ViewGroup)this.findViewById(android.R.id.content);
        contentView.addView(mPhoneDialView);
        contentView.addView(mPhoneCallView);

        mPresenter.update();
    }

    @Override
    public void updateCallStatus(int status) {
        if (status == BtCallStatus.BT_CALL_IDLE) {
            mPhoneDialView.setVisibility(View.VISIBLE);
            mPhoneCallView.setVisibility(View.INVISIBLE);
        }
        else {
            mPhoneDialView.setVisibility(View.INVISIBLE);
            mPhoneCallView.setVisibility(View.VISIBLE);
        }

        mPhoneCallView.updateCallStatus(status);
    }

    @Override
    public void updateCallNumber(String number) {
        mPhoneCallView.updateCallNumber(number);
    }

    @Override
    public void updateCallTime(int time) {
        mPhoneCallView.updateCallTime(time);
    }
}
