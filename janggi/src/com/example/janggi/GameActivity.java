package com.example.janggi;

import com.example.janggi.LocalService.LocalBinder;

import android.annotation.SuppressLint;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.media.MediaPlayer;
import android.net.wifi.p2p.WifiP2pManager.ActionListener;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.LinearLayout.LayoutParams;

@SuppressLint("NewApi")
public class GameActivity extends ActionBarActivity {
	private final int BLK = 9;
	private final int B_CH = 1;
	private final int B_KI = 2;
	private final int B_MA = 3;
	private final int B_PA = 4;
	private final int B_PO = 5;
	private final int B_SA = 6;
	private final int B_SAN = 7;
	private final int R_CH = 11;
	private final int R_KI = 12;
	private final int R_MA = 13;
	private final int R_PA = 14;
	private final int R_PO = 15;
	private final int R_SA = 16;
	private final int R_SAN = 17;
	ImageButton IB[][];
	GameBoard g;
	LocalService mService;
	MediaPlayer move;
	MediaPlayer janggun;
	@Override
	protected void onStart() {
		super.onStart();
		Intent intent = new Intent(this, LocalService.class);
		bindService(intent, mConnection, Context.BIND_AUTO_CREATE);
	}

	@Override
	protected void onStop() {
		super.onStop();
		unbindService(mConnection);
		g.timer_end();
	}

	private ServiceConnection mConnection = new ServiceConnection() {

		@Override
		public void onServiceConnected(ComponentName className, IBinder service) {
			// We've bound to LocalService, cast the IBinder and get
			// LocalService instance
			LocalBinder binder = (LocalBinder) service;
			mService = binder.getService();
		}

		@Override
		public void onServiceDisconnected(ComponentName arg0) {
		}
	};

