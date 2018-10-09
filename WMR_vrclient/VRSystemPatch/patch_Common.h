//Note this is included into patch files directly

MyVRSystem g_mySystem;

bool g_touched[5];
bool g_pressed[5];
std::deque<VREvent_t> g_queuedVirtualEvents;
bool g_simulateLeftClick = false;

const int LEFT_CONTROLLER = 1;
const int RIGHT_CONTROLLER = 2;

static VRControllerState_t g_swapStates[3];

void GenerateTrackPadEvents(int controller, VRControllerState_t* pState, bool releaseOnly)
{
	if (!g_settings.m_generateEvents)
		return;

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

	if (g_queuedVirtualEvents.size() > 100)
	{
		g_queuedVirtualEvents.clear(); //probably app is not calling poll event
	}
}

void GenerateTouchedEvent(int controller)
{
	if (!g_settings.m_generateEvents)
		return;

	VREvent_t ev;
	ev.trackedDeviceIndex = controller;
	ev.eventAgeSeconds = 0.0f;
	ev.eventType = VREvent_ButtonTouch;
	ev.data.controller.button = k_EButton_SteamVR_Touchpad;
	g_queuedVirtualEvents.push_back(ev);
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
	const int JOYSTICK_AXIS = 2;

	VRControllerState_t* pSrcState = pState;
	if (controller == LEFT_CONTROLLER || controller == RIGHT_CONTROLLER)
	{
		g_swapStates[controller] = *pState;
	}
	if (g_settings.m_southpaw)
	{
		bool inDeadzone = fabs(pSrcState->rAxis[JOYSTICK_AXIS].x) < g_settings.m_deadzone && fabs(pSrcState->rAxis[JOYSTICK_AXIS].y) < g_settings.m_deadzone;
		
		if (controller == LEFT_CONTROLLER)
		{
			pSrcState = &g_swapStates[RIGHT_CONTROLLER];
			if (!inDeadzone)
				GenerateTouchedEvent(RIGHT_CONTROLLER);
		}
		else
		{
			pSrcState = &g_swapStates[LEFT_CONTROLLER];
			if (!inDeadzone)
				GenerateTouchedEvent(LEFT_CONTROLLER);
		}
	}
	
	//request: filter joystick moves in deadzone - this should be harmless in the normal case?
	if (fabs(pSrcState->rAxis[JOYSTICK_AXIS].x) < g_settings.m_deadzone)
	{
		pSrcState->rAxis[JOYSTICK_AXIS].x = 0;
	}
	if (fabs(pSrcState->rAxis[JOYSTICK_AXIS].y) < g_settings.m_deadzone)
	{
		pSrcState->rAxis[JOYSTICK_AXIS].y = 0;
	}

	if ((pState->ulButtonPressed & ButtonMaskFromId(k_EButton_SteamVR_Touchpad)) != 0 || (pState->ulButtonTouched & ButtonMaskFromId(k_EButton_SteamVR_Touchpad)) != 0)
	{
		//do nothing - want to be able to press touchpad (but release any events we made up earlier!)
		GenerateTrackPadEvents(controller, pState, true);
	}
	else
	{
		if (controller == RIGHT_CONTROLLER)
		{
			g_simulateLeftClick = false;
		}
		
		bool inDeadzone = fabs(pSrcState->rAxis[JOYSTICK_AXIS].x) < g_settings.m_deadzone && fabs(pSrcState->rAxis[JOYSTICK_AXIS].y) < g_settings.m_deadzone;
		if (!inDeadzone)
		{
			if (controller == RIGHT_CONTROLLER && g_settings.m_mapRightUpDownToLeftPadClick && fabs(pSrcState->rAxis[JOYSTICK_AXIS].y) >= g_settings.m_deadzone)
			{
				g_simulateLeftClick = true;
				pSrcState->rAxis[JOYSTICK_AXIS].y = 0;
			}
			
			if (g_settings.m_touchPad || g_settings.m_pressPad)
			{
				pState->rAxis[0] = pSrcState->rAxis[JOYSTICK_AXIS];

				if (g_settings.m_flipXY)
				{
					pState->rAxis[0].x *= -1.0f;
					pState->rAxis[0].y *= -1.0f;
				}
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
				if (pSrcState->rAxis[JOYSTICK_AXIS].x < -g_settings.m_deadzone)
					pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Left);
				else if (pSrcState->rAxis[JOYSTICK_AXIS].x > g_settings.m_deadzone)
					pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Right);
				if (pSrcState->rAxis[JOYSTICK_AXIS].y < -g_settings.m_deadzone)
					pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Down);
				else if (pSrcState->rAxis[JOYSTICK_AXIS].y > g_settings.m_deadzone)
					pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_DPad_Up);
			}
		}

		if (controller == LEFT_CONTROLLER && g_simulateLeftClick)
		{
			pState->ulButtonPressed |= ButtonMaskFromId(k_EButton_SteamVR_Touchpad);
		}

		GenerateTrackPadEvents(controller, pState, false);
	}
}

void TraceEvent(VREvent_t* pEvent)
{
	if (!g_settings.m_traceEvents)
		return;

	g_log << "-------------------------------\nEvent:\nType:" << pEvent->eventType << "\nDeviceIndex:" << pEvent->trackedDeviceIndex << "\nData:" << pEvent->data.controller.button << "\n-------------------------------\n";
}

#if VRSYSTEM_VERSION < 14
typedef int32_t(__thiscall *GetControllerStateFn)(IVRSystem*, TrackedDeviceIndex_t, VRControllerState_t*);
FunctionPatch<GetControllerStateFn> g_realGetControllerState;

