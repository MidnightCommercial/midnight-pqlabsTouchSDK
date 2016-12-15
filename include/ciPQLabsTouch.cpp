#include "ciPQLabsTouch.h"
#include "cinder/Log.h"

using namespace PQ_SDK_MultiTouch;
using namespace ci;

PQLabsTouch::PQLabsTouch()
{
}

PQLabsTouch::~PQLabsTouch()
{
	DisconnectServer();
}

int PQLabsTouch::connect( Service requested_services )
{
	int err_code = PQMTE_SUCCESS;

	// set the functions on server callback
	SetOnReceivePointFrame(&PQLabsTouch::OnReceivePointFrame, this);
	SetOnReceiveGesture(&PQLabsTouch::OnReceiveGesture, this);
	//TODO these should receive callbacks
	SetOnServerBreak(&PQLabsTouch::OnServerBreak, nullptr);
	SetOnReceiveError(&PQLabsTouch::OnReceiveError, nullptr);
	SetOnGetDeviceInfo(&PQLabsTouch::OnGetDeviceInfo, nullptr);
	// connect server
	CI_LOG_V( " connect to server...");
	if ((err_code = ConnectServer()) != PQMTE_SUCCESS) {
		CI_LOG_E(" connect server fail, socket error code:" << err_code );
		return err_code;
	}
	// send request to server
	CI_LOG_V(" connect success, send request." );
	TouchClientRequest tcq = { 0 };
	tcq.type = requested_services;
	;
	if ((err_code = SendRequest(tcq)) != PQMTE_SUCCESS) {
		CI_LOG_E(" send request fail, error code:" << err_code );
		return err_code;
	}


	//////// you can set the resolution of the touch point(raw data) here;
	//// setrawdata_resolution
	/*int maxX = 32768, maxY = 32768;
	if((err_code= SetRawDataResolution(maxX, maxY)) != PQMTE_SUCCESS){
		CI_LOG_E(" set raw data resolution fail, error code:" << err_code);
	}*/
	////////////////////////
	//get server resolution
	if ((err_code = GetServerResolution(OnGetServerResolution, NULL)) != PQMTE_SUCCESS) {
		CI_LOG_E(" get server resolution fail,error code:" << err_code);
		return err_code;
	}
	//
	// start receiving
	CI_LOG_V(" send request success, start recv.");
	return err_code;
}

int PQLabsTouch::setTouchMovementThreshold(int threshold_in_pixels)
{
	////////////you can set the move_threshold when the tcq.type is RQST_RAWDATA_INSIDE;
	////send threshold
	int err_code = PQMTE_SUCCESS;
	//int threshold_in_pixels = 0; receuve all the touch points that touching in the windows area of this client;
	if((err_code = SendThreshold(threshold_in_pixels)) != PQMTE_SUCCESS){
		CI_LOG_E(" send threadhold fail, error code:" << err_code);
	}
	return err_code;
}


void PQLabsTouch::OnReceivePointFrame(int frame_id, int time_stamp, int moving_point_count, const TouchPoint * moving_point_array, void * call_back_object)
{
	PQLabsTouch * sdk = static_cast<PQLabsTouch*>(call_back_object);
	CI_ASSERT(sdk != NULL);

	CI_LOG_V(" frame_id:" << frame_id << " time:" << time_stamp << " ms" << " moving point count:" << moving_point_count);
	for (int i = 0; i < moving_point_count; ++i) {
		TouchPoint tp = moving_point_array[i];
		sdk->mTouchSignal.emit(tp);

		switch (tp.type)
		{
		case 0 :
			sdk->mTouchBeginSignal.emit(tp);
			break;
		case 1:
			sdk->mTouchMoveSignal.emit(tp);
			break;
		case 2:
			sdk->mTouchEndSignal.emit(tp);
			break;
		default:
			CI_LOG_E("Unknown Touch type");
			break;
		}
	}
	//throw exception("test exception here");
}
void PQLabsTouch::OnReceiveGesture(const TouchGesture & ges, void * call_back_object)
{
	PQLabsTouch * sdk = static_cast<PQLabsTouch*>(call_back_object);
	CI_ASSERT(sdk != NULL);
	sdk->mGestureSignal.emit(ges);

	if (TG_NO_ACTION == ges.type)
		return;

	switch (ges.type)
	{
	case TG_TOUCH_START:
		sdk->mGestureStartSignal.emit(ges);
		break;
	case TG_TOUCH_END:
		sdk->mGestureEndSignal.emit(ges);
		break;
	case TG_DOWN:
		sdk->mGestureDownSignal.emit(ges);
		break;
	case TG_MOVE:
		sdk->mGestureMoveSignal.emit(ges);
		break;
	case TG_UP:
		sdk->mGestureUpSignal.emit(ges);
		break;
	case TG_MOVE_DOWN:
		sdk->mSwipeDownSignal.emit(ges);
		break;
	case TG_MOVE_UP:
		sdk->mSwipeUpSignal.emit(ges);
		break;
	case TG_MOVE_LEFT:
		sdk->mSwipeLeftSignal.emit(ges);
		break;
	case TG_MOVE_RIGHT:
		sdk->mSwipeRightSignal.emit(ges);
		break;
	case TG_CLICK:
		sdk->mClickSignal.emit(ges);
		break;
	case TG_DB_CLICK:
		sdk->mDoubleClickSignal.emit(ges);
		break;
	case TG_NEAR_PARALLEL_CLICK:
		sdk->mTwoFingerClick.emit(ges);
		break;
	default:
		CI_LOG_E("Unknown Gesture type, or gesture has no implemented signal");
		break;
	}

	//throw exception("test exception here");
}
void PQLabsTouch::OnServerBreak(void * param, void * call_back_object)
{
	// when the server break, disconenct server;
	CI_LOG_E( "server break, disconnecting" );
	DisconnectServer();
}
void PQLabsTouch::OnReceiveError(int err_code, void * call_back_object)
{
	switch (err_code)
	{
	case PQMTE_RCV_INVALIDATE_DATA:
		CI_LOG_E(" error: receive invalidate data.");
		break;
	case PQMTE_SERVER_VERSION_OLD:
		CI_LOG_E(" error: the multi-touch server is old for this client, please update the multi-touch server.");
		break;
	case PQMTE_EXCEPTION_FROM_CALLBACKFUNCTION:
		CI_LOG_E("**** some exceptions thrown from the call back functions.");
		CI_ASSERT(0); //need to add try/catch in the callback functions to fix the bug;
		break;
	default:
		CI_LOG_E( " socket error, socket error code:" << err_code );
	}
}
void PQLabsTouch::OnGetServerResolution(int x, int y, void * call_back_object)
{
	CI_LOG_V( " server resolution:" << x << "," << y );
}
void PQLabsTouch::OnGetDeviceInfo(const TouchDeviceInfo & deviceinfo, void *call_back_object)
{
	CI_LOG_V(" touch screen, SerialNumber: " << deviceinfo.serial_number << ",(" << deviceinfo.screen_width << "," << deviceinfo.screen_height << ").");
}