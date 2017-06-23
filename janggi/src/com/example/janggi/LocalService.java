package com.example.janggi;

import android.app.Service;
import android.content.Intent;
import android.media.MediaPlayer;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

public class LocalService extends Service{
	private final IBinder mBinder = new LocalBinder();
	public class LocalBinder extends Binder {
		LocalService getService(){
			return LocalService.this;
		}
	}
	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return mBinder;
	}
	public void move(MediaPlayer move){
		move.start();
	}
	public void janggun(MediaPlayer janggun){
		janggun.start();
	}

}
