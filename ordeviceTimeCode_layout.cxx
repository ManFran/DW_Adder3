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

/**	\file	ordeviceTimeCode_layout.cxx
*	Definition of the functions of the ORDeviceTimeCodeLayout class.
*/

//--- Class declarations
#include "ordeviceTimeCode_device.h"
#include "ordeviceTimeCode_layout.h"

#define ORDEVICETIMECODE__LAYOUT	ORDeviceTimeCodeLayout

//--- FiLMBOX implementation and registration
FBDeviceLayoutImplementation(	ORDEVICETIMECODE__LAYOUT		);
FBRegisterDeviceLayout		(	ORDEVICETIMECODE__LAYOUT,
								ORDEVICETIMECODE__CLASSSTR,
								FB_DEFAULT_SDK_ICON			);	// Icon filename (default=Open Reality icon)

/************************************************
 *	FiLMBOX constructor.
 ************************************************/
bool ORDeviceTimeCodeLayout::FBCreate()
{
	mDevice = ((ORDeviceTimeCode *)(FBDevice *)Device);

    int lB = 10;
	//int lS = 4;
	int lW = 120;
	int lH = 18;

	// Configure layout
	AddRegion( "TakeRecordingTest", "TakeRecordingTest",
										lB,	kFBAttachLeft,	"",	1.0	,
										lB,	kFBAttachTop,	"",	1.0,
										lW,	kFBAttachNone,	"",	1.0,
										lH,	kFBAttachNone,	"",	1.0 );
	SetControl( "TakeRecordingTest", mTakeRecordingTest );

	AddRegion( "StoryRecordingTest", "StoryRecordingTest",
										lB,	kFBAttachLeft,	"",	1.0	,
										lB,	kFBAttachBottom,	"TakeRecordingTest",	1.0,
										lW,	kFBAttachNone,	"",	1.0,
										lH,	kFBAttachNone,	"",	1.0 );
	SetControl( "StoryRecordingTest", mStoryRecordingTest );

	// Configure button
	mTakeRecordingTest.OnClick.Add( this, (FBCallback) &ORDeviceTimeCodeLayout::EventTakeRecordingTestClick );
	mTakeRecordingTest.Caption = "Take Recording test";

	mStoryRecordingTest.OnClick.Add( this, (FBCallback) &ORDeviceTimeCodeLayout::EventStoryRecordingTestClick );
	mStoryRecordingTest.Caption = "Story Recording test";

	mMarkerTCProp = NULL;

	FBSystem().OnUIIdle.Add( this,(FBCallback)&ORDeviceTimeCodeLayout::EventUIIdle );

	return true;
}


/************************************************
 *	FiLMBOX destructor.
 ************************************************/
void ORDeviceTimeCodeLayout::FBDestroy()
{
	FBSystem().OnUIIdle.Remove( this,(FBCallback)&ORDeviceTimeCodeLayout::EventUIIdle );
}

void ORDeviceTimeCodeLayout::RecordingTest(bool pTakeRecording)
{
	mDevice->Online = true;

	// Create the model
	FBModel* lMarker	= new FBModelMarker("MyMarker");
	lMarker->Show = true;
	lMarker->Selected = true;
	
    mMarkerTCProp = (FBPropertyAnimatable*)lMarker->PropertyCreate( "TimeCode", kFBPT_TimeCode, "TimeCode", true, true );
	mMarkerTCProp->SetAnimated(true);

	if(pTakeRecording)
	{
		mMarkerTCProp->GetAnimationNode()->RecordMode = true;
	} else
	{
		FBStoryTrack*	lTrack = new FBStoryTrack(kFBStoryTrackAnimation);

		lTrack->Details.Add(lMarker);

		lTrack->RecordClipPath = FBSystem().TempPath;

		lTrack->RecordTrack = true;
	}

	FBConnect(mDevice->mTimeCodeAnimNode,mMarkerTCProp->GetAnimationNode(),kFBConnectionTypeSystem);

	FBPlayerControl().Record();
	FBPlayerControl().Play();
}

void ORDeviceTimeCodeLayout::EventTakeRecordingTestClick( HISender pSender, HKEvent pEvent )
{
	RecordingTest(true);
}

void ORDeviceTimeCodeLayout::EventStoryRecordingTestClick( HISender pSender, HKEvent pEvent )
{
	RecordingTest(false);
}

void ORDeviceTimeCodeLayout::EventUIIdle( HISender pSender, HKEvent pEvent )
{
	if(mMarkerTCProp && !(bool)FBPlayerControl().IsRecording)
	{
		FBDisconnect(mDevice->mTimeCodeAnimNode,mMarkerTCProp->GetAnimationNode());
		mMarkerTCProp->GetAnimationNode()->RecordMode = true;
		mMarkerTCProp = NULL;
	}
}