class ReplaceGetControllerState : public MyVRSystem
{
	virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState)
	{
		if (g_realGetControllerState.m_symbol(this, unControllerDeviceIndex, pControllerState))
		{
			RemapControls(unControllerDeviceIndex, pControllerState);

			return true;
		}
		return false;
	}
} g_replaceGetControllerState;

typedef int32_t(__thiscall *GetControllerStateWithPoseFn)(IVRSystem*, TrackingUniverseOrigin, TrackedDeviceIndex_t, VRControllerState_t*, TrackedDevicePose_t*);
FunctionPatch<GetControllerStateWithPoseFn> g_realGetControllerStateWithPose;

class ReplaceGetControllerStateWithPose : public MyVRSystem
{
	virtual bool GetControllerStateWithPose(TrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, TrackedDevicePose_t *pTrackedDevicePose)
	{
		if (g_realGetControllerStateWithPose.m_symbol(this, eOrigin, unControllerDeviceIndex, pControllerState, pTrackedDevicePose))
		{
			RemapControls(unControllerDeviceIndex, pControllerState);

			return true;
		}
		return false;
	}
} g_replaceGetControllerStateWithPose;
#else
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

typedef int32_t(__thiscall *GetControllerStateWithPoseFn)(IVRSystem*, ETrackingUniverseOrigin, TrackedDeviceIndex_t, VRControllerState_t*, uint32_t, TrackedDevicePose_t*);
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
#endif

#if VRSYSTEM_VERSION < 11
typedef bool(__thiscall *PollNextEventFn)(IVRSystem*, VREvent_t*);
FunctionPatch<PollNextEventFn> g_realPollNextEvent;

class ReplacePollNextEvent : public MyVRSystem
{
	virtual bool PollNextEvent(VREvent_t *pEvent)
	{
		if (DequeueEvent(pEvent, nullptr))
		{
			return true;
		}
		
		if (g_realPollNextEvent.m_symbol(this, pEvent))
		{
			TraceEvent(pEvent);
			return true;
		}

		return false;
	}
} g_replacePollNextEvent;

typedef bool(__thiscall *PollNextEventWithPoseFn)(IVRSystem*, TrackingUniverseOrigin, VREvent_t*, vr::TrackedDevicePose_t*);
FunctionPatch<PollNextEventWithPoseFn> g_realPollNextEventWithPose;

TrackedDevicePose_t g_lastPose[k_unMaxTrackedDeviceCount];

class ReplacePollNextEventWithPose : public MyVRSystem
{
	virtual bool PollNextEventWithPose(TrackingUniverseOrigin eOrigin, VREvent_t *pEvent, vr::TrackedDevicePose_t *pTrackedDevicePose)
	{
		if (DequeueEvent(pEvent, pTrackedDevicePose))
		{
			if ((int)pEvent->trackedDeviceIndex >= 0)
			{
				*pTrackedDevicePose = g_lastPose[pEvent->trackedDeviceIndex];
			}
			return true;
		}

		if (g_realPollNextEventWithPose.m_symbol(this, eOrigin, pEvent, pTrackedDevicePose))
		{
			TraceEvent(pEvent);
			if ((int)pEvent->trackedDeviceIndex >= 0)
			{
				g_lastPose[pEvent->trackedDeviceIndex] = *pTrackedDevicePose;
			}
			return true;
		}

		return false;
	}
} g_replacePollNextEventWithPose;
#else
typedef bool(__thiscall *PollNextEventFn)(IVRSystem*, VREvent_t*, uint32_t);
FunctionPatch<PollNextEventFn> g_realPollNextEvent;

class ReplacePollNextEvent : public MyVRSystem
{
	virtual bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent)
	{
		if (DequeueEvent(pEvent, nullptr))
		{
			return true;
		}

		if (g_realPollNextEvent.m_symbol(this, pEvent, uncbVREvent))
		{
			TraceEvent(pEvent);
			return true;
		}

		return false;
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
			TraceEvent(pEvent);
			if ((int)pEvent->trackedDeviceIndex >= 0)
			{
				g_lastPose[pEvent->trackedDeviceIndex] = *pTrackedDevicePose;
			}
			return true;
		}

		return false;
	}
} g_replacePollNextEventWithPose;
#endif


IVRSystem* g_pVRSystem;

void PatchVRSystem(void* pVRSystem, const char* interfaceVersion)
{
	if (std::string(interfaceVersion) != vr::IVRSystem_Version || pVRSystem == g_pVRSystem)
		return;

	g_pVRSystem = (IVRSystem*)pVRSystem;

	InstallWritableVtable(g_pVRSystem);

	PatchVTable(g_pVRSystem, &g_mySystem, &g_replaceGetControllerState, g_realGetControllerState);
	PatchVTable(g_pVRSystem, &g_mySystem, &g_replaceGetControllerStateWithPose, g_realGetControllerStateWithPose);
	PatchVTable(g_pVRSystem, &g_mySystem, &g_replacePollNextEvent, g_realPollNextEvent);
	PatchVTable(g_pVRSystem, &g_mySystem, &g_replacePollNextEventWithPose, g_realPollNextEventWithPose);
}


void RemovePatchVRSystem()
{
	if (!g_pVRSystem)
		return;

	RemovePatchVTable(g_pVRSystem, g_realGetControllerState);
	RemovePatchVTable(g_pVRSystem, g_realGetControllerStateWithPose);
	RemovePatchVTable(g_pVRSystem, g_realPollNextEvent);
	RemovePatchVTable(g_pVRSystem, g_realPollNextEventWithPose);
}