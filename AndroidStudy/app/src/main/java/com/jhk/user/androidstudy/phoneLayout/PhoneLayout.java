package com.jhk.user.androidstudy.phoneLayout;

import android.content.Context;
import android.support.constraint.ConstraintLayout;
import android.util.AttributeSet;

import com.jhk.user.androidstudy.PhoneContract;

public class PhoneLayout extends ConstraintLayout {

    protected PhoneContract.Presenter mPresenter = null;

    public PhoneLayout(Context context) {
        super(context);
    }

    public PhoneLayout(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public void setPresenter(PhoneContract.Presenter presenter) {
        mPresenter = presenter;
    }
}
