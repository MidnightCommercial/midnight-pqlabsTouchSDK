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

	ci::signals::Connection connectTouchListener(const TouchCallback& callback, int priority = 0) { return mTouchSignal.connect(priority, callback); }
	ci::signals::Connection connectGestureListener(const GestureCallback& callback, int priority = 0) { return mGestureSignal.connect(priority, callback); }

	ci::signals::Connection connectTouchBeginListener(const TouchCallback& callback, int priority = 0) { return mTouchBeginSignal.connect(priority, callback); }
	ci::signals::Connection connectTouchMoveListener(const TouchCallback& callback, int priority = 0) { return mTouchMoveSignal.connect(priority, callback); }
	ci::signals::Connection connectTouchEndListener(const TouchCallback& callback, int priority = 0) { return mTouchEndSignal.connect(priority, callback); }

	ci::signals::Connection connectGestureStartListener(const GestureCallback& callback, int priority = 0) { return mGestureStartSignal.connect(priority, callback); }
	ci::signals::Connection connectGestureEndListener(const GestureCallback& callback, int priority = 0) { return mGestureEndSignal.connect(priority, callback); }

	ci::signals::Connection connectGestureDownListener(const GestureCallback& callback, int priority = 0) { return mGestureDownSignal.connect(priority, callback); }
	ci::signals::Connection connectGestureMoveListener(const GestureCallback& callback, int priority = 0) { return mGestureMoveSignal.connect(priority, callback); }
	ci::signals::Connection connectGestureUpListener(const GestureCallback& callback, int priority = 0) { return mGestureUpSignal.connect(priority, callback); }

	ci::signals::Connection connectClickListener(const GestureCallback& callback, int priority = 0) { return mClickSignal.connect(priority, callback); }
	ci::signals::Connection connectDoubleClickListener(const GestureCallback& callback, int priority = 0) { return mDoubleClickSignal.connect(priority, callback); }
	ci::signals::Connection connectTwoFingerClickListener(const GestureCallback& callback, int priority = 0) { return mTwoFingerClick.connect(priority, callback); }

	ci::signals::Connection connectSwipeUpListener(const GestureCallback& callback, int priority = 0) { return mSwipeUpSignal.connect(priority, callback); }
	ci::signals::Connection connectSwipeDownListener(const GestureCallback& callback, int priority = 0) { return mSwipeDownSignal.connect(priority, callback); }
	ci::signals::Connection connectSwipeLeftListener(const GestureCallback& callback, int priority = 0) { return mSwipeLeftSignal.connect(priority, callback); }
	ci::signals::Connection connectSwipeRightListener(const GestureCallback& callback, int priority = 0) { return mSwipeRightSignal.connect(priority, callback); }

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