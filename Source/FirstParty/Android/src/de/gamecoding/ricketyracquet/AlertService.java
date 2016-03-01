package de.gamecoding.ricketyracquet;

import android.app.IntentService;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

public class AlertService extends IntentService {

    public static final String ERROR_EXTRA_KEY = "msg";

    public AlertService() {
        super("AlertService");
        Log.i(this.getClass().getName(), "Created service");
    }

    @Override
    protected void onHandleIntent(Intent workIntent) {
        String msg = workIntent.getStringExtra(ERROR_EXTRA_KEY);
        Log.i(this.getClass().getName(), "Service intent received with msg: " + msg);
        Intent intent = new Intent(this, AlertActivity.class);
        intent.putExtra(ERROR_EXTRA_KEY, msg);
        intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        startActivity(intent);
    }
}
