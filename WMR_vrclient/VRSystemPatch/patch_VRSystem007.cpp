#include "stdafx.h"
#include "patch_Include.h"

namespace IVRSystem_007
{
#include "openvr007.h"

	using namespace vr;

	class MyVRSystem : public vr::IVRSystem
	{
	public:
		virtual void GetWindowBounds(int32_t *pnX, int32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) {}
		virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) {}
		virtual void GetEyeOutputViewport(Hmd_Eye eEye, uint32_t *pnX, uint32_t *pnY, uint32_t *pnWidth, uint32_t *pnHeight) {}
		virtual HmdMatrix44_t GetProjectionMatrix(Hmd_Eye eEye, float fNearZ, float fFarZ, GraphicsAPIConvention eProjType) { return HmdMatrix44_t(); }
		virtual void GetProjectionRaw(Hmd_Eye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom) {}
		virtual DistortionCoordinates_t ComputeDistortion(Hmd_Eye eEye, float fU, float fV) { return DistortionCoordinates_t(); }
		virtual HmdMatrix34_t GetEyeToHeadTransform(Hmd_Eye eEye) { return HmdMatrix34_t(); }
		virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) { return false; }
		virtual int32_t GetD3D9AdapterIndex() { return 0; }
		virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex, int32_t *pnAdapterOutputIndex) { }
		virtual bool AttachToWindow(void *hWnd) { return false; }
		virtual void GetDeviceToAbsoluteTrackingPose(TrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) {}
		virtual void ResetSeatedZeroPose() {}
		virtual HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() { return HmdMatrix34_t(); }
		virtual HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() { return HmdMatrix34_t(); }
		virtual uint32_t GetSortedTrackedDeviceIndicesOfClass(TrackedDeviceClass eTrackedDeviceClass, VR_ARRAY_COUNT(unTrackedDeviceIndexArrayCount) vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex = k_unTrackedDeviceIndex_Hmd) { return 0; }
		virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) { return EDeviceActivityLevel(); }
		virtual void ApplyTransform(TrackedDevicePose_t *pOutputPose, const TrackedDevicePose_t & trackedDevicePose, const HmdMatrix34_t & transform) {}
		virtual TrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) { return TrackedDeviceClass(); }
		virtual bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) { return false; }
		virtual bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError *pError = 0L) { return false; }
		virtual float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError *pError = 0L) { return 0.0f; }
		virtual int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError *pError = 0L) { return 0; }
		virtual uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError *pError = 0L) { return 0; }
		virtual HmdMatrix34_t GetMatrix34TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, TrackedPropertyError *pError = 0L) { return HmdMatrix34_t(); }
		virtual uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, TrackedDeviceProperty prop, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, TrackedPropertyError *pError = 0L) { return 0; }
		virtual const char *GetPropErrorNameFromEnum(TrackedPropertyError error) { return ""; }
		virtual bool PollNextEvent(VREvent_t *pEvent) { return false;  }
		virtual bool PollNextEventWithPose(TrackingUniverseOrigin eOrigin, vr::VREvent_t *pEvent, vr::TrackedDevicePose_t *pTrackedDevicePose) { return false; }
		virtual const char *GetEventTypeNameFromEnum(EVREventType eType) { return ""; }
		virtual HiddenAreaMesh_t GetHiddenAreaMesh(Hmd_Eye eEye) { return HiddenAreaMesh_t(); }
		virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState) { return false; }
		virtual bool GetControllerStateWithPose(TrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, TrackedDevicePose_t *pTrackedDevicePose) { return false; }
		virtual void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) {}
		virtual const char *GetButtonIdNameFromEnum(EVRButtonId eButtonId) { return ""; }
		virtual const char *GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType) { return ""; }
		virtual bool CaptureInputFocus() { return false; }
		virtual void ReleaseInputFocus() {}
		virtual bool IsInputFocusCapturedByAnotherProcess() { return false; }
		virtual uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize) { return 0; }
		virtual vr::VRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) { return VRFirmwareError(); }
		virtual bool IsDisplayOnDesktop() { return false; }
		virtual bool SetDisplayVisibility(bool bIsVisibleOnDesktop) { return false; }
	};


	#define VRSYSTEM_VERSION 7

	#include "patch_Common.h"		
}