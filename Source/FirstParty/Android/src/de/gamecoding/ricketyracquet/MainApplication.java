package de.gamecoding.ricketyracquet;

import android.app.ActivityManager;
import android.app.Application;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.BatteryManager;
import android.util.Log;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Timer;
import java.util.TimerTask;

public class MainApplication extends Application {

    native public void setBattery(int percent, int status);

    private Timer timer = new Timer("batteryTimer");

    private static final int CHARGE_UNPLUGGED = 1;
    private static final int CHARGE_CHARGING = 2;
    private static final int CHARGE_FULL = 3;
        
    static {
        System.loadLibrary("RicketyRacquet");
    }
    
    @Override
    public void onCreate() {
        super.onCreate();
        ActivityManager man = (ActivityManager)getSystemService(ACTIVITY_SERVICE);
        FileOutputStream stream = null;
        try{
            stream = openFileOutput("meminfo", MODE_PRIVATE);
            String mem = String.valueOf(man.getLargeMemoryClass());
            stream.write(man.getLargeMemoryClass());
        } catch(IOException e) {
            Log.e(this.getClass().getName(), "Couldn't write memory size");
        } finally {
            if (stream != null) {
                try {
                    stream.close();
                } catch(IOException e) {
                    Log.e(this.getClass().getName(), "Failed closing memory info file");
                }
            }
        }
        timer.scheduleAtFixedRate(new TimerTask() {
                public void run() {
                    IntentFilter ifilter = new IntentFilter(Intent.ACTION_BATTERY_CHANGED);
                    Intent batteryStatus = registerReceiver(null, ifilter);
                
                    int rawlevel = batteryStatus.getIntExtra(BatteryManager.EXTRA_LEVEL, -1);
                    int scale = batteryStatus.getIntExtra(BatteryManager.EXTRA_SCALE, -1);
                    int level = -1;
                    if (rawlevel >= 0 && scale > 0) {
                        level = (rawlevel * 100) / scale;
                    }
                    int status = batteryStatus.getIntExtra(BatteryManager.EXTRA_STATUS, -1);
                    int state = CHARGE_UNPLUGGED;
                    if (status == BatteryManager.BATTERY_STATUS_CHARGING) {
                        state = CHARGE_CHARGING;
                    } else if (status == BatteryManager.BATTERY_STATUS_FULL) {
                        state = CHARGE_FULL;
                    }
                    setBattery(level, state);
            }
        }, 1000, 2000);
    }
}
