package de.gamecoding.ricketyracquet;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;

public class AlertActivity extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        final Builder alertDialog = new AlertDialog.Builder(this);
        alertDialog.setTitle("Error");
        Intent intent = getIntent();
        alertDialog.setMessage(intent.getStringExtra(AlertService.ERROR_EXTRA_KEY));
        alertDialog.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int which) { 
                dialog.cancel();
            }
        });
        alertDialog.show();
    }
}