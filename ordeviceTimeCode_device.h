#ifndef __ORDEVICE_TIMECODE_DEVICE_H__
#define __ORDEVICE_TIMECODE_DEVICE_H__
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

/**	\file	ordeviceTimeCode_device.h
*	Declaration of the ORDeviceTimeCode class.
*/

//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Class declaration
#include "ordeviceTimeCode_hardware.h"

//--- Registration defines
#define ORDEVICETIMECODE__CLASSNAME	ORDeviceTimeCode
#define ORDEVICETIMECODE__CLASSSTR		"ORDeviceTimeCode"


//! TimeCode device.
class ORDeviceTimeCode : public FBDevice
{
	//--- FiLMBOX declaration
	FBDeviceDeclare( ORDeviceTimeCode, FBDevice );

public:
	//--- FiLMBOX Construction/Destruction
	virtual bool FBCreate();		//!< FiLMBOX Constructor.
	virtual void FBDestroy();		//!< FiLMBOX Destructor.

	//--- Initialisation/Shutdown
	bool  Reset();					//!< Reset function.
	bool  Stop();					//!< Device online routine.
	bool  Start();					//!< Device offline routine.

	//--- Real-Time Engine callbacks
	virtual bool AnimationNodeNotify( FBAnimationNode* pAnimationNode,FBEvaluateInfo* pEvaluateInfo	);		//!< Real-time evaluation function.
	virtual void DeviceIONotify		( kDeviceIOs  pAction, FBDeviceNotifyInfo &pDeviceNotifyInfo	);		//!< Hardware I/O notification.
	void	DeviceRecordFrame			(FBTime &pTime,FBDeviceNotifyInfo &pDeviceNotifyInfo);

	//--- Device operation
	virtual bool DeviceOperation	( kDeviceOperations pOperation									);		//!< Operate device.

	//--- Load/Save.
	virtual bool FbxStore	( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat		);		//!< Store configuration in FBX.
	virtual bool FbxRetrieve( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat		);		//!< Retrieve configuration from FBX.

public:
	int						mReferenceTimeIndex;		//!< ReferenceTime index.
	FBAnimationNode*		mTimeCodeAnimNode;

private:
	ORDeviceTimeCodeHardware	mHardware;					//!< Handle onto hardware.
	FBReferenceTime			mRefTime;
	FBPlayerControl			mPlayerControl;				//!< To get play mode for recording.
	FBSystem				mSystem;
};

#endif /* __ORDEVICE_TIMECODE_DEVICE_H__ */
