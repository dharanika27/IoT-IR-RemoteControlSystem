/*******************************************************************************
 * Copyright (c) 1999, 2016 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 * http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 */
package com.ir.example

import android.os.Bundle
import android.view.inputmethod.EditorInfo
import androidx.appcompat.app.AppCompatActivity
import com.ir.example.databinding.ActivityLearnRepeatBinding
import dagger.hilt.android.AndroidEntryPoint
import org.json.JSONObject

@AndroidEntryPoint
class IRLearnRepeatActivity : AppCompatActivity() {
    private lateinit var bind: ActivityLearnRepeatBinding
    var appContext:IRApplication? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        bind = ActivityLearnRepeatBinding.inflate(layoutInflater)
        setContentView(bind.root)
        bind.pubTv.text = ""
        bind.tvMsg.text = ""
        appContext = (applicationContext as IRApplication)
        appContext?.onPublishData?.postValue("")
        bind.learnIr.setOnClickListener {
            bind.pubTv.text = ""
            appContext?.publishMessage = "IRL"
            appContext?.publishMessage1()
        }
        bind.repeatIr.setOnClickListener {
            if (bind.etIr.text.toString().isNotEmpty()) {
                bind.pubTv.text = ""
                appContext?.publishMessage = "IRR${bind.tvMsg.text.toString()}"
                appContext?.publishMessage1()
            }
        }
        appContext?.onPublishData?.observe(this) { msg->
        bind.pubTv.text = msg
        }
        appContext?.onMsgData?.observe(this) { msg->
            if (msg.contains("LearnData")) {
                try {
                    val jsonObject = JSONObject(msg)
                    if (jsonObject.has("data"))
                        bind.tvMsg.text = (jsonObject.getString("data"))
                }catch (e:Exception){
                    e.printStackTrace()
                }
            }
        }
    }

}
