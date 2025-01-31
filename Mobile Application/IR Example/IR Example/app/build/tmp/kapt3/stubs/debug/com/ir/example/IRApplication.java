package com.ir.example;

import java.lang.System;

@kotlin.Metadata(mv = {1, 6, 0}, k = 1, d1 = {"\u0000(\n\u0002\u0018\u0002\n\u0002\u0018\u0002\n\u0002\b\u0002\n\u0002\u0018\u0002\n\u0002\u0010\u000e\n\u0002\b\u0003\n\u0002\u0018\u0002\n\u0002\b\u001a\n\u0002\u0010\u0002\n\u0002\b\u0004\b\u0007\u0018\u00002\u00020\u0001B\u0005\u00a2\u0006\u0002\u0010\u0002J\b\u0010#\u001a\u00020$H\u0002J\b\u0010%\u001a\u00020$H\u0016J\u0006\u0010&\u001a\u00020$J\u0006\u0010\'\u001a\u00020$R\u0017\u0010\u0003\u001a\b\u0012\u0004\u0012\u00020\u00050\u0004\u00a2\u0006\b\n\u0000\u001a\u0004\b\u0006\u0010\u0007R\u001c\u0010\b\u001a\u0004\u0018\u00010\tX\u0086\u000e\u00a2\u0006\u000e\n\u0000\u001a\u0004\b\n\u0010\u000b\"\u0004\b\f\u0010\rR\u0017\u0010\u000e\u001a\b\u0012\u0004\u0012\u00020\u00050\u0004\u00a2\u0006\b\n\u0000\u001a\u0004\b\u000f\u0010\u0007R\u0017\u0010\u0010\u001a\b\u0012\u0004\u0012\u00020\u00050\u0004\u00a2\u0006\b\n\u0000\u001a\u0004\b\u0011\u0010\u0007R\u0017\u0010\u0012\u001a\b\u0012\u0004\u0012\u00020\u00050\u0004\u00a2\u0006\b\n\u0000\u001a\u0004\b\u0013\u0010\u0007R\u001a\u0010\u0014\u001a\u00020\u0005X\u0086\u000e\u00a2\u0006\u000e\n\u0000\u001a\u0004\b\u0015\u0010\u0016\"\u0004\b\u0017\u0010\u0018R\u0014\u0010\u0019\u001a\u00020\u0005X\u0086D\u00a2\u0006\b\n\u0000\u001a\u0004\b\u001a\u0010\u0016R\u0014\u0010\u001b\u001a\u00020\u0005X\u0086D\u00a2\u0006\b\n\u0000\u001a\u0004\b\u001c\u0010\u0016R\u0014\u0010\u001d\u001a\u00020\u0005X\u0086D\u00a2\u0006\b\n\u0000\u001a\u0004\b\u001e\u0010\u0016R\u0014\u0010\u001f\u001a\u00020\u0005X\u0086D\u00a2\u0006\b\n\u0000\u001a\u0004\b \u0010\u0016R\u0017\u0010!\u001a\b\u0012\u0004\u0012\u00020\u00050\u0004\u00a2\u0006\b\n\u0000\u001a\u0004\b\"\u0010\u0007\u00a8\u0006("}, d2 = {"Lcom/ir/example/IRApplication;", "Landroid/app/Application;", "()V", "lux", "Landroidx/lifecycle/MutableLiveData;", "", "getLux", "()Landroidx/lifecycle/MutableLiveData;", "mqttAndroidClient", "Lorg/eclipse/paho/android/service/MqttAndroidClient;", "getMqttAndroidClient", "()Lorg/eclipse/paho/android/service/MqttAndroidClient;", "setMqttAndroidClient", "(Lorg/eclipse/paho/android/service/MqttAndroidClient;)V", "onConnectionData", "getOnConnectionData", "onMsgData", "getOnMsgData", "onPublishData", "getOnPublishData", "publishMessage", "getPublishMessage", "()Ljava/lang/String;", "setPublishMessage", "(Ljava/lang/String;)V", "publishTopic", "getPublishTopic", "serverUri", "getServerUri", "subscriptionTopic", "getSubscriptionTopic", "subscriptionlearnTopic", "getSubscriptionlearnTopic", "temp", "getTemp", "connecttoMqtt", "", "onCreate", "publishMessage1", "subscribeToTopic", "app_debug"})
@dagger.hilt.android.HiltAndroidApp
public final class IRApplication extends android.app.Application {
    @org.jetbrains.annotations.NotNull
    private final androidx.lifecycle.MutableLiveData<java.lang.String> temp = null;
    @org.jetbrains.annotations.NotNull
    private final androidx.lifecycle.MutableLiveData<java.lang.String> lux = null;
    @org.jetbrains.annotations.NotNull
    private final androidx.lifecycle.MutableLiveData<java.lang.String> onPublishData = null;
    @org.jetbrains.annotations.NotNull
    private final androidx.lifecycle.MutableLiveData<java.lang.String> onMsgData = null;
    @org.jetbrains.annotations.NotNull
    private final androidx.lifecycle.MutableLiveData<java.lang.String> onConnectionData = null;
    @org.jetbrains.annotations.Nullable
    private org.eclipse.paho.android.service.MqttAndroidClient mqttAndroidClient;
    @org.jetbrains.annotations.NotNull
    private final java.lang.String serverUri = "tcp://mqtt.eclipseprojects.io";
    @org.jetbrains.annotations.NotNull
    private final java.lang.String subscriptionTopic = "mqtt/temp_data";
    @org.jetbrains.annotations.NotNull
    private final java.lang.String subscriptionlearnTopic = "mqtt/learn";
    @org.jetbrains.annotations.NotNull
    private final java.lang.String publishTopic = "/mqtt/receive";
    @org.jetbrains.annotations.NotNull
    private java.lang.String publishMessage = "Test";
    
    public IRApplication() {
        super();
    }
    
    @org.jetbrains.annotations.NotNull
    public final androidx.lifecycle.MutableLiveData<java.lang.String> getTemp() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final androidx.lifecycle.MutableLiveData<java.lang.String> getLux() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final androidx.lifecycle.MutableLiveData<java.lang.String> getOnPublishData() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final androidx.lifecycle.MutableLiveData<java.lang.String> getOnMsgData() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final androidx.lifecycle.MutableLiveData<java.lang.String> getOnConnectionData() {
        return null;
    }
    
    @org.jetbrains.annotations.Nullable
    public final org.eclipse.paho.android.service.MqttAndroidClient getMqttAndroidClient() {
        return null;
    }
    
    public final void setMqttAndroidClient(@org.jetbrains.annotations.Nullable
    org.eclipse.paho.android.service.MqttAndroidClient p0) {
    }
    
    @org.jetbrains.annotations.NotNull
    public final java.lang.String getServerUri() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final java.lang.String getSubscriptionTopic() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final java.lang.String getSubscriptionlearnTopic() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final java.lang.String getPublishTopic() {
        return null;
    }
    
    @org.jetbrains.annotations.NotNull
    public final java.lang.String getPublishMessage() {
        return null;
    }
    
    public final void setPublishMessage(@org.jetbrains.annotations.NotNull
    java.lang.String p0) {
    }
    
    @java.lang.Override
    public void onCreate() {
    }
    
    private final void connecttoMqtt() {
    }
    
    public final void subscribeToTopic() {
    }
    
    public final void publishMessage1() {
    }
}