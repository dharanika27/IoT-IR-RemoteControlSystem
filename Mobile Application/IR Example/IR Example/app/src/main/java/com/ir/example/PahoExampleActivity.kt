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

import android.content.Intent
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import com.ir.example.databinding.ActivityPahoExampleBinding
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class PahoExampleActivity : AppCompatActivity() {
    private lateinit var bind: ActivityPahoExampleBinding
    var appContext:IRApplication? = null
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        bind = ActivityPahoExampleBinding.inflate(layoutInflater)
        setContentView(bind.root)
        appContext = (applicationContext as IRApplication)

        appContext?.onConnectionData?.observe(this) {
            bind.tvConnectionStatus.text = it
        }
        bind.irDetailsTv.setOnClickListener {
            startActivity(Intent(this, IRDetailsActivity::class.java))
        }
        bind.learnRepeatTv.setOnClickListener {
            startActivity(Intent(this, IRLearnRepeatActivity::class.java))
        }
    }


}
