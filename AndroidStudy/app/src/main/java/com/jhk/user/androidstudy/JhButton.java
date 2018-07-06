package com.jhk.user.androidstudy;

import android.content.Context;
import android.content.res.TypedArray;
import android.support.constraint.ConstraintLayout;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class JhButton extends ConstraintLayout {
    private ImageView mBg = null;
    private ImageView mIcon = null;
    private TextView mText = null;

    public JhButton(Context context) {
        super(context);
        init(null, 0);
    }

    public JhButton(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(attrs, 0);
    }

    public JhButton(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(attrs, defStyle);
    }

    private void init(AttributeSet attrs, int defStyle) {
        String infService = Context.LAYOUT_INFLATER_SERVICE;
        LayoutInflater li = (LayoutInflater) getContext().getSystemService(infService);
        View v = li.inflate(R.layout.sample_jh_button, this, false);
        addView(v);

        mBg = (ImageView)findViewById(R.id.jh_button_bg);
        mIcon = (ImageView)findViewById(R.id.jh_button_icon);
        mText = (TextView)findViewById(R.id.jh_button_text);

        final TypedArray a = getContext().obtainStyledAttributes(
                attrs, R.styleable.JhButton, defStyle, 0);

        mBg.setBackgroundResource(a.getResourceId(R.styleable.JhButton_bg, -1));
        mIcon.setBackgroundResource(a.getResourceId(R.styleable.JhButton_icon, -1));
        mText.setText(a.getString(R.styleable.JhButton_text));
        mText.setTextColor(a.getColor(R.styleable.JhButton_textColor, 0));

        ConstraintLayout.LayoutParams lp = (ConstraintLayout.LayoutParams)mText.getLayoutParams();
        lp.leftMargin = a.getDimensionPixelSize(R.styleable.JhButton_distanceOfIconText, 0);
        mText.setLayoutParams(lp);
    }


}
