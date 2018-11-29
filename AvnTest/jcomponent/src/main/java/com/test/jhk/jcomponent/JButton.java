package com.test.jhk.jcomponent;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.support.annotation.ColorInt;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;

/**
 * TODO: document your custom view class.
 */
public class JButton extends RelativeLayout {
    private ImageView mBg = null;
    private TextView mText = null;

    public JButton(Context context) {
        super(context);
        __init(null, 0);
    }

    public JButton(Context context, AttributeSet attrs) {
        super(context, attrs);
        __init(attrs, 0);
    }

    public JButton(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        __init(attrs, defStyle);
    }

    private void __init(AttributeSet attrs, int defStyle) {
        String infService = Context.LAYOUT_INFLATER_SERVICE;
        LayoutInflater li = (LayoutInflater) getContext().getSystemService(infService);
        View v = li.inflate(R.layout.jbutton, this, false);
        addView(v);

        mBg = (ImageView)findViewById(R.id.bg);
        mText = (TextView)findViewById(R.id.text);

        final TypedArray a = getContext().obtainStyledAttributes(
                attrs, R.styleable.JButton, defStyle, 0);

        mBg.setBackgroundResource(a.getResourceId(R.styleable.JButton_bg, 0));
        mBg.setBackgroundColor(a.getColor(R.styleable.JButton_bgColor, 0));
        mText.setText(a.getString(R.styleable.JButton_text));
        mText.setTextColor(a.getColor(R.styleable.JButton_textColor, 0));
        mText.setTextSize(a.getDimensionPixelSize(R.styleable.JButton_textSize, 0));
    }

    public void setBackgroundColor(@ColorInt int color) {
        mBg.setBackgroundColor(color);
    }

    public void setTextColor(@ColorInt int color) {
        mText.setTextColor(color);
    }

    public void setTextSize(float size) {
        mText.setTextSize(size);
    }
}
