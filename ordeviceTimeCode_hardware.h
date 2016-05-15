#ifndef __ORDEVICE_TIMECODE_HARDWARE_H__
#define __ORDEVICE_TIMECODE_HARDWARE_H__
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

/**	\file	ordeviceTimeCode_hardware.h
*	Declaration of a virtual hardware class.
*	ORDeviceTimeCodeHardware.
*/

//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Elements in array of models.
#define PARENT_A	0
#define CHILD_A		1
#define PARENT_B	2
#define CHILD_B		3

//--- Data codes.
enum {
	DATA_TX			= 0,
	DATA_TY			= 1,
	DATA_TZ			= 2,
	DATA_RX			= 3,
	DATA_RY			= 4,
	DATA_RZ			= 5,
	DATA_TYPE_COUNT = 6
};

//--- Number of channels.
#define MAX_CHANNEL 100

//! Simple hardware.
class ORDeviceTimeCodeHardware
{
public:
	//! Constructor.
	ORDeviceTimeCodeHardware();

	//! Destructor.
	~ORDeviceTimeCodeHardware();

	//--- Opens and closes connection with data server. returns true if successful
	bool	Open();								//!< Open the connection.
	bool	Close();							//!< Close connection.

	//--- Hardware communication
	bool	ReadTimeCode	(FBTime &pTime);		//!< Fetch a data packet from the computer.
	bool	PollData		();						//!< Poll the device for a data packet.
	bool	GetSetupInfo	();						//!< Get the setup information.

	//--- Data management
	FBTimeCode	GetData()		{	return mTimeCode;	}

private:
	FBSystem	mSystem;									//!< System interface.
	FBTimeCode  mTimeCode;

	long		mCounter;									//!< Time counter for hands.
};

#endif /* __ORDEVICE_TIMECODE_HARDWARE_H__ */
