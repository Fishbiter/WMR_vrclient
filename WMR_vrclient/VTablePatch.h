#pragma once

#include "openvr.h"

using namespace vr;

class MyVRSystem : public vr::IVRSystem
{
public:
	virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) {}
	virtual HmdMatrix44_t GetProjectionMatrix(EVREye eEye, float fNearZ, float fFarZ) { return HmdMatrix44_t(); }
	virtual void GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom) {}
	virtual bool ComputeDistortion(EVREye eEye, float fU, float fV, DistortionCoordinates_t *pDistortionCoordinates) { return false; }
	virtual HmdMatrix34_t GetEyeToHeadTransform(EVREye eEye) { return HmdMatrix34_t(); }
	virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) { return false; }
	virtual int32_t GetD3D9AdapterIndex() { return 0; }
	virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex) {}
	virtual void GetOutputDevice(uint64_t *pnDevice, ETextureType textureType, VkInstance_T *pInstance = nullptr) {}
	virtual bool IsDisplayOnDesktop() { return false; }
	virtual bool SetDisplayVisibility(bool bIsVisibleOnDesktop) { return false; }
	virtual void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) {}
	virtual void ResetSeatedZeroPose() {}
	virtual HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() { return HmdMatrix34_t(); }
	virtual HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() { return HmdMatrix34_t(); }
	virtual uint32_t GetSortedTrackedDeviceIndicesOfClass(ETrackedDeviceClass eTrackedDeviceClass, VR_ARRAY_COUNT(unTrackedDeviceIndexArrayCount) vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex = k_unTrackedDeviceIndex_Hmd) { return 0; }
	virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) { return EDeviceActivityLevel(); }
	virtual void ApplyTransform(TrackedDevicePose_t *pOutputPose, const TrackedDevicePose_t *pTrackedDevicePose, const HmdMatrix34_t *pTransform) {}
	virtual vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType) { return 0; }
	virtual vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex) { return vr::ETrackedControllerRole(); }
	virtual ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) { return ETrackedDeviceClass(); }
	virtual bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) { return false; }
	virtual bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) { return false; }
	virtual float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) { return false; }
	virtual int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) { return false; }

	virtual uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) { return 0; }
	virtual HmdMatrix34_t GetMatrix34TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError = 0L) { return HmdMatrix34_t(); }
	virtual uint32_t GetArrayTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, PropertyTypeTag_t propType, void *pBuffer, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L) { return 0; }
	virtual uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError = 0L) { return 0; }
	virtual const char *GetPropErrorNameFromEnum(ETrackedPropertyError error) { return ""; }
	virtual bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent) { return false; }
	virtual bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin, VREvent_t *pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t *pTrackedDevicePose) { return false; }
	virtual const char *GetEventTypeNameFromEnum(EVREventType eType) { return ""; }
	virtual HiddenAreaMesh_t GetHiddenAreaMesh(EVREye eEye, EHiddenAreaMeshType type = k_eHiddenAreaMesh_Standard) { return HiddenAreaMesh_t(); }
	virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize) { return false; }
	virtual bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, uint32_t unControllerStateSize, TrackedDevicePose_t *pTrackedDevicePose) { return false; }
	virtual void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) {}
	virtual const char *GetButtonIdNameFromEnum(EVRButtonId eButtonId) { return ""; }
	virtual const char *GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType) { return ""; }
	virtual bool CaptureInputFocus() { return false; }
	virtual void ReleaseInputFocus() {}
	virtual bool IsInputFocusCapturedByAnotherProcess() { return false; }
	virtual bool IsInputAvailable() { return false; }
	virtual bool IsSteamVRDrawingControllers() { return false; }
	virtual bool ShouldApplicationPause() { return false; }
	virtual bool ShouldApplicationReduceRenderingWork() { return false; }
	virtual uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest, VR_OUT_STRING() char *pchResponseBuffer, uint32_t unResponseBufferSize) { return 0; }
	virtual vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) { return vr::EVRFirmwareError(); }
	virtual void AcknowledgeQuit_Exiting() {}
	virtual void AcknowledgeQuit_UserPrompt() {}
};

template<class T> struct FunctionPatch
{
	T m_symbol;
	void* m_replaced;
	int m_entry = -1;
};

void** VTable(IVRSystem* pVRSystem);

int GetNumberEntries(void** vtable);

void InstallWritableVtable(IVRSystem* system);

template<class T> void PatchVTable(IVRSystem* pVRSystem, IVRSystem* pRefNoOverride, IVRSystem* pOverride, FunctionPatch<T>& out)
{
	void** vtable = VTable(pRefNoOverride);
	void** compvtable = VTable(pOverride);
	void** tgtVtable = VTable(pVRSystem);

	for (int i = 0; i < GetNumberEntries(vtable); i++)
	{
		if (vtable[i] != compvtable[i])
		{
			out.m_entry = i + (int)g_settings.m_patchOffset;
			out.m_symbol = (T)tgtVtable[out.m_entry];
			tgtVtable[out.m_entry] = out.m_replaced = compvtable[i];
			return;
		}
	}
}

template<class T> void RemovePatchVTable(IVRSystem* pVRSystem, const FunctionPatch<T>& out)
{
	if (out.m_entry == -1)
		return;

	void** vtable = VTable(pVRSystem);
	if (vtable[out.m_entry] == out.m_replaced)
		vtable[out.m_entry] = out.m_symbol;
}