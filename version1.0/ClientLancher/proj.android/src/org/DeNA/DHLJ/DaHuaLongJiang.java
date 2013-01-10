package org.DeNA.DHLJ;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Stack;
import java.util.TimerTask;

import org.cocos2dx.lib.Cocos2dxActivity;

import org.cocos2dx.lib.Cocos2dxEditText;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.json.JSONException;
import org.json.JSONObject;
import org.xmlpull.v1.XmlPullParserException;

import com.mobage.android.C2DMBaseReceiver;
import com.mobage.android.Error;
import com.mobage.android.Mobage;
import com.mobage.android.Mobage.PlatformListener;
import com.mobage.android.Mobage.ServerMode;
import com.mobage.android.cn.dynamicmenubar.DynamicMenuBar;
import com.mobage.android.social.BalanceButton;
import com.mobage.android.social.common.RemoteNotification;
import com.mobage.android.social.common.RemoteNotification.RemoteNotificationListener;
import com.mobage.android.social.common.RemoteNotificationResponse;

import org.DeNA.DHLJ.PushService;

import org.DeNA.DHLJ.SocialUtils;
//import android.app.ActionBar.LayoutParams;
import android.app.AlertDialog;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences.Editor;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.res.Configuration;
import android.net.ParseException;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewParent;
import android.webkit.CookieSyncManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.Toast;
import android.app.AlertDialog;
import android.content.Context;
import android.graphics.PixelFormat;
import android.graphics.Rect;
import android.os.Bundle;
import android.os.Environment;
import android.provider.Settings.Secure;
import android.text.Layout;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;
import android.widget.VideoView;

public class DaHuaLongJiang extends Cocos2dxActivity
{
	private String mDeviceID;
	private static final String TAG = "DaHuaLongJiang";
	public static DaHuaLongJiang ms_pkDHLJ = null;
	private PlatformListener mPlatformListener;
	public static DynamicMenuBar menubar;
	private static BalanceButton balancebutton;
	private static float s_fScaleX;
	private static float s_fScaleY;
	private View rootView = null;
	private static boolean m_bIsStartingVideo = false;
	private final static boolean playVideoInActivity = true; //�Ƿ��ڶ�����activity�в�����Ƶ
	private static Context s_context;
	private static LinearLayout s_balancelayout;

	private static Cocos2dxEditText edittext; //@ime
	private static Button testbutton;

	private WindowManager wm=null;
	private static FloatView myFV=null;
	private static int FVAlpha=255;

	private WindowManager.LayoutParams wmParams=new WindowManager.LayoutParams();
	java.util.Timer timer = new java.util.Timer(true);

	public static WindowManager.LayoutParams getMywmParams(){
		return ms_pkDHLJ.wmParams;
	}
	private static Handler VideoViewHandler = new Handler();
	private static Handler RootViewHandler = new Handler();
	private static Runnable mHideBalance = new Runnable()
	{
		public void run()
		{
			s_balancelayout.setVisibility(View.INVISIBLE);
		};
	};
	private static Runnable mShowVideoView = new Runnable()
	{
		public void run()
		{
			LinearLayout pkLinearLayout = new LinearLayout(ms_pkDHLJ);
			pkLinearLayout.addView(m_pkView);
			pkLinearLayout.setOrientation(LinearLayout.HORIZONTAL);
			LinearLayout.LayoutParams pkParams = new LinearLayout.LayoutParams(
					ViewGroup.LayoutParams.FILL_PARENT,
					ViewGroup.LayoutParams.FILL_PARENT);
			pkParams.gravity = Gravity.CENTER;
			ms_pkDHLJ.setContentView(pkLinearLayout, pkParams);
			m_pkView.setLayoutParams(pkParams);
			// ms_pkDHLJ.rootView.setVisibility(View.INVISIBLE);
			// ms_pkDHLJ.balancebutton.setVisibility(View.INVISIBLE);
			// menubar.setVisibility(View.INVISIBLE);
			m_pkView.start();
			m_pkView.setVisibility(View.VISIBLE);
		};
	};

	private static Runnable mContinueRootView = new Runnable()
	{
		public void run()
		{
			// ms_pkDHLJ.setMain();
			ViewGroup.LayoutParams pkParams = new ViewGroup.LayoutParams(
					ViewGroup.LayoutParams.FILL_PARENT,
					ViewGroup.LayoutParams.FILL_PARENT);
			// ms_pkDHLJ.nativeInit(100, 200);
			ms_pkDHLJ.setContentView(menubar, pkParams);
			ms_pkDHLJ.rootView.setVisibility(View.VISIBLE);
			// ms_pkDHLJ.balancebutton.setVisibility(View.INVISIBLE);
			// menubar.setVisibility(View.INVISIBLE);
			// m_pkView.start();
			// m_pkView.setVisibility(View.INVISIBLE);
		};
	};

