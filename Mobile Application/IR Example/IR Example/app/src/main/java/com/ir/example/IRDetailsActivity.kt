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
import androidx.appcompat.app.AppCompatActivity
import com.ir.example.databinding.ActivityIrDetailsBinding
import dagger.hilt.android.AndroidEntryPoint
import org.json.JSONObject

@AndroidEntryPoint
class IRDetailsActivity : AppCompatActivity() {
    private lateinit var bind: ActivityIrDetailsBinding
    var appContext: IRApplication? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        bind = ActivityIrDetailsBinding.inflate(layoutInflater)
        setContentView(bind.root)
        appContext = (applicationContext as IRApplication)
        appContext?.onMsgData?.observe(this) { msg ->
            if (msg.contains("HubDetailsEvent")) {
                try {
                    val jsonObject = JSONObject(msg)
                    if (jsonObject.has("data")) {
                        val data = jsonObject.getJSONObject("data")
                        if (data.has("temp"))
                            bind.txtTemp.text = (data.getString("temp"))
                        if (data.has("lux"))
                            bind.txtLux.text = (data.getString("lux"))
                    }
                } catch (e: Exception) {
                    e.printStackTrace()
                }
            }
        }
    }

}
