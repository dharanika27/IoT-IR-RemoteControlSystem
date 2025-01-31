// Generated by view binder compiler. Do not edit!
package com.ir.example.databinding;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.widget.AppCompatTextView;
import androidx.constraintlayout.widget.ConstraintLayout;
import androidx.viewbinding.ViewBinding;
import androidx.viewbinding.ViewBindings;
import com.ir.example.R;
import java.lang.NullPointerException;
import java.lang.Override;
import java.lang.String;

public final class ActivityPahoExampleBinding implements ViewBinding {
  @NonNull
  private final ConstraintLayout rootView;

  @NonNull
  public final AppCompatTextView connectionStatus;

  @NonNull
  public final AppCompatTextView irDetailsTv;

  @NonNull
  public final AppCompatTextView learnRepeatTv;

  @NonNull
  public final AppCompatTextView tvConnectionStatus;

  private ActivityPahoExampleBinding(@NonNull ConstraintLayout rootView,
      @NonNull AppCompatTextView connectionStatus, @NonNull AppCompatTextView irDetailsTv,
      @NonNull AppCompatTextView learnRepeatTv, @NonNull AppCompatTextView tvConnectionStatus) {
    this.rootView = rootView;
    this.connectionStatus = connectionStatus;
    this.irDetailsTv = irDetailsTv;
    this.learnRepeatTv = learnRepeatTv;
    this.tvConnectionStatus = tvConnectionStatus;
  }

  @Override
  @NonNull
  public ConstraintLayout getRoot() {
    return rootView;
  }

  @NonNull
  public static ActivityPahoExampleBinding inflate(@NonNull LayoutInflater inflater) {
    return inflate(inflater, null, false);
  }

  @NonNull
  public static ActivityPahoExampleBinding inflate(@NonNull LayoutInflater inflater,
      @Nullable ViewGroup parent, boolean attachToParent) {
    View root = inflater.inflate(R.layout.activity_paho_example, parent, false);
    if (attachToParent) {
      parent.addView(root);
    }
    return bind(root);
  }

  @NonNull
  public static ActivityPahoExampleBinding bind(@NonNull View rootView) {
    // The body of this method is generated in a way you would not otherwise write.
    // This is done to optimize the compiled bytecode for size and performance.
    int id;
    missingId: {
      id = R.id.connectionStatus;
      AppCompatTextView connectionStatus = ViewBindings.findChildViewById(rootView, id);
      if (connectionStatus == null) {
        break missingId;
      }

      id = R.id.ir_details_tv;
      AppCompatTextView irDetailsTv = ViewBindings.findChildViewById(rootView, id);
      if (irDetailsTv == null) {
        break missingId;
      }

      id = R.id.learn_repeat_tv;
      AppCompatTextView learnRepeatTv = ViewBindings.findChildViewById(rootView, id);
      if (learnRepeatTv == null) {
        break missingId;
      }

      id = R.id.tvConnectionStatus;
      AppCompatTextView tvConnectionStatus = ViewBindings.findChildViewById(rootView, id);
      if (tvConnectionStatus == null) {
        break missingId;
      }

      return new ActivityPahoExampleBinding((ConstraintLayout) rootView, connectionStatus,
          irDetailsTv, learnRepeatTv, tvConnectionStatus);
    }
    String missingId = rootView.getResources().getResourceName(id);
    throw new NullPointerException("Missing required view with ID: ".concat(missingId));
  }
}
