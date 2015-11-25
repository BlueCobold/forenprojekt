package de.gamecoding.ricketyracquet;

import android.app.ActivityManager;
import android.app.Application;
import android.util.Log;
import java.io.FileOutputStream;
import java.io.IOException;

public class MainApplication extends Application {

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
                try{
                    stream.close();
                } catch(IOException e) {
                    Log.e(this.getClass().getName(), "Failed closing memory info file");
                }
            }
        }
    }    
}
