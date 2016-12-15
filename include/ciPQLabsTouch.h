#pragma once

#include "cinder/app/App.h"
#include "PQMTClient.h"
#include "cinder/Signals.h"

namespace pql = PQ_SDK_MultiTouch;

using GestureCallback = std::function<void(const pql::TouchGesture&)>;
using TouchCallback = std::function<void(const pql::TouchPoint&)>;

using GestureSignal = ci::signals::Signal<void(const pql::TouchGesture&)>;
using TouchSignal = ci::signals::Signal<void(const pql::TouchPoint&)>;


class PQLabsTouch : public ci::Noncopyable {
	
public:

	enum Service { RAW_TOUCH_ALL = RQST_RAWDATA_ALL, GESTURES_ALL = RQST_GESTURE_ALL, ALL = RQST_RAWDATA_ALL | RQST_GESTURE_ALL };

	PQLabsTouch();
	~PQLabsTouch();

	int connect( Service requested_services = Service::ALL );
	int setTouchMovementThreshold( int threshold_in_pixels );

	void connectTouchListener(const TouchCallback& callback, int priority = 0) { mTouchSignal.connect(priority, callback); }
	void connectGestureListener(const GestureCallback& callback, int priority = 0) { mGestureSignal.connect(priority, callback); }

	void connectTouchBeginListener(const TouchCallback& callback, int priority = 0) { mTouchBeginSignal.connect(priority, callback); }
	void connectTouchMoveListener(const TouchCallback& callback, int priority = 0) { mTouchMoveSignal.connect(priority, callback); }
	void connectTouchEndListener(const TouchCallback& callback, int priority = 0) { mTouchEndSignal.connect(priority, callback); }

	void connectGestureStartListener(const GestureCallback& callback, int priority = 0) { mGestureStartSignal.connect(priority, callback); }
	void connectGestureEndListener(const GestureCallback& callback, int priority = 0) { mGestureEndSignal.connect(priority, callback); }

	void connectGestureDownListener(const GestureCallback& callback, int priority = 0) { mGestureDownSignal.connect(priority, callback); }
	void connectGestureMoveListener(const GestureCallback& callback, int priority = 0) { mGestureMoveSignal.connect(priority, callback); }
	void connectGestureUpListener(const GestureCallback& callback, int priority = 0) { mGestureUpSignal.connect(priority, callback); }

	void connectClickListener(const GestureCallback& callback, int priority = 0) { mClickSignal.connect(priority, callback); }
	void connectDoubleClickListener(const GestureCallback& callback, int priority = 0) { mDoubleClickSignal.connect(priority, callback); }
	void connectTwoFingerClickListener(const GestureCallback& callback, int priority = 0) { mTwoFingerClick.connect(priority, callback); }

	void connectSwipeUpListener(const GestureCallback& callback, int priority = 0) { mSwipeUpSignal.connect(priority, callback); }
	void connectSwipeDownListener(const GestureCallback& callback, int priority = 0) { mSwipeDownSignal.connect(priority, callback); }
	void connectSwipeLeftListener(const GestureCallback& callback, int priority = 0) { mSwipeLeftSignal.connect(priority, callback); }
	void connectSwipeRightListener(const GestureCallback& callback, int priority = 0) { mSwipeRightSignal.connect(priority, callback); }

private:

	// OnReceivePointFrame: function to handle when recieve touch point frame
	//	the unmoving touch point won't be sent from server. The new touch point with its pointevent is TP_DOWN
	//	and the leaving touch point with its pointevent will be always sent from server;
	static void OnReceivePointFrame(int frame_id, int time_stamp, int moving_point_count, const PQ_SDK_MultiTouch::TouchPoint * moving_point_array, void * call_back_object);
	// OnReceivePointFrame: function to handle when recieve touch gesture
	static void OnReceiveGesture(const pql::TouchGesture & ges, void * call_back_object);
	// OnServerBreak: function to handle when server break(disconnect or network error)
	static void OnServerBreak(void * param, void * call_back_object);
	// OnReceiveError: function to handle when some errors occur on the process of receiving touch datas.
	static void OnReceiveError(int err_code, void * call_back_object);
	// OnGetServerResolution: function to get the resolution of the server system.attention: not the resolution of touch screen. 
	static void OnGetServerResolution(int x, int y, void * call_back_object);
	// OnGetDeviceInfo: function to get the information of the touch device.
	static void OnGetDeviceInfo(const pql::TouchDeviceInfo & device_info, void * call_back_object);

	TouchSignal mTouchSignal;
	GestureSignal mGestureSignal;

	TouchSignal mTouchBeginSignal;
	TouchSignal mTouchMoveSignal;
	TouchSignal mTouchEndSignal;

	//todo include all types of touch, pen, eraser, etc

	GestureSignal mGestureStartSignal;

	GestureSignal mGestureDownSignal;
	GestureSignal mGestureMoveSignal;
	GestureSignal mGestureUpSignal;

	GestureSignal mSwipeUpSignal;
	GestureSignal mSwipeRightSignal;
	GestureSignal mSwipeLeftSignal;
	GestureSignal mSwipeDownSignal;

	//GestureSignal mBigDownSignal;
	//GestureSignal mBigMoveSignal;
	//GestureSignal mBigUpSignal;

	GestureSignal mClickSignal;
	GestureSignal mDoubleClickSignal;
	GestureSignal mTwoFingerClick;

	/*GestureSignal mSecondDownSignal;
	GestureSignal mSecondUpSignal;

	GestureSignal mSecondClickSignal;
	GestureSignal mSecondDoubleClickSignal;

	GestureSignal mPinchStartSignal;
	GestureSignal mPinchApartSignal;
	GestureSignal mPinchCloseSignal;
	GestureSignal mPinchEndSignal;

	GestureSignal mRotateStartSignal;
	GestureSignal mRotateLeftSignal;
	GestureSignal mRotateRightSignal;
	GestureSignal mRotateEndSignal;*/

	GestureSignal mGestureEndSignal;

};

//PQLabsTouch::Service operator | (const PQLabsTouch::Service& lhs, const PQLabsTouch::Service& rhs)
//{
//	return (PQLabsTouch::Service)((int)lhs | (int)rhs);
//}