	public void continueRootView()
	{
		Log.i("DaHuaLongJiang", "continueRootView");
		VideoViewHandler.post(mContinueRootView);
		resumeBackgroundMusic();
	}

	private static Handler BalanceHandler = new Handler();
	private static Runnable mUpdateBalance = new Runnable()
	{
		public void run()
		{
			s_balancelayout.setVisibility(View.VISIBLE);
			balancebutton.update();
		};
	};


	protected void onCreate(Bundle savedInstanceState)
	{
		if (isSDCardCanUse())
		{
			ms_pkDHLJ = this;
			super.onCreate(savedInstanceState);
			Log.e(TAG, "onCreate called");

			Context context = getApplication().getApplicationContext();
			CookieSyncManager.createInstance(this);
			s_context = context;

			Mobage.registerMobageResource(this, "tw.mobage.g23000052.R");
//			RemoteNotificationView.DisableRemoteNotification();
			SocialUtils.initializeMobage(this);
			mPlatformListener = SocialUtils.createPlatformListener(true);
			Mobage.addPlatformListener(mPlatformListener);

			RemoteNotification.setListener(new RemoteNotificationListener()
			{
				@Override
				public void handleReceive(Context context, Intent intent)
				{
					// You can use static method which performing a notification
					// in status bar.
					C2DMBaseReceiver.displayStatusBarNotification(context,
							intent);
				}
			});

			nativeInit(480, 320);

			Mobage.checkLoginStatus();
			Mobage.onCreate();

			mDeviceID = Secure.getString(this.getContentResolver(),
					Secure.ANDROID_ID);
			PushService.actionStart(context);

			menubar = new DynamicMenuBar(this);
			menubar.setMenubarVisibility(View.VISIBLE);
			menubar.setMenuIconGravity(Gravity.TOP | Gravity.LEFT);

			Rect rect = new Rect(0, 0, 160, 90);
			balancebutton = com.mobage.android.social.common.Service
					.getBalanceButton(rect);

//			 testbutton = new Button(this);
//			 testbutton.setText("aaaaaaaaa".toCharArray(), 1, 6);
//			 FrameLayout.LayoutParams pkParamsButton = new
//			 FrameLayout.LayoutParams(200,200);
//			 pkParamsButton.topMargin = 100;
//			 pkParamsButton.leftMargin = 10;
//			 testbutton.setLayoutParams(pkParamsButton);
//			
//			 testbutton.setOnClickListener(new OnClickListener() {
//			 @Override
//			 public void onClick(View view) {
//				 menubar.setRankButtonVisibility(View.INVISIBLE);
//			 //FeedsView.openActivityFeeds();
//			 // RemoteNotificationView.SendRemoteNotification("500002013");
//			 // RemoteNotificationView.SendRemoteNotification("500001919");
//			 // PeopleView.getFriendsWithGame();
//			 // PeopleView.getFriends();
//			 // PeopleView.getUsers();
//			 // PeopleView.getCurrentUser();
//			 // PeopleView.getUser();
//			 }
//			 });
		} else
		{
			AlertDialog alertDialog = new AlertDialog.Builder(this).create();
			alertDialog.setTitle("Title");
			alertDialog.setMessage("Message");

			// alertDialog.setIcon(R.drawable.dhlj_icon);
			alertDialog.show();
		}
	}

	@Override
	public void onPause()
	{
		super.onPause();
		Mobage.onPause();
	}

	@Override
	public void onResume()
	{
		Mobage.setCurrentActivity(this);
		super.onResume();
		Mobage.onResume();
	}

	@Override
	public void onStop()
	{
		Log.e(TAG, "onStop called");
		super.onStop();

		if(myFV != null)
			myFV.setVisibility(View.INVISIBLE);
	}

	@Override
	public void onDestroy()
	{
		Log.e(TAG, "onDestroy called");
		super.onDestroy();
		Mobage.onStop();
    	wm.removeView(myFV);
	}

	@Override
	public void onRestart()
	{
		Log.e(TAG, "onRestart called");
		super.onRestart();
		Mobage.onRestart();
		if(myFV != null)
			myFV.setVisibility(View.VISIBLE);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig)
	{
		super.onConfigurationChanged(newConfig);
	}

