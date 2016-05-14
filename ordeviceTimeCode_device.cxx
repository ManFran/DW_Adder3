/***************************************************************************************
 Autodesk(R) Open Reality(R) Samples
 
 (C) 2009 Autodesk, Inc. and/or its licensors
 All rights reserved.
 
 AUTODESK SOFTWARE LICENSE AGREEMENT
 Autodesk, Inc. licenses this Software to you only upon the condition that 
 you accept all of the terms contained in the Software License Agreement ("Agreement") 
 that is embedded in or that is delivered with this Software. By selecting 
 the "I ACCEPT" button at the end of the Agreement or by copying, installing, 
 uploading, accessing or using all or any portion of the Software you agree 
 to enter into the Agreement. A contract is then formed between Autodesk and 
 either you personally, if you acquire the Software for yourself, or the company 
 or other legal entity for which you are acquiring the software.
 
 AUTODESK, INC., MAKES NO WARRANTY, EITHER EXPRESS OR IMPLIED, INCLUDING BUT 
 NOT LIMITED TO ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
 PURPOSE REGARDING THESE MATERIALS, AND MAKES SUCH MATERIALS AVAILABLE SOLELY ON AN 
 "AS-IS" BASIS.
 
 IN NO EVENT SHALL AUTODESK, INC., BE LIABLE TO ANYONE FOR SPECIAL, COLLATERAL, 
 INCIDENTAL, OR CONSEQUENTIAL DAMAGES IN CONNECTION WITH OR ARISING OUT OF PURCHASE 
 OR USE OF THESE MATERIALS. THE SOLE AND EXCLUSIVE LIABILITY TO AUTODESK, INC., 
 REGARDLESS OF THE FORM OF ACTION, SHALL NOT EXCEED THE PURCHASE PRICE OF THE 
 MATERIALS DESCRIBED HEREIN.
 
 Autodesk, Inc., reserves the right to revise and improve its products as it sees fit.
 
 Autodesk and Open Reality are registered trademarks or trademarks of Autodesk, Inc., 
 in the U.S.A. and/or other countries. All other brand names, product names, or 
 trademarks belong to their respective holders. 
 
 GOVERNMENT USE
 Use, duplication, or disclosure by the U.S. Government is subject to restrictions as 
 set forth in FAR 12.212 (Commercial Computer Software-Restricted Rights) and 
 DFAR 227.7202 (Rights in Technical Data and Computer Software), as applicable. 
 Manufacturer is Autodesk, Inc., 10 Duke Street, Montreal, Quebec, Canada, H3C 2L7.
***************************************************************************************/

/**	\file	ordeviceTimeCode_device.cxx
*	Function definitions for the ORDeviceTimeCode class.
*/

//--- Class declaration
#include "ordeviceTimeCode_device.h"

//--- Registration defines
#define ORDEVICETIMECODE__CLASS	ORDEVICETIMECODE__CLASSNAME
#define ORDEVICETIMECODE__NAME		ORDEVICETIMECODE__CLASSSTR
#define ORDEVICETIMECODE__LABEL		"OR - TimeCode Device"
#define ORDEVICETIMECODE__DESC		"OR - Time code sample"
#define ORDEVICETIMECODE__PREFIX	"TimeCode"

//--- FiLMBOX implementation and registration
FBDeviceImplementation	(	ORDEVICETIMECODE__CLASS	);
FBRegisterDevice		(	ORDEVICETIMECODE__NAME,
							ORDEVICETIMECODE__CLASS,
							ORDEVICETIMECODE__LABEL,
							ORDEVICETIMECODE__DESC,
							"devices_ltc.png"		);

/************************************************
 *	FiLMBOX Constructor.
 ************************************************/
bool ORDeviceTimeCode::FBCreate()
{
	// Create my animation nodes
	mTimeCodeAnimNode = AnimationNodeOutCreate( 0, "TimeCode",	ANIMATIONNODE_TYPE_TIMECODE );

	// Device sampling information
	SamplingMode = kFBSoftwareTimestamp;
	SamplingPeriod = FBTime(0,0,1)/24;

	mReferenceTimeIndex = mRefTime.Add("OR-TimeCode");
	return true;
}

/************************************************
 *	FiLMBOX Destructor.
 ************************************************/
