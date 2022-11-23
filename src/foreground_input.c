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

//-----------------------------------------------------------------------------
// Headers inclusions.
//-----------------------------------------------------------------------------
#include <assert.h>
#include <process.h> // for _beginthreadex
#include <windows.h>
#include <tchar.h>
#include <Psapi.h> // GetProcessImageFileName

#include "foreground_input.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------
BOOL f_stop = FALSE;
HHOOK h_mouse_hook = NULL;
POINT click_position = { 0 };
CRITICAL_SECTION cs = { NULL };
HANDLE h_click_detected = NULL;

//-----------------------------------------------------------------------------
// Custom event-listener thread data.
//-----------------------------------------------------------------------------
HANDLE p_thread = NULL;
DWORD p_thread_id = 0;
HANDLE p_stop = NULL;
HANDLE h_thread = NULL;
DWORD h_thread_id = 0;
HANDLE h_stop = NULL;

char class_name[STRING_BUFFERS_SIZE] = { '\0' };
char window_name[STRING_BUFFERS_SIZE] = { '\0' };
char image_name[MAX_PATH] = { '\0' };

/*-----------------------------------------------------------------------------
Function: modeler_init_inputs
Purpose : return the inputs count.
In      : pointer to an unsigned integer.
Out     : modified unsigned integer.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_init_inputs(
	unsigned int *p, 
	int *pfd, 
	int *pfe, 
	char *po,
	size_t os
) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);
	assert(pfd != NULL);
	assert(pfe != NULL);

	SIGNAL_PURE_EVENT_DRIVEN_MODE;
	SET_INPUTS_COUNT(INPUTS_COUNT);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}

/*-----------------------------------------------------------------------------
Function: modeler_open_inputs
Purpose : open inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_open_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	unsigned int i = 0;

	//-------------------------------------------------------------------------
	// Input descriptions.
	//-------------------------------------------------------------------------
	static char* descriptions[INPUTS_COUNT] = {
		INPUT_DESCRIPTION_STRINGS
	};
	static INTEL_MODELER_INPUT_TYPES types[INPUTS_COUNT] = {
		INPUT_TYPES
	};

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

		assert(p != NULL);

	//-------------------------------------------------------------------------
	// Start the pure event-driven thread.
	//-------------------------------------------------------------------------
	p_thread = (HANDLE)_beginthreadex(
		NULL,
		0,
		custom_event_listner_thread,
		(void*)p,
		0,
		(unsigned int*)&p_thread_id
	);
	if (p_thread == NULL) {
		goto modeler_open_inputs_error;
	}

	//-------------------------------------------------------------------------
	// Set input information.
	//-------------------------------------------------------------------------
	SET_INPUTS_NAME(INPUT_NAME_STRING);
	for (i = 0; i < INPUTS_COUNT; i++) {
		SET_INPUT_DESCRIPTION(
			i,
			descriptions[i]
		);
		SET_INPUT_TYPE(
			i,
			types[i]
		);
	} // for i (each input)

	//-------------------------------------------------------------------------
	// mouse hook pure event-driven thread
	//-------------------------------------------------------------------------
	h_thread = (HANDLE)_beginthreadex(
		NULL,
		0,
		mouse_hook_listner_thread,
		(void*)p,
		0,
		(unsigned int*)&h_thread_id
	);
	if (h_thread == NULL) {
		goto modeler_open_inputs_error;
	}

	//-------------------------------------------------------------------------
	// exit event
	//-------------------------------------------------------------------------
	h_stop = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL
	);
	if (h_stop == NULL) {
		goto modeler_open_inputs_error;
	}

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

modeler_open_inputs_error:

	return(ESRV_FAILURE);

}

/*-----------------------------------------------------------------------------
Function: modeler_close_inputs
Purpose : close inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_close_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	BOOL bret = FALSE;

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

//-------------------------------------------------------------------------
// Stop our event thread.
//-------------------------------------------------------------------------
	if (h_stop != NULL) {
		bret = SetEvent(h_stop);
		if (bret == FALSE) {
			goto modeler_close_inputs_error;
		}
	}

	//-------------------------------------------------------------------------
	// Free resources.
	//-------------------------------------------------------------------------
	if (h_stop != NULL) {
		bret = CloseHandle(h_stop);
		if (bret == FALSE) {
			goto modeler_close_inputs_error;
		}
		h_stop = NULL;
	}

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)
modeler_close_inputs_error:

	return(ESRV_FAILURE);
}

/*-----------------------------------------------------------------------------
Function: modeler_read_inputs
Purpose : collect all inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_read_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

		assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_listen_inputs
Purpose : listen for all inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_process_dctl
Purpose : process DCTL commands on DCTL interrupt notification.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_process_dctl(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_process_lctl
Purpose : process LCTL commands on LCTL interrupt notification.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_process_lctl(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}
/*-----------------------------------------------------------------------------
Function: custom_event_listner_thread
Purpose : implement the pure event listener thread.
In      : pointers to the input table (passed as void *).
Out     : modified input variables and time events list data.
Return  : status.
-----------------------------------------------------------------------------*/
unsigned int __stdcall custom_event_listner_thread(void* px) {
	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	DWORD dwret = 0;
	DWORD pid = 0;
	DWORD tid = 0;

	BOOL bret = FALSE;
	BOOL is_hung = FALSE;
	BOOL is_immersive = FALSE;

	HWND handle = NULL;
	HANDLE process = NULL;
	BOOL success = FALSE;

	RECT lprect;

	size_t stret = 0;

	TCHAR tchar_image_name[MAX_PATH] = { _T('\0') };
	TCHAR tchar_class_name[STRING_BUFFERS_SIZE] = { _T('\0') };
	TCHAR tchar_window_name[STRING_BUFFERS_SIZE] = { _T('\0') };


	char* token;
	char* lastName;
	char* lastImageName;

	//-------------------------------------------------------------------------
	// Watchdog variables.
	//-------------------------------------------------------------------------
	WATCHDOG_VARIABLES

	//-------------------------------------------------------------------------
	// Access helper variables.
	//-------------------------------------------------------------------------
	PINTEL_MODELER_INPUT_TABLE p = NULL;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

		assert(px != NULL);
	p = (PINTEL_MODELER_INPUT_TABLE)px;

	//-------------------------------------------------------------------------
	// Name this thread (for debug mode only).
	//-------------------------------------------------------------------------
	INPUT_DIAGNOSTIC_NAME_THIS_THREAD(
		p,
		"custom_event_listner_thread"
	);

	//-------------------------------------------------------------------------
	// Register this thread with watchdog.
	//-------------------------------------------------------------------------
	INPUT_REGISTER_EVENT_LOCKED_THREAD_WITH_WATCHDOG(
		p,
		"custom_event_listner_thread",
		p_thread,
		p_thread_id,
		STOP_SIGNAL,
		custom_event_listner_thread_exit
	);

	while (STOP_REQUEST == 0) {

		//---------------------------------------------------------------------
		// Pause to simulate event triggering.
		// Note:
		//    Rather than using a sleep, which would lock the event listener 
		//    thread, we recommend using the method shown below. In general
		//    developers of event-driven input libraries should add into the
		//    end condition the event / semaphore via the STOP_SIGNAL macro 
		//    (also - but not instead - use the STOP_REQUEST macro).
		//---------------------------------------------------------------------
		dwret = WaitForSingleObject(
			STOP_SIGNAL,
			INPUT_PAUSE_IN_MS
		);
		switch (dwret) {
		case WAIT_OBJECT_0:
			goto custom_event_listner_thread_exit; // time to leave!
			break;
		case WAIT_TIMEOUT:
			break; // all good, time to make a measurement
		default:
			goto custom_event_listner_thread_exit; // error condition
		} // switch

		

		handle = GetForegroundWindow();
		if (handle == NULL) {
			break;
		}

		tid = GetWindowThreadProcessId(
			handle,
			&pid
		);

		SET_INPUT_ULL_VALUE(
			PROCESS_ID,
			pid
		);

		SET_INPUT_ULL_VALUE(
			THREAD_ID,
			tid
		);

		//---------------------------------------------------------------------
		// Get the name of the window and class name
		//---------------------------------------------------------------------
		(void)InternalGetWindowText(
			handle,
			tchar_window_name,
			sizeof(tchar_window_name) / sizeof(TCHAR)
		);
		(void)wcstombs_s(
			&stret,
			window_name,
			sizeof(window_name),
			tchar_window_name,
			_TRUNCATE
		);
		token = strtok(window_name, "\\");
		lastName = NULL;
		while (token != NULL)
		{
			lastName = token;
			token = strtok(NULL, "\\");
		}
		SET_INPUT_STRING_ADDRESS(
			NAME_INDEX,
			lastName
		);
	
		success = GetWindowRect(handle, &lprect);

		//---------------------------------------------------------------------
		SET_INPUT_ULL_VALUE(
			INPUT_LEFT,
			lprect.left
		);
		//---------------------------------------------------------------------
		SET_INPUT_ULL_VALUE(
			INPUT_TOP,
			lprect.top
		);
		//---------------------------------------------------------------------
		SET_INPUT_ULL_VALUE(
			INPUT_RIGHT,
			lprect.right
		);
		//---------------------------------------------------------------------
		SET_INPUT_ULL_VALUE(
			INPUT_BOTTOM,
			lprect.bottom
		);
		//---------------------------------------------------------------------
		
		
		//---------------------------------------------------------------------
		// See if the window is hung
		//---------------------------------------------------------------------

		is_hung = IsHungAppWindow(handle);

		SET_INPUT_ULL_VALUE(
			IS_HUNG_APP_WINDOW,
			is_hung
		);

		//---------------------------------------------------------------------
		// See if the window is a Windows store app
		//---------------------------------------------------------------------
		is_immersive = IsImmersiveProcess(handle);
		SET_INPUT_ULL_VALUE(
			IS_IMMERSIVE,
			is_immersive
		);

		//---------------------------------------------------------------------
		// Get the owning process image
		//---------------------------------------------------------------------
		process = OpenProcess(
			PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE,
			pid
		);
		if (process != NULL) {
			is_immersive = IsImmersiveProcess(process);
			dwret = GetProcessImageFileName(
				process,
				tchar_image_name,
				sizeof(tchar_image_name)
			);
			bret = CloseHandle(process);
			process = NULL;
		}
		else {
			(void)_tcsncpy(
				tchar_image_name,
				_T("Unable To Open Process"),
				sizeof(tchar_image_name) / sizeof(TCHAR)
			);
		}
		(void)wcstombs_s(
			&stret,
			image_name,
			sizeof(image_name),
			tchar_image_name,
			_TRUNCATE
		);

		token = strtok(image_name, "\\");
		lastImageName = NULL;
		while (token != NULL)
		{
			lastImageName = token;
			token = strtok(NULL, "\\");
		}

		SET_INPUT_STRING_ADDRESS(
			IMAGE_NAME,
			lastImageName
		);
		//---------------------------------------------------------------------
		// Trigger logging.
		//---------------------------------------------------------------------
		LOG_INPUT_VALUES;

	} // while

custom_event_listner_thread_exit:

	//-------------------------------------------------------------------------
	// Un-register this thread with watchdog.
	//-------------------------------------------------------------------------
	INPUT_UNREGISTER_THREAD_WITH_WATCHDOG(
		p,
		h_thread,
		thread_id
	);

	//-------------------------------------------------------------------------
	// Success return hub.
	//-------------------------------------------------------------------------
	return(ESRV_SUCCESS);
	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

custom_event_listner_thread_error:

	//-------------------------------------------------------------------------
	// Failure return hub.
	//-------------------------------------------------------------------------
	return(ESRV_FAILURE);

}

