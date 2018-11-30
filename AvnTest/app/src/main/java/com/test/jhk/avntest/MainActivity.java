package com.test.jhk.avntest;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onStart() {
        super.onStart();

        //Intent intent = new Intent(getApplicationContext(), ControlPanelService.class);
        //startService(intent);

        startService(new Intent(getApplicationContext(), ModeManager.class));
        startService(new Intent(getApplicationContext(), OsdManager.class));
    }
}
