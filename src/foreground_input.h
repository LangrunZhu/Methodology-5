/**
*** INTEL CONFIDENTIAL
*** 
*** Copyright (March 2011) (March 2011) Intel Corporation All Rights Reserved. 
*** The source code contained or described herein and all documents related to the
*** source code ("Material") are owned by Intel Corporation or its suppliers or 
*** licensors. Title to the Material remains with Intel Corporation or its 
*** suppliers and licensors. The Material contains trade secrets and proprietary 
*** and confidential information of Intel or its suppliers and licensors. 
*** The Material is protected by worldwide copyright and trade secret laws 
*** and treaty provisions. No part of the Material may be used, copied, 
*** reproduced, modified, published, uploaded, posted, transmitted, distributed,
*** or disclosed in any way without Intel's prior express written permission.
***
*** No license under any patent, copyright, trade secret or other intellectual
*** property right is granted to or conferred upon you by disclosure or delivery
*** of the Materials, either expressly, by implication, inducement, estoppel or
*** otherwise. Any license under such intellectual property rights must be 
*** express and approved by Intel in writing.
**/

#ifndef __INCLUDE_PURE_EVENT_DRIVEN_INCREMENTING_INPUT__
#define __INCLUDE_PURE_EVENT_DRIVEN_INCREMENTING_INPUT__
#ifndef __INCLUDE_HOOK_INPUT__
#define __INCLUDE_HOOK_INPUT__


//-----------------------------------------------------------------------------
// Headers inclusions.
//-----------------------------------------------------------------------------
#include <windows.h>
#include "pub_intel_modeler.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/*--------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Defines.
//-----------------------------------------------------------------------------
#define INPUT_PAUSE_IN_MS 100
#define STRING_BUFFERS_SIZE 1024
#define INPUT_NAME_STRING "Foreground Window"


#define INPUTS_COUNT 11
	#define PROCESS_ID 0
	#define THREAD_ID 1
	#define INPUT_LEFT 2
	#define INPUT_TOP 3
	#define INPUT_RIGHT 4
	#define INPUT_BOTTOM 5
	#define INPUT_SUCCESS 6
	#define IS_HUNG_APP_WINDOW 7
	#define IS_IMMERSIVE 8
	#define NAME_INDEX 9
	#define IMAGE_NAME 10

#define INPUT_DESCRIPTION_STRINGS \
	"Process ID of the foreground application", \
	"Thread ID of the foreground application", \
	"Left of the rectangle of foreground window", \
	"Top of the rectangle of foreground window", \
	"Right of the rectangle of foreground window", \
	"Bottom of the rectangle of foreground window", \
	"if the input success", \
	"if the application is responsive", \
	"if the application is a Windows Store app",\
	"name of the window",\
	"image name of the window"

#define INPUT_TYPES \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	STRING_COUNTER, \
	STRING_COUNTER

//-----------------------------------------------------------------------------
// Function prototypes.
//-----------------------------------------------------------------------------
ESRV_API ESRV_STATUS modeler_init_inputs(
	unsigned int *, 
	int *, 
	int *, 
	char *,
	size_t
);
ESRV_API ESRV_STATUS modeler_open_inputs(PINTEL_MODELER_INPUT_TABLE);
ESRV_API ESRV_STATUS modeler_close_inputs(PINTEL_MODELER_INPUT_TABLE);
ESRV_STATUS modeler_read_inputs(PINTEL_MODELER_INPUT_TABLE);
ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE);
ESRV_STATUS modeler_process_dctl(PINTEL_MODELER_INPUT_TABLE);
ESRV_STATUS modeler_process_lctl(PINTEL_MODELER_INPUT_TABLE);
//-----------------------------------------------------------------------------

ESRV_API unsigned int __stdcall custom_event_listner_thread(void*);
ESRV_API unsigned int __stdcall mouse_hook_listner_thread(void*);
unsigned int __stdcall get_object_info(void*);
unsigned int __stdcall mouse_messages_loop(void*);
LRESULT CALLBACK process_mouse_messages(int, WPARAM, LPARAM);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // __INCLUDE_HOOK_INPUT__
#endif // __INCLUDE_PURE_EVENT_DRIVEN_INCREMENTING_INPUT__