void ORDeviceTimeCode::FBDestroy()
{
	mRefTime.Remove(mReferenceTimeIndex);
}

/************************************************
 *	Device operation.
 ************************************************/
bool ORDeviceTimeCode::DeviceOperation( kDeviceOperations pOperation )
{
	// Must return the online/offline status of device.
	switch (pOperation)
	{
		case kOpInit:	return true;
		case kOpStart:	return Start();
		case kOpStop:	return Stop();
		case kOpReset:	return Reset();
		case kOpDone:	return false;
	}
	return FBDevice::DeviceOperation( pOperation );
}

/************************************************
 *	Device is stopped (offline).
 ************************************************/
bool ORDeviceTimeCode::Stop()
{
	FBProgress	lProgress;
	lProgress.Caption	= "Shutting down device";

	lProgress.Text		= "Closing device communication";
	Information			= "Closing device communication";
	if(! mHardware.Close() )
	{
		Information = "Could not close device";
	}

    return false;
}


/************************************************
 *	Device is started (online).
 ************************************************/
bool ORDeviceTimeCode::Start()
{
	FBProgress	Progress;

	Progress.Caption	= "Setting up device";

	// Step 1: Open device
	if(! mHardware.Open() )
	{
		Information = "Could not open device";
		return false;
	}

	// Step 2: Ask hardware to get information
	Progress.Text	= "Device found, scanning for information...";
	Information		= "Retrieving information";
	if(!mHardware.GetSetupInfo())
	{
		Information = "Could not get information from device.";
		return false;
	}

    return true; // if true the device is online
}


/************************************************
 *	Reset of device.
 ************************************************/
bool ORDeviceTimeCode::Reset()
{
    Stop();
    return Start();
}

/************************************************
 *	Real-Time Engine Evaluation.
 ************************************************/
bool ORDeviceTimeCode::AnimationNodeNotify(FBAnimationNode* pAnimationNode ,FBEvaluateInfo* pEvaluateInfo)
{
	if(mTimeCodeAnimNode)
	{
		FBTimeCode tc = mHardware.GetData();
		mTimeCodeAnimNode->WriteData( (double*)&tc, pEvaluateInfo );
	}
    return true;
}


/************************************************
 *	Real-Time Synchronous Device IO.
 ************************************************/
void ORDeviceTimeCode::DeviceIONotify( kDeviceIOs pAction,FBDeviceNotifyInfo &pDeviceNotifyInfo)
{
	FBTime lEvalTime;
    switch (pAction)
	{
		case kIOPlayModeWrite:
		case kIOStopModeWrite:
		{
			// Output devices
		}
		break;

		case kIOStopModeRead:
		case kIOPlayModeRead:
		{
			if(mHardware.ReadTimeCode(lEvalTime))
			{
				DeviceRecordFrame(lEvalTime,pDeviceNotifyInfo);
				AckOneSampleReceived();
			}
		}
		break;
	}
}

/************************************************
 *	Record a frame of the device (recording).
 ************************************************/
void ORDeviceTimeCode::DeviceRecordFrame(FBTime &pTime,FBDeviceNotifyInfo &pDeviceNotifyInfo)
{
	FBTimeCode tc = mHardware.GetData();

	pTime = mSystem.LocalTime;

	mRefTime.SetTime(mReferenceTimeIndex,tc.GetTime(),mSystem.SystemTime);

	// Translation information.
	if (mTimeCodeAnimNode)
	{
		FBAnimationNode* Data = mTimeCodeAnimNode->GetAnimationToRecord();
		if (Data)
		{
			if( mPlayerControl.GetTransportMode() == kFBTransportPlay )
			{
				Data->KeyAdd(pTime, (double*)&tc);
			}
		}
	}
}

/************************************************
 *	Store data in FBX.
 ************************************************/
bool ORDeviceTimeCode::FbxStore(FBFbxObject* pFbxObject,kFbxObjectStore pStoreWhat)
{
	if (pStoreWhat & kAttributes)
	{
	}
	return true;
}


/************************************************
 *	Retrieve data from FBX.
 ************************************************/
bool ORDeviceTimeCode::FbxRetrieve(FBFbxObject* pFbxObject,kFbxObjectStore pStoreWhat)
{
	if( pStoreWhat & kAttributes )
	{
	}
	return true;
}
