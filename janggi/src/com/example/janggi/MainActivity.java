package com.example.janggi;

import android.support.v7.app.ActionBarActivity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

public class MainActivity extends ActionBarActivity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Button pvp = (Button)findViewById(R.id.pvp);
		pvp.setOnClickListener(new Button.OnClickListener(){
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, GameActivity.class);
				startActivity(intent);
			}
		});
		Button pvc = (Button)findViewById(R.id.pvc);
		pvc.setOnClickListener(new Button.OnClickListener(){
			@Override
			public void onClick(View v) {
				Intent intent = new Intent(MainActivity.this, GameActivity.class);
				startActivity(intent);
			}
		});
	}
}