	public void setMain()
	{
		Log.d(TAG, "DaHuaLongJiang::setMain()");

		// remove all views
		rootView = (View) getView();
		FrameLayout parent = (FrameLayout) rootView.getParent();
		if (parent != null)
		{
			parent.removeView(rootView);
		}
		menubar.removeAllViews();

		// add edit view
		addEditView();

		// add surface view
		m_pkView.setVisibility(View.INVISIBLE);
		// menubar.addView(m_pkView);
		menubar.addView(rootView);

		s_balancelayout = new LinearLayout(s_context);
		s_balancelayout.setOrientation(LinearLayout.VERTICAL);

		LinearLayout.LayoutParams layoutParams = new LinearLayout.LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT);
		
		setScaleX();
		Float x = 200 * s_fScaleX;
		Float y = 70 * s_fScaleY;
		Float sizex = 80 * s_fScaleX;
		Float sizey = 40 * s_fScaleY;
		layoutParams.topMargin = y.intValue();
		layoutParams.leftMargin = x.intValue();
		layoutParams.width = sizex.intValue();
		layoutParams.height = sizey.intValue();

		s_balancelayout.addView(balancebutton, layoutParams);

		menubar.addView(s_balancelayout);
		s_balancelayout.setVisibility(View.INVISIBLE);
		
//		menubar.addView(testbutton);

		// set content view
		ViewGroup.LayoutParams pkParams = new ViewGroup.LayoutParams(
				ViewGroup.LayoutParams.FILL_PARENT,
				ViewGroup.LayoutParams.FILL_PARENT);
		this.setContentView(menubar, pkParams);

		// set menu bar visible
		menubar.setMenubarVisibility(View.VISIBLE);
		