/*-----------------------------------------------------------------------------
Function: mouse_hook_listner_thread
Purpose : implement the pure event listener thread.
In      : pointers to the input table (passed as void *).
Out     : modified input variables and time events list data.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API unsigned int __stdcall mouse_hook_listner_thread(void* px) {

	//-------------------------------------------------------------------------
	// Hook thread variables.
	//-------------------------------------------------------------------------
	HANDLE h_msg_loop_thread = NULL;
	DWORD msg_loop_thread_id = 0;
	HANDLE h_collector_thread = NULL;
	DWORD collector_thread_id = 0;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

		assert(px != NULL);

	//-------------------------------------------------------------------------
	// Setup threads and synch data.
	//-------------------------------------------------------------------------
	InitializeCriticalSection(&cs);
	//-------------------------------------------------------------------------
	h_click_detected = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL
	);
	if (h_click_detected == NULL) {
		goto custom_event_listner_thread_exit;
	}
	//-------------------------------------------------------------------------
	h_collector_thread = (HANDLE)_beginthreadex(
		NULL,
		0,
		custom_event_listner_thread,
		px,
		0,
		(unsigned int*)&collector_thread_id
	);
	if (h_collector_thread == NULL) {
		goto custom_event_listner_thread_exit;
	}
	//-------------------------------------------------------------------------
	h_msg_loop_thread = (HANDLE)_beginthreadex(
		NULL,
		0,
		mouse_messages_loop,
		NULL,
		0,
		(unsigned int*)&msg_loop_thread_id
	);
	if (h_msg_loop_thread == NULL) {
		goto custom_event_listner_thread_exit;
	}

	//-------------------------------------------------------------------------
	// Run the message loop!
	//-------------------------------------------------------------------------
	if (h_msg_loop_thread != NULL) {
		WaitForSingleObject(
			h_msg_loop_thread,
			INFINITE
		);
	}

custom_event_listner_thread_exit:

	//-------------------------------------------------------------------------
	// Free resources.
	//-------------------------------------------------------------------------
	if (h_click_detected != NULL) {
		CloseHandle(h_click_detected);
		h_click_detected = NULL;
	}
	DeleteCriticalSection(&cs);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}
/*-----------------------------------------------------------------------------
Function: mouse_messages_loop
Purpose : mouse message hook function.
In      : pointer - as void - to a message structure.
Out     : updated input data.
Return  : status.
-----------------------------------------------------------------------------*/
unsigned int __stdcall mouse_messages_loop(void* pv) {

	//-------------------------------------------------------------------------
	// Message handling variables.
	//-------------------------------------------------------------------------
	MSG message = { 0 };
	HINSTANCE h_instance = GetModuleHandle(NULL);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

		h_mouse_hook = SetWindowsHookEx(
			WH_MOUSE_LL,
			process_mouse_messages,
			h_instance,
			0
		);

	while (
		GetMessage(
			&message,
			NULL,
			0,
			0
		)
		) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UnhookWindowsHookEx(h_mouse_hook);

	return(0);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}

/*-----------------------------------------------------------------------------
Function: process_mouse_messages
Purpose : mouse event handler.
In      : mouse hook message data.
Out     : updated click position data.
Return  : status.
-----------------------------------------------------------------------------*/
LRESULT CALLBACK process_mouse_messages(
	int code,
	WPARAM wparam,
	LPARAM lparam
) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	BOOL bret = FALSE;

	//-------------------------------------------------------------------------
	// Message handling variables.
	//-------------------------------------------------------------------------
	MOUSEHOOKSTRUCT* p_mouse_struct = (MOUSEHOOKSTRUCT*)lparam;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

		//-------------------------------------------------------------------------
		// Do as MSDN says!
		//-------------------------------------------------------------------------
		if (code < 0) {
			goto process_mouse_messages_exit;
		}

	//-------------------------------------------------------------------------
	// Capture the data.
	//-------------------------------------------------------------------------
	if (
		(p_mouse_struct != NULL) &&
		(
			(wparam == WM_LBUTTONDOWN) ||
			(wparam == WM_RBUTTONDOWN)
			)
		) {
		bret = TryEnterCriticalSection(&cs);
		if (bret == FALSE) {
			goto process_mouse_messages_exit;
		}
		click_position.x = p_mouse_struct->pt.x;
		click_position.y = p_mouse_struct->pt.y;
		LeaveCriticalSection(&cs);
		SetEvent(h_click_detected);
	}

process_mouse_messages_exit:

	return(
		CallNextHookEx(
			h_mouse_hook,
			code,
			wparam,
			lparam
		)
		);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}
