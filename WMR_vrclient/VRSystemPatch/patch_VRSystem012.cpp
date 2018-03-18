#include "stdafx.h"
#include "patch_Include.h"

namespace IVRSystem_012
{
#include "openvr012.h"

	using namespace vr;

	class MyVRSystem : public vr::IVRSystem
	{
	public:
		virtual void GetRecommendedRenderTargetSize(uint32_t *pnWidth, uint32_t *pnHeight) {};
		virtual HmdMatrix44_t GetProjectionMatrix(EVREye eEye, float fNearZ, float fFarZ, EGraphicsAPIConvention eProjType) { return HmdMatrix44_t(); };
		virtual void GetProjectionRaw(EVREye eEye, float *pfLeft, float *pfRight, float *pfTop, float *pfBottom) {};
		virtual DistortionCoordinates_t ComputeDistortion(EVREye eEye, float fU, float fV) { return DistortionCoordinates_t(); };
		virtual HmdMatrix34_t GetEyeToHeadTransform(EVREye eEye) { return HmdMatrix34_t(); };
		virtual bool GetTimeSinceLastVsync(float *pfSecondsSinceLastVsync, uint64_t *pulFrameCounter) { return false; };
		virtual int32_t GetD3D9AdapterIndex() { return 0; };
		virtual void GetDXGIOutputInfo(int32_t *pnAdapterIndex) {};
		virtual bool IsDisplayOnDesktop() { return false; };
		virtual bool SetDisplayVisibility(bool bIsVisibleOnDesktop) { return false; };
		virtual void GetDeviceToAbsoluteTrackingPose(ETrackingUniverseOrigin eOrigin, float fPredictedSecondsToPhotonsFromNow, VR_ARRAY_COUNT(unTrackedDevicePoseArrayCount) TrackedDevicePose_t *pTrackedDevicePoseArray, uint32_t unTrackedDevicePoseArrayCount) {};
		virtual void ResetSeatedZeroPose() {};
		virtual HmdMatrix34_t GetSeatedZeroPoseToStandingAbsoluteTrackingPose() { return HmdMatrix34_t(); };
		virtual HmdMatrix34_t GetRawZeroPoseToStandingAbsoluteTrackingPose() { return HmdMatrix34_t(); };
		virtual uint32_t GetSortedTrackedDeviceIndicesOfClass(ETrackedDeviceClass eTrackedDeviceClass, VR_ARRAY_COUNT(unTrackedDeviceIndexArrayCount) vr::TrackedDeviceIndex_t *punTrackedDeviceIndexArray, uint32_t unTrackedDeviceIndexArrayCount, vr::TrackedDeviceIndex_t unRelativeToTrackedDeviceIndex = k_unTrackedDeviceIndex_Hmd) { return 0; };
		virtual EDeviceActivityLevel GetTrackedDeviceActivityLevel(vr::TrackedDeviceIndex_t unDeviceId) { return EDeviceActivityLevel(); }
		virtual void ApplyTransform(TrackedDevicePose_t *pOutputPose, const TrackedDevicePose_t *pTrackedDevicePose, const HmdMatrix34_t *pTransform) {};
		virtual vr::TrackedDeviceIndex_t GetTrackedDeviceIndexForControllerRole(vr::ETrackedControllerRole unDeviceType) { return 0; };
		virtual vr::ETrackedControllerRole GetControllerRoleForTrackedDeviceIndex(vr::TrackedDeviceIndex_t unDeviceIndex) { return ETrackedControllerRole(); };
		virtual ETrackedDeviceClass GetTrackedDeviceClass(vr::TrackedDeviceIndex_t unDeviceIndex) { return ETrackedDeviceClass(); };
		virtual bool IsTrackedDeviceConnected(vr::TrackedDeviceIndex_t unDeviceIndex) { return false; };
		virtual bool GetBoolTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) { return false; };
		virtual float GetFloatTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) { return 0; };
		virtual int32_t GetInt32TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) { return 0; };
		virtual uint64_t GetUint64TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) { return 0; };
		virtual HmdMatrix34_t GetMatrix34TrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, ETrackedPropertyError *pError) { return HmdMatrix34_t(); };
		virtual uint32_t GetStringTrackedDeviceProperty(vr::TrackedDeviceIndex_t unDeviceIndex, ETrackedDeviceProperty prop, VR_OUT_STRING() char *pchValue, uint32_t unBufferSize, ETrackedPropertyError *pError) { return 0; };
		virtual const char *GetPropErrorNameFromEnum(ETrackedPropertyError error) { return 0; };
		virtual bool PollNextEvent(VREvent_t *pEvent, uint32_t uncbVREvent) { return false; };
		virtual bool PollNextEventWithPose(ETrackingUniverseOrigin eOrigin, vr::VREvent_t *pEvent, uint32_t uncbVREvent, vr::TrackedDevicePose_t *pTrackedDevicePose) { return false; };
		virtual const char *GetEventTypeNameFromEnum(EVREventType eType) { return 0; };
		virtual HiddenAreaMesh_t GetHiddenAreaMesh(EVREye eEye) { return HiddenAreaMesh_t(); };
		virtual bool GetControllerState(vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState) { return false; };
		virtual bool GetControllerStateWithPose(ETrackingUniverseOrigin eOrigin, vr::TrackedDeviceIndex_t unControllerDeviceIndex, vr::VRControllerState_t *pControllerState, TrackedDevicePose_t *pTrackedDevicePose) { return false; };
		virtual void TriggerHapticPulse(vr::TrackedDeviceIndex_t unControllerDeviceIndex, uint32_t unAxisId, unsigned short usDurationMicroSec) {};
		virtual const char *GetButtonIdNameFromEnum(EVRButtonId eButtonId) { return 0; };
		virtual const char *GetControllerAxisTypeNameFromEnum(EVRControllerAxisType eAxisType) { return 0; };
		virtual bool CaptureInputFocus() { return false; };
		virtual void ReleaseInputFocus() {};
		virtual bool IsInputFocusCapturedByAnotherProcess() { return false; };
		virtual uint32_t DriverDebugRequest(vr::TrackedDeviceIndex_t unDeviceIndex, const char *pchRequest, char *pchResponseBuffer, uint32_t unResponseBufferSize) { return 0; };
		virtual vr::EVRFirmwareError PerformFirmwareUpdate(vr::TrackedDeviceIndex_t unDeviceIndex) { return EVRFirmwareError(); };
		virtual void AcknowledgeQuit_Exiting() {};
		virtual void AcknowledgeQuit_UserPrompt() {};
		virtual void PerformanceTestEnableCapture(bool bEnable) {};
		virtual void PerformanceTestReportFidelityLevelChange(int nFidelityLevel) {};
	};

#define VRSYSTEM_VERSION 12

#include "patch_Common.h"		
}