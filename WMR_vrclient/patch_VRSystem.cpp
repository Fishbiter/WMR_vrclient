#include "stdafx.h"
#include "VTablePatch.h"
#include "stdlib.h"
#include "stdio.h"
#include "Settings.h"
#include <algorithm>
#include <deque>

MyVRSystem g_mySystem;

bool g_touched[5];
bool g_pressed[5];
std::deque<VREvent_t> g_queuedVirtualEvents;

void GenerateTrackPadEvents(int controller, VRControllerState_t* pState, bool releaseOnly)
{
	bool touched = (pState->ulButtonTouched & ButtonMaskFromId(k_EButton_SteamVR_Touchpad)) != 0;
	bool pressed = (pState->ulButtonPressed & ButtonMaskFromId(k_EButton_SteamVR_Touchpad)) != 0;
	
	if (!releaseOnly || g_touched[controller])
	{
		if (touched != g_touched[controller])
		{
			g_touched[controller] = touched;
			VREvent_t ev;
			ev.trackedDeviceIndex = controller;
			ev.eventAgeSeconds = 0.0f;
			ev.eventType = touched ? VREvent_ButtonTouch : VREvent_ButtonUntouch;
			ev.data.controller.button = k_EButton_SteamVR_Touchpad;
			g_queuedVirtualEvents.push_back(ev);
		}
	}

	if (!releaseOnly || g_pressed[controller])
	{
		if (pressed != g_pressed[controller])
		{
			g_pressed[controller] = pressed;
			VREvent_t ev;
			ev.trackedDeviceIndex = controller;
			ev.eventAgeSeconds = 0.0f;
			ev.eventType = pressed ? VREvent_ButtonPress : VREvent_ButtonUnpress;
			ev.data.controller.button = k_EButton_SteamVR_Touchpad;
			g_queuedVirtualEvents.push_back(ev);
		}
	}
}

bool DequeueEvent(VREvent_t* ev, TrackedDevicePose_t* pose)
{
	if (g_queuedVirtualEvents.empty())
		return false;

	*ev = g_queuedVirtualEvents.front();
	g_queuedVirtualEvents.pop_front();
	//what about pose!?

	return true;
}


void RemapControls(int controller, VRControllerState_t* pState)
{
	if ((pState->ulButtonPressed & ButtonMaskFromId(k_EButton_SteamVR_Touchpad)) != 0 || (pState->ulButtonTouched & ButtonMaskFromId(k_EButton_SteamVR_Touchpad)) != 0)
	{
		//do nothing - want to be able to press touchpad (but release any events we made up earlier!)
		GenerateTrackPadEvents(controller, pState, true);
	}
	else
	{
		const int JOYSTICK_AXIS = 2;
		const float DEADZONE = 0.15f;
		bool inDeadzone = fabs(pState->rAxis[JOYSTICK_AXIS].x) < DEADZONE && fabs(pState->rAxis[JOYSTICK_AXIS].y) < DEADZONE;
		if (!inDeadzone)
		{
			if (g_settings.m_touchPad || g_settings.m_pressPad)
			{
				pState->rAxis[0] = pState->rAxis[2];
			}
			if (g_settings.m_touchPad)
			{
				pState->ulButtonTouched |= ButtonMaskFromId(k_EButton_SteamVR_Touchpad);
			}
			if (g_settings.m_pressPad)
			{
				pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_SteamVR_Touchpad);
			}
			if (g_settings.m_mapToDPad)
			{
				if (pState->rAxis[JOYSTICK_AXIS].x < -DEADZONE)
					pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Left);
				else if(pState->rAxis[JOYSTICK_AXIS].x > DEADZONE)
					pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Right);
				if (pState->rAxis[JOYSTICK_AXIS].y < -DEADZONE)
					pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Down);
				else if (pState->rAxis[JOYSTICK_AXIS].y > DEADZONE)
					pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Up);
			}
		}

		GenerateTrackPadEvents(controller, pState, false);
	}
}

typedef int32_t(__thiscall *GetControllerStateFn)(IVRSystem*, TrackedDeviceIndex_t, VRControllerState_t*, uint32_t);
FunctionPatch<GetControllerStateFn> g_realGetControllerState;

