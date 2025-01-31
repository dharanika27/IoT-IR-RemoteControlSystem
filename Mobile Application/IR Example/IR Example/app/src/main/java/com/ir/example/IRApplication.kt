package com.ir.example

import android.app.Application
import android.content.Context
import android.net.ConnectivityManager
import android.net.Network
import android.net.NetworkCapabilities
import android.net.wifi.WifiManager
import androidx.lifecycle.MutableLiveData
import dagger.hilt.android.HiltAndroidApp
import org.eclipse.paho.android.service.MqttAndroidClient
import org.eclipse.paho.client.mqttv3.DisconnectedBufferOptions
import org.eclipse.paho.client.mqttv3.IMqttActionListener
import org.eclipse.paho.client.mqttv3.IMqttToken
import org.eclipse.paho.client.mqttv3.MqttClient
import org.eclipse.paho.client.mqttv3.MqttConnectOptions
import org.eclipse.paho.client.mqttv3.MqttException
import org.eclipse.paho.client.mqttv3.MqttMessage
import org.json.JSONObject

@HiltAndroidApp
class IRApplication : Application(){

    val temp = MutableLiveData<String>()
    val lux = MutableLiveData<String>()
    val onPublishData = MutableLiveData<String>()
    val onMsgData = MutableLiveData<String>()
    val onConnectionData = MutableLiveData<String>()

    var mqttAndroidClient: MqttAndroidClient? = null
    val serverUri = "tcp://mqtt.eclipseprojects.io"
    val subscriptionTopic = "mqtt/temp_data"
    val subscriptionlearnTopic = "mqtt/learn"
    val publishTopic = "/mqtt/receive"
    var publishMessage = "Test"
    override fun onCreate() {
        super.onCreate()
        mqttAndroidClient =
            MqttAndroidClient(applicationContext, serverUri, MqttClient.generateClientId())
        connecttoMqtt()

        val wifiManager = getSystemService(Context.WIFI_SERVICE) as WifiManager
        val connectivityManager =
            getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.N) {
            connectivityManager.registerDefaultNetworkCallback(object :
                ConnectivityManager.NetworkCallback() {
                override fun onAvailable(network: Network) {
                }

                override fun onCapabilitiesChanged(
                    network: Network, networkCapabilities: NetworkCapabilities
                ) {
                    connecttoMqtt()
                }
            })
        }

    }
    private fun connecttoMqtt() {
         val mqttConnectOptions = MqttConnectOptions()
        mqttConnectOptions.isAutomaticReconnect = true
        mqttConnectOptions.isCleanSession = false
        try {
            mqttAndroidClient!!.connect(mqttConnectOptions, null, object : IMqttActionListener {
                override fun onSuccess(asyncActionToken: IMqttToken) {
                    onConnectionData.postValue("Connected")
                    val disconnectedBufferOptions = DisconnectedBufferOptions()
                    disconnectedBufferOptions.isBufferEnabled = true
                    disconnectedBufferOptions.bufferSize = 100
                    disconnectedBufferOptions.isPersistBuffer = false
                    disconnectedBufferOptions.isDeleteOldestMessages = false
                    mqttAndroidClient!!.setBufferOpts(disconnectedBufferOptions)
                    subscribeToTopic()
                }

                override fun onFailure(asyncActionToken: IMqttToken, exception: Throwable) {
                    onConnectionData.postValue("Connection Failed!")
                    connecttoMqtt()
                }
            })
        } catch (ex: MqttException) {
            ex.printStackTrace()
        }
    }
    fun subscribeToTopic() {
        try {
            mqttAndroidClient!!.subscribe(subscriptionTopic, 0, null, object : IMqttActionListener {
                override fun onSuccess(asyncActionToken: IMqttToken) {

                }

                override fun onFailure(asyncActionToken: IMqttToken, exception: Throwable) {
                }
            })
            mqttAndroidClient!!.subscribe(subscriptionlearnTopic, 0, null, object : IMqttActionListener {
                override fun onSuccess(asyncActionToken: IMqttToken) {

                }

                override fun onFailure(asyncActionToken: IMqttToken, exception: Throwable) {
                }
            })

            // THIS DOES NOT WORK!
            mqttAndroidClient!!.subscribe(
                subscriptionTopic,
                0
            ) { topic, message -> // message Arrived!
                try {
                    println("Message: " + topic + " : " + String(message.payload))
                    onMsgData.postValue(String(message.payload))
                    val msg = String(message.payload)
                        if (msg.contains("HubDetailsEvent")) {
                            val jsonObject = JSONObject(msg)
                            if (jsonObject.has("data")) {
                                val data = jsonObject.getJSONObject("data")
                                if (data.has("temp"))
                                    temp.postValue(data.getString("temp"))
                                if (data.has("lux"))
                                    lux.postValue(data.getString("lux"))
                            }
                        }

                } catch (ex: MqttException) {
                    ex.printStackTrace()
                }
            }
            mqttAndroidClient!!.subscribe(
                subscriptionlearnTopic,
                0
            ) { topic, message -> // message Arrived!
                try {
                    println("Message: " + topic + " : " + String(message.payload))
                    onMsgData.postValue(String(message.payload))
                    val msg = String(message.payload)
                    if (msg.contains("HubDetailsEvent")) {
                        val jsonObject = JSONObject(msg)
                        if (jsonObject.has("data")) {
                            val data = jsonObject.getJSONObject("data")
                            if (data.has("temp"))
                                temp.postValue(data.getString("temp"))
                            if (data.has("lux"))
                                lux.postValue(data.getString("lux"))
                        }
                    }

                } catch (ex: MqttException) {
                    ex.printStackTrace()
                }
            }
        } catch (ex: MqttException) {
            System.err.println("Exception whilst subscribing")
            ex.printStackTrace()
        }
    }


    fun publishMessage1() {
        try {
            val message = MqttMessage()
            message.payload = publishMessage.toByteArray()
            mqttAndroidClient!!.publish(publishTopic, message)
            onPublishData.postValue("Message Published")
        } catch (e: MqttException) {
            System.err.println("Error Publishing: " + e.message)
            onPublishData.postValue("Message Not Published")
            e.printStackTrace()
        }
    }
}

