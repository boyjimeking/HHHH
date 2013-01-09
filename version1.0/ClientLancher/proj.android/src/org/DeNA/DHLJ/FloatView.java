package org.DeNA.DHLJ;

import android.content.Context;
import android.util.Log;
import android.view.MotionEvent;
import android.view.WindowManager;
import android.widget.ImageView;

public class FloatView extends ImageView {
	private float mTouchStartX;
    private float mTouchStartY;
    private float x;
    private float y;
    private float mLastX1;
    private float mLastY1;
    private float mLastX2;
    private float mLastY2;
    
    private WindowManager wm=(WindowManager)getContext().getApplicationContext().getSystemService("window");
    
    //��wmParamsΪ��ȡ��ȫ�ֱ��������Ա����������ڵ�����
    private WindowManager.LayoutParams wmParams = DaHuaLongJiang.getMywmParams();

	public FloatView(Context context) {
		super(context);		
		// TODO Auto-generated constructor stub
	}
	
	 @Override
	 public boolean onTouchEvent(MotionEvent event) {
		 //��ȡ�����Ļ�����꣬������Ļ���Ͻ�Ϊԭ��		 
	     x = event.getRawX();   
	     y = event.getRawY();   //25��ϵͳ״̬���ĸ߶�
	     int nMoveX = 0;
	     int nMoveY = 0;
	     Log.i("currP", "currX"+x+"====currY"+y);
	     switch (event.getAction()) {
	        case MotionEvent.ACTION_DOWN:
	        	//��ȡ���View�����꣬���Դ�View���Ͻ�Ϊԭ��
	        	mTouchStartX =  event.getX();  
                mTouchStartY =  event.getY();

	    		mLastX1=(int)( x-mTouchStartX);
	    		mLastY1=(int) (y-mTouchStartY);
	            Log.i("startP", "startX"+mTouchStartX+"====startY"+mTouchStartY);
	            
	            break;
	        case MotionEvent.ACTION_MOVE:	            
	            updateViewPosition();
	            break;

	        case MotionEvent.ACTION_UP:
	    		mLastX2=(int)( x-event.getX());
	    		mLastY2=(int) (y-event.getY());
	    		nMoveX=(int)Math.abs(mLastX2-mLastX1);
	    		nMoveY=(int)Math.abs(mLastY2-mLastY1);
	            Log.i("MoveP", "nMoveX"+nMoveX+"====nMoveY"+nMoveY);

	        	if(nMoveX < 4 && nMoveY < 4) {
	        		restorViewPosition();
   			 		FeedsView.openActivityFeeds();
   		            DaHuaLongJiang.FVClicked();
	        	}
	        	else {
		        	updateViewPosition();
	        	}
	        	mTouchStartX=mTouchStartY=0;
	        	break;
	        }
	        return true;
		}
	 
	 private void updateViewPosition(){
		//���¸�������λ�ò���
		wmParams.x=(int)( x-mTouchStartX);
		wmParams.y=(int) (y-mTouchStartY);
	    wm.updateViewLayout(this, wmParams);
	 }

	 private void restorViewPosition(){
		//���¸�������λ�ò���
		wmParams.x=(int)( mLastX1);
		wmParams.y=(int) (mLastY1);
	    wm.updateViewLayout(this, wmParams);
	 }
}