class ReplaceGetControllerState : public MyVRSystem
{
	virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize)
	{
		if (g_realGetControllerState.m_symbol(this, unControllerDeviceIndex, pControllerState, unControllerStateSize))
		{
			RemapControls(unControllerDeviceIndex, pControllerState);

			return true;
		}
		return false;
	}
} g_replaceGetControllerState;

typedef int32_t(__thiscall *GetControllerStateWithPoseFn)(IVRSystem*,  ETrackingUniverseOrigin, TrackedDeviceIndex_t, VRControllerState_t*, uint32_t, TrackedDevicePose_t*);
FunctionPatch<GetControllerStateWithPoseFn> g_realGetControllerStateWithPose;

class ReplaceGetControllerStateWithPose : public MyVRSystem
{
	virtual bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize, TrackedDevicePose_t *pTrackedDevicePose)
	{
		if (g_realGetControllerStateWithPose.m_symbol(this, eOrigin, unControllerDeviceIndex, pControllerState, unControllerStateSize, pTrackedDevicePose))
		{
			RemapControls(unControllerDeviceIndex, pControllerState);

			return true;
		}
		return false;
	}
} g_replaceGetControllerStateWithPose;

typedef bool(__thiscall *PollNextEventFn)(IVRSystem*, VREvent_t*, uint32_t);
FunctionPatch<PollNextEventFn> g_realPollNextEvent;

class ReplacePollNextEvent : public MyVRSystem
{
	virtual bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent)
	{
		bool res = DequeueEvent(pEvent, nullptr) || g_realPollNextEvent.m_symbol(this, pEvent, uncbVREvent);

		return res;
	}
} g_replacePollNextEvent;

typedef bool(__thiscall *PollNextEventWithPoseFn)(IVRSystem*, ETrackingUniverseOrigin, VREvent_t*, uint32_t, vr::TrackedDevicePose_t*);
FunctionPatch<PollNextEventWithPoseFn> g_realPollNextEventWithPose;

TrackedDevicePose_t g_lastPose[10];

class ReplacePollNextEventWithPose : public MyVRSystem
{
	virtual bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin, VREvent_t *pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t *pTrackedDevicePose)
	{
		if (DequeueEvent(pEvent, pTrackedDevicePose))
		{
			if ((int)pEvent->trackedDeviceIndex >= 0)
			{
				*pTrackedDevicePose = g_lastPose[pEvent->trackedDeviceIndex];
			}
			return true;
		}

		if (g_realPollNextEventWithPose.m_symbol(this, eOrigin, pEvent, uncbVREvent, pTrackedDevicePose))
		{
			if ((int)pEvent->trackedDeviceIndex >= 0)
			{
				g_lastPose[pEvent->trackedDeviceIndex] = *pTrackedDevicePose;
			}
			return true;
		}

		return false;
	}
} g_replacePollNextEventWithPose;

IVRSystem* g_pVRSystem;

void PatchVRSystem(void* pVRSystem, const char* interfaceVersion)
{
	g_pVRSystem = (IVRSystem*)pVRSystem;

	InstallWritableVtable(g_pVRSystem);

	PatchVTable(g_pVRSystem, &g_mySystem, &g_replaceGetControllerState, g_realGetControllerState);
	PatchVTable(g_pVRSystem, &g_mySystem, &g_replaceGetControllerStateWithPose, g_realGetControllerStateWithPose);
	PatchVTable(g_pVRSystem, &g_mySystem, &g_replacePollNextEvent, g_realPollNextEvent);
	PatchVTable(g_pVRSystem, &g_mySystem, &g_replacePollNextEventWithPose, g_realPollNextEventWithPose);
}


void RemovePatchVRSystem()
{
	RemovePatchVTable(g_pVRSystem, g_realGetControllerState);
	RemovePatchVTable(g_pVRSystem, g_realGetControllerStateWithPose);
	RemovePatchVTable(g_pVRSystem, g_realPollNextEvent);
	RemovePatchVTable(g_pVRSystem, g_realPollNextEventWithPose);
}