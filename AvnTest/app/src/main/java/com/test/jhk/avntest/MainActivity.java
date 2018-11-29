package com.test.jhk.avntest;

import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {
    private static Context mContext;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mContext = MainActivity.this;
    }

    @Override
    protected void onStart() {
        super.onStart();

        //Intent intent = new Intent(getApplicationContext(), ControlPanelService.class);
        //startService(intent);
    }

    public static Context getContext() {
        return mContext;
    }
}
