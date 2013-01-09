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
    private float mLastX;
    private float mLastY;
    
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
	     y = event.getRawY()-25;   //25��ϵͳ״̬���ĸ߶�
	     int nMoveX = 0;
	     int nMoveY = 0;
	     Log.i("currP", "currX"+x+"====currY"+y);
	     switch (event.getAction()) {
	        case MotionEvent.ACTION_DOWN:
	        	//��ȡ���View�����꣬���Դ�View���Ͻ�Ϊԭ��
	        	mTouchStartX =  event.getX();  
                mTouchStartY =  event.getY();

	    		mLastX = x;
	    		mLastY = y;
	            Log.i("startP", "startX"+mTouchStartX+"====startY"+mTouchStartY);
	            
	            break;
	        case MotionEvent.ACTION_MOVE:	            
	            updateViewPosition();
	            break;

	        case MotionEvent.ACTION_UP:
	    		nMoveX=(int)Math.abs(x-mLastX);
	    		nMoveY=(int)Math.abs(y-mLastY);
	            Log.i("MoveP", "nMoveX"+nMoveX+"====nMoveY"+nMoveY);

	        	updateViewPosition();
	        	if(nMoveX < 4 && nMoveY < 4) {
   			 		FeedsView.openActivityFeeds();
   		            DaHuaLongJiang.FVClicked();
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

}
