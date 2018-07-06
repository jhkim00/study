package com.jhk.user.androidstudy.phoneLayout;

import android.content.Context;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.jhk.user.androidstudy.R;

public class PhoneDialLayout extends PhoneLayout {

    private static final int DIAL_NUM_LEN_MAX = 16;
    private EditText mDialNumInput = null;
    private String mDialNumBuffer = new String();

    public PhoneDialLayout(Context context) {
        super(context);

        __init(context);
    }
    public PhoneDialLayout(Context context, AttributeSet attrs) {
        super(context, attrs);

        __init(context);
    }

    private void __init(Context context) {
        LayoutInflater inflater =(LayoutInflater)context.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        View v = inflater.inflate(R.layout.layout_phone_dial,this,false);

        addView(v);

        mDialNumInput = (EditText)(findViewById(R.id.dial_num_input));

        for (int i = 0; i <= 9; ++i) {
            int resourceId = getResources().getIdentifier("kp_" + i,
                    "id",
                    "com.jhk.user.androidstudy");
            findViewById(resourceId).setOnClickListener(mKpNumClickListener);
        }

        findViewById(R.id.kp_star).setOnClickListener(mKpStarClickListener);
        findViewById(R.id.kp_0).setOnLongClickListener(mKp0LongClickListener);
        findViewById(R.id.kp_sharp).setOnClickListener(mKpSharpClickListener);
        findViewById(R.id.kp_call).setOnClickListener(mKpCallClickListener);
        //findViewById(R.id.kp_setting).setOnClickListener(mKpClickListener);
        findViewById(R.id.kp_del).setOnClickListener(mKpDelClickListener);
        findViewById(R.id.kp_del).setOnLongClickListener(mKpDelLongClickListener);

        __updateKpState();
    }

    private void __updateKpState() {
        for (int i = 0; i <= 9; ++i) {
            int resourceId = getResources().getIdentifier("kp_" + i,
                    "id",
                    "com.jhk.user.androidstudy");
            findViewById(resourceId).setEnabled(mDialNumBuffer.length() < DIAL_NUM_LEN_MAX);
        }

        findViewById(R.id.kp_star).setEnabled(mDialNumBuffer.length() < DIAL_NUM_LEN_MAX);
        findViewById(R.id.kp_sharp).setEnabled(mDialNumBuffer.length() < DIAL_NUM_LEN_MAX);
        findViewById(R.id.kp_del).setEnabled(mDialNumBuffer.length() != 0);
    }

    private void __updateDialNumInput() {
        if (mDialNumBuffer.isEmpty()) {
            mDialNumInput.setText(R.string.str_phone_enter_number);
        } else {
            mDialNumInput.setText(mDialNumBuffer);
            mDialNumInput.setSelection(mDialNumInput.length());
        }
    }

    Button.OnClickListener mKpNumClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            String[] inputVal = {
                    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"
            };
            String strId = getResources().getResourceEntryName(v.getId());
            String strIdx = strId.substring(strId.length() - 1);
            int idx = Integer.parseInt(strIdx);

            mDialNumBuffer = mDialNumBuffer + inputVal[idx];

            __updateDialNumInput();
            __updateKpState();
        }
    };

    Button.OnClickListener mKpStarClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            mDialNumBuffer = mDialNumBuffer + "*";
            __updateDialNumInput();
            __updateKpState();
        }
    };

    Button.OnClickListener mKpSharpClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            mDialNumBuffer = mDialNumBuffer + "#";
            __updateDialNumInput();
            __updateKpState();
        }
    };

    Button.OnClickListener mKpCallClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            mPresenter.makeCallByNumber(mDialNumBuffer);
            mDialNumBuffer = "";
            __updateDialNumInput();
            __updateKpState();
        }
    };

    Button.OnClickListener mKpDelClickListener = new View.OnClickListener() {
        public void onClick(View v) {
            if (!mDialNumBuffer.isEmpty()) {
                mDialNumBuffer = mDialNumBuffer.substring(0, mDialNumBuffer.length() - 1);
                __updateDialNumInput();
                __updateKpState();
            }
        }
    };

    Button.OnLongClickListener mKp0LongClickListener = new View.OnLongClickListener() {
        public boolean onLongClick(View v) {
            mDialNumBuffer = mDialNumBuffer + "+";
            __updateDialNumInput();
            __updateKpState();

            return true;
        }
    };

    Button.OnLongClickListener mKpDelLongClickListener = new View.OnLongClickListener() {
        public boolean onLongClick(View v) {
            if (!mDialNumBuffer.isEmpty()) {
                mDialNumBuffer = "";
                __updateDialNumInput();
                __updateKpState();
            }
            return true;
        }
    };
}