	static {
		System.loadLibrary("janggi_c");
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.game_view);
		move=MediaPlayer.create(this, R.raw.move);
		janggun=MediaPlayer.create(this,R.raw.janggun);
		g = new GameBoard();
	}

	public class GameBoard implements View.OnClickListener {
		static final int row = 10;
		static final int col = 9;
		static final int RED = 1;
		static final int BLUE = 0;
		int turn;
		int bt = 0;
		boolean clicked;
		int clicked_row;
		int clicked_col;
		int[][] board;
		ImageView img;
		interrupt_thread th;
		private native int[] Poss_pos(int r, int c, int[][] board);

		private native void timer_start();

		private native void timer_turn_change();

		private native void timer_end();
		
		GameBoard() {
			int i, j;
			turn = BLUE;
			clicked = false;
			timer_start();
			IB = new ImageButton[10][9];
			img = (ImageView) findViewById(R.id.imageView1);
			for (i = 0; i < 10; i++) {
				for (j = 0; j < 9; j++) {
					String imagebutton_id = "imageButton" + Integer.toString(i)
							+ Integer.toString(j);
					int resID = getResources().getIdentifier(imagebutton_id,
							"id", getPackageName());
					IB[i][j] = (ImageButton) findViewById(resID);
					IB[i][j].setOnClickListener(this);
				}
			}
			board = new int[][] {
					{ B_CH, B_SAN, B_MA, B_SA, BLK, B_SA, B_SAN, B_MA, B_CH },
					{ BLK, BLK, BLK, BLK, B_KI, BLK, BLK, BLK, BLK },
					{ BLK, B_PO, BLK, BLK, BLK, BLK, BLK, B_PO, BLK },
					{ B_PA, BLK, B_PA, BLK, B_PA, BLK, B_PA, BLK, B_PA },
					{ BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK },
					{ BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK, BLK },
					{ R_PA, BLK, R_PA, BLK, R_PA, BLK, R_PA, BLK, R_PA },
					{ BLK, R_PO, BLK, BLK, BLK, BLK, BLK, R_PO, BLK },
					{ BLK, BLK, BLK, BLK, R_KI, BLK, BLK, BLK, BLK },
					{ R_CH, R_MA, R_SAN, R_SA, BLK, R_SA, R_MA, R_SAN, R_CH } };
			this.drawboard();
			th=new interrupt_thread();
			th.start();
			
		}

		private void drawboard() {
			int i, j;
			for (i = 0; i < row; i++) {
				for (j = 0; j < col; j++) {
					switch (board[i][j]) {
					case BLK:
						IB[i][j].setImageResource(R.drawable.blue_sa);
						IB[i][j].setAlpha(0);
						break;
					case B_CH:
						IB[i][j].setImageResource(R.drawable.blue_ch);
						IB[i][j].setAlpha(255);
						break;
					case B_KI:
						IB[i][j].setImageResource(R.drawable.blue_ki);
						IB[i][j].setAlpha(255);
						break;
					case B_MA:
						IB[i][j].setImageResource(R.drawable.blue_ma);
						IB[i][j].setAlpha(255);
						break;
					case B_PA:
						IB[i][j].setImageResource(R.drawable.blue_pa);
						IB[i][j].setAlpha(255);
						break;
					case B_PO:
						IB[i][j].setImageResource(R.drawable.blue_po);
						IB[i][j].setAlpha(255);
						break;
					case B_SA:
						IB[i][j].setImageResource(R.drawable.blue_sa);
						IB[i][j].setAlpha(255);
						break;
					case B_SAN:
						IB[i][j].setImageResource(R.drawable.blue_san);
						IB[i][j].setAlpha(255);
						break;
					case R_CH:
						IB[i][j].setImageResource(R.drawable.red_ch);
						IB[i][j].setAlpha(255);
						break;
					case R_KI:
						IB[i][j].setImageResource(R.drawable.red_ki);
						IB[i][j].setAlpha(255);
						break;
					case R_MA:
						IB[i][j].setImageResource(R.drawable.red_ma);
						IB[i][j].setAlpha(255);
						break;
					case R_PA:
						IB[i][j].setImageResource(R.drawable.red_pa);
						IB[i][j].setAlpha(255);
						break;
					case R_PO:
						IB[i][j].setImageResource(R.drawable.red_po);
						IB[i][j].setAlpha(255);
						break;
					case R_SA:
						IB[i][j].setImageResource(R.drawable.red_sa);
						IB[i][j].setAlpha(255);
						break;
					case R_SAN:
						IB[i][j].setImageResource(R.drawable.red_san);
						IB[i][j].setAlpha(255);
						break;
					}
					if (clicked == true && clicked_row == i && clicked_col == j)
						IB[i][j].setAlpha(170);
				}
			}
		}

		@Override
		public void onClick(View v) {
			if (bt == 1)
				return;
			bt = 1;
			int i, j;
			int temp_turn = turn;
			int id = v.getId();
			for (i = 0; i < row; i++) {
				for (j = 0; j < col; j++) {
					String imagebutton_id = "imageButton" + Integer.toString(i)
							+ Integer.toString(j);
					int resID = getResources().getIdentifier(imagebutton_id,
							"id", getPackageName());
					if (id == resID) {
						// mService.turn_change();
						if (clicked == true) {
							move(i, j);
						}
						if ((board[i][j] > 9 && turn == RED)
								|| (board[i][j] < 9 && turn == BLUE)) {
							clicked_row = i;
							clicked_col = j;
							clicked = true;
						}
					}
				}
			}
			this.drawboard();
			bt = 0;
		}

		public void move(int r, int c) {
			int[] pos = new int[40];
			pos = Poss_pos(clicked_row, clicked_col, board);
			int i;
			for (i = 0; i < 40; i++) {
				if (pos[i] == -1)
					break;
				if (pos[i] == r && pos[i + 20] == c) {
					mService.move(move);
					board[r][c] = board[clicked_row][clicked_col];
					board[clicked_row][clicked_col] = BLK;
					clicked_row = clicked_col = -1;
					clicked = false;
					boolean win = check_win();
					Log.v("win", Boolean.toString(win));
					if (win == true) {
						if (turn == RED)
							img.setImageResource(R.drawable.right);
						img.setVisibility(View.VISIBLE);
						timer_end();
					} else {
						if (turn == BLUE)
							turn = RED;
						else
							turn = BLUE;
						timer_turn_change();
					}
					return;
				}
			}
		}

		public boolean check_win() {
			int i, j;
			int check = 0;
			for (j = 3; j <= 5; j++) {
				for (i = 0; i <= 2; i++) {
					if (board[i][j] == B_KI) {
						check = 1;
						break;
					}
				}
			}
			if (check == 0)
				return true;
			check = 0;
			for (j = 3; j <= 5; j++) {
				for (i = 7; i <= 9; i++) {
					if (board[i][j] == R_KI) {
						check = 1;
						break;
					}
				}
			}
			if (check == 0)
				return true;
			return false;
		}
		public class interrupt_thread extends Thread{
			private native void sense_interrupt();
			public void run(){
				while(true){
					sense_interrupt();
					turn=1-turn;
					timer_turn_change();
				}
			}
		}
	}

}