		createFloatView();
	}

	private static void dump_menubar()
	{
		int n = menubar.getChildCount();
		for (int i = 0; i < n; i++)
		{
			View v = menubar.getChildAt(i);
			Log.d("test", "@@ menubar.child["+i+"]="+v.toString() + ",vis=" + v.getVisibility());
		}		
	}
	
	// @ime
	public void addEditView()
	{
		if (edittext == null)
		{
			ViewGroup.LayoutParams edittext_layout_params = new ViewGroup.LayoutParams(
					ViewGroup.LayoutParams.FILL_PARENT,
					ViewGroup.LayoutParams.WRAP_CONTENT);

			edittext = new Cocos2dxEditText(this);
			edittext.setLayoutParams(edittext_layout_params);
		}

		if (edittext != null)
		{
			// add edit to layout
			menubar.addView(edittext);

			// set this edit to cocos2dx surface view
			Cocos2dxGLSurfaceView surfaceView = getView();
			surfaceView.setCocos2dxEditText(edittext);
			surfaceView.setDHLJ(this);

			// single line
			edittext.setSingleLine();
		}
	}

	//@ime
	//1=yes, 0=no, -1=unknown.
	private int isFullScreenIME()
	{
		int ret = -1;
		if (getView() != null)
		{
			final InputMethodManager imm = (InputMethodManager) 
					getView().getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
			
			if (imm != null)
			{
				ret = imm.isFullscreenMode() ? 1 : 0;
			}
		}
		Log.d("test", "@@ ime isFull: " + ret);
		return ret;
	}

	//@ime
	public void notifyIMEOpenClose( boolean bImeOpen ) 
	{
		Log.d("test", "@@ DaHuaLongJiang.notifyIMEOpenClose(): " + (bImeOpen ? "open" : "close"));
		
		//refreshLayout( bOpen );
		if (true)// || isFullScreenIME() == 0)
		{
			if (bImeOpen) 
			{
				//bring editView to top
				menubar.bringChildToFront(edittext);
			}
			else 
			{
				//bring surface view to top
				menubar.bringChildToFront(getView());
				bringLayoutToFront();
				menubar.bringToFront();
				menubar.postInvalidate();
				dump_menubar();
			}
		}
	}

	private static void bringLayoutToFront()
	{
		Log.d("test","@@ bringLayoutToFront()");
		
		View vLinearLayout = null;
		View vRelativeLayout = null;
		
		int n = menubar.getChildCount();
		for (int i = 0; i < n; i++)
		{
			View v = menubar.getChildAt(i);
			if (v.toString().indexOf("LinearLayout") != -1)
			{
				vLinearLayout = v;
			}
			else if (v.toString().indexOf("RelativeLayout") != -1)
			{
				vRelativeLayout = v;
			}			
		}
		
		if (vLinearLayout != null)
			menubar.bringChildToFront(vLinearLayout);
		
		if (vRelativeLayout != null)
			menubar.bringChildToFront(vRelativeLayout);		
	}
	public void LoginComplete(int userid)
	{
		onLoginComplete(userid, mDeviceID);
	}

	public void LoginError(String error)
	{
		onLoginError(error);
	}

	private void setScaleX()
	{
		Log.v(TAG, "begin setScaleX");

		DisplayMetrics dm = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(dm);

		s_fScaleX = 2.0f * dm.widthPixels / 960.0f;
		s_fScaleY = 2.0f * dm.heightPixels / 640.0f;
	}

	private static void showBalanceButton()
	{
		Log.v(TAG, "begin showBalanceButton");

		BalanceHandler.post(mUpdateBalance);
	}

	private static void hideBalanceButton()
	{
		Log.v(TAG, "begin showBalanceButton");
		BalanceHandler.post(mHideBalance);
	}

	public static String getTextFromStringXML(int nTextID)
	{
		String strRet = "";

		strRet = ms_pkDHLJ.getResources().getString(nTextID);

		return strRet;
	}

	public static String getStringFromJasonFile(String strBuffer,
			String strTextName)
	{
		String strRet = "";
		StringBuffer strReadBuffer = new StringBuffer();
		String strLine = null;

		strReadBuffer.append(strBuffer);
//			BufferedReader kReader = new BufferedReader(strBuffer);
//			while ((strLine = kReader.readLine()) != null)
//			{
//				
//			}

		try
		{
			JSONObject kJsonObject;
			kJsonObject = new JSONObject(strReadBuffer.toString());
			strRet = kJsonObject.getString(strTextName);
			// JSONArray provinces = jsonObject.getJSONArray("provinces");
			// String name = null;
			// StringBuffer jsonFileInfo = new StringBuffer();
			// JSONArray citys = null;
			// for (int i = 0; i < provinces.length(); i++)
			// {
			// name = provinces.getJSONObject(i).getString("name");
			// jsonFileInfo.append("/nname:" + name + "/n" + "citys:");
			// citys = provinces.getJSONObject(i).getJSONArray("citys");
			// for (int j = 0; j < citys.length(); j++)
			// {
			// jsonFileInfo.append(citys.getString(j) + "/t");
			// }
			// }
			//
			// System.out.println(jsonFileInfo);
		} catch (ParseException e)
		{
			e.printStackTrace();
		}
		catch (JSONException e1)
		{
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}

		return strRet;
	}

	public static void ShowBankUi()
	{
		Log.v(TAG, "begin ShowBankUi");
		com.mobage.android.social.common.Service
				.showBankUi(new com.mobage.android.social.common.Service.OnDialogComplete()
				{
					@Override
					public void onDismiss()
					{
					}
				});
	}

	private static void OpenUserProfile()
	{
		Log.v(TAG, "begin testGetUserProfile");
		String userId = SocialUtils.getmUserId();// Platform.getInstance().getUserId();
		com.mobage.android.social.common.Service.openUserProfile(userId,
				new com.mobage.android.social.common.Service.OnDialogComplete()
				{
					@Override
					public void onDismiss()
					{
						SocialUtils.showConfirmDialog("openUserProfile status",
								"Dismiss", "OK");
					}
				});
	}

	public void changeViewToVideo()
	{
	}

	public boolean isSDCardCanUse()
	{
		String strState = Environment.getExternalStorageState();

		if (Environment.MEDIA_MOUNTED.equals(strState))
		{
			return true;
		}

		return false;
	}
	
	//@video
	public static int playVideo(final String strFile)
	{
		Log.d("video", "@@ playVideo: " + strFile);
		
		pauseAllBackgroundMusic();

		if (playVideoInActivity)
		{
			ms_pkDHLJ.startVideoActivity();
		}
		else
		{
			m_bIsStartingVideo = true;
			VideoViewHandler.post(mShowVideoView);
		}

		return 0;
	}

	//@video
	private void startVideoActivity()
	{
		Log.d( "video", "@@ startVideoActivity()");

        Intent intent = new Intent(getApplication(), VideoActivity.class);
        startActivity(intent);
        
        Log.d( "video", "@@ startVideoActivity() -- done");
	}

	//@video
	public static int stopVideo(final String strFile)
	{
		return 0;
	}

	public static int openUrl(final String strFile)
	{
		if (ms_pkDHLJ == null)
		{
			return -1;
		}

		Intent it = new Intent(Intent.ACTION_VIEW, Uri.parse(strFile));
		it.setClassName("com.android.browser",
				"com.android.browser.BrowserActivity");
		ms_pkDHLJ.startActivity(it);

		return 0;
	}

	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0)
		{
			event.startTracking();
			return true;
		}
		return super.onKeyDown(keyCode, event);
	}

	public boolean onTouchEvent(MotionEvent event)
	{
		if (m_bIsStartingVideo)
		{
			m_pkView.stopPlayback();
			continueRootView();
		}

		return true;
	}

	private void getAndroidVer()
	{
		// Log.d( "getAndroidVer" , "@@ ver SDK: " +
		// android.os.Build.VERSION.SDK );
		// Log.d( "getAndroidVer" , "@@ ver release: " +
		// android.os.Build.VERSION.RELEASE );
		// Log.d( "getAndroidVer" , "@@ ver codename: " +
		// android.os.Build.VERSION.CODENAME );
		// Log.d( "getAndroidVer" , "@@ ver incremental: " +
		// android.os.Build.VERSION.INCREMENTAL );
		// Log.d( "getAndroidVer" , "@@ ver SDK: " +
		// android.os.Build.VERSION.SDK );
		// isVerOlder(0);
	}

	public static int isWifiConnected()
	{
		WifiManager wifiManger = (WifiManager) s_context
				.getSystemService(Service.WIFI_SERVICE);
		if (WifiManager.WIFI_STATE_ENABLED == wifiManger.getWifiState())
			return 1;
		return 0;
	}
	public static void FVClicked() {
		FVAlpha = 255;
		myFV.getBackground().setAlpha(FVAlpha);  
	}
	private static Handler FloatViewHandler = new Handler();
	private static Runnable mFloatViewRuner = new Runnable()
	{
		public void run()
		{
    		//myFV.setBackgroundDrawable(drawable);
			if(myFV != null) {
				if(FVAlpha > 20) {
					FVAlpha = FVAlpha-2;
					myFV.getBackground().setAlpha(FVAlpha);  
				}
			}
		};
	};
    TimerTask task = new TimerTask() {   
    	public void run() {  
    		FloatViewHandler.post(mFloatViewRuner); 
    	}   
    };   
	
    private void createFloatView(){
    	myFV=new FloatView(getApplicationContext());
    	//myFV.setImageResource(tw.mobage.g23000052.R.drawable.icon);
    	myFV.setBackgroundResource(tw.mobage.g23000052.R.drawable.icon);
    	//��ȡWindowManager
    	wm=(WindowManager)getApplicationContext().getSystemService("window");
        //����LayoutParams(ȫ�ֱ�������ز���
    	wmParams = getMywmParams();
        wmParams.type=WindowManager.LayoutParams.TYPE_PHONE;   //����window type
        wmParams.format=PixelFormat.RGBA_8888;   //����ͼƬ��ʽ��Ч��Ϊ����͸��

        //����Window flag
        wmParams.flags=WindowManager.LayoutParams.FLAG_NOT_TOUCH_MODAL
                              | WindowManager.LayoutParams.FLAG_NOT_FOCUSABLE;
        wmParams.gravity=Gravity.LEFT|Gravity.TOP;   //�����������������½�
        //����Ļ���Ͻ�Ϊԭ�㣬����x��y��ʼֵ

		DisplayMetrics dm = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(dm);
        wmParams.x=0;
        wmParams.y=dm.heightPixels-100;
        
        //�����������ڳ�������
        wmParams.width=80;
        wmParams.height=80;
    
        //��ʾFloatViewͼ��
        wm.addView(myFV, wmParams);
        timer.schedule(task, 0, 50);
    }
    	  
	// �Ƿ����ϵͳ
	public static int isVerOlder(int n)
	{
		String osVer = android.os.Build.VERSION.RELEASE;
		final String[] ver = osVer.split("\\.");

		// for (String v : ver)
		// {
		// Log.d( "ver", "@@ v: " + v );
		// }

		return (ver[0].compareTo("2") == 0) ? 1 : 0;
	}

	static
	{
		System.loadLibrary("mobage");
		System.loadLibrary("cocos2d");
		System.loadLibrary("GameLauncher");
	}

	private static native void pauseAllBackgroundMusic();

	private static native void resumeBackgroundMusic();

	private static native void nativeInit(int w, int h);

	private static native void onLoginComplete(int userid, String DeviceToken);

	private static native void onLoginError(String error);
}
