#ifndef __CLIENTSWITCHES_H__
#define __CLIENTSWITCHES_H__

// Defines all of the command line switches used by cefclient.
namespace switches
{
	extern const char kMultiThreadedMessageLoop[];
	extern const char kExternalMessagePump[];
	extern const char kCachePath[];
	extern const char kUrl[];
	extern const char kOffScreenRenderingEnabled[];
	extern const char kOffScreenFrameRate[];
	extern const char kTransparentPaintingEnabled[];
	extern const char kShowUpdateRect[];
	extern const char kMouseCursorChangeDisabled[];
	extern const char kRequestContextPerBrowser[];
	extern const char kRequestContextSharedCache[];
	extern const char kRequestContextBlockCookies[];
	extern const char kBackgroundColor[];
	extern const char kEnableGPU[];
	extern const char kFilterURL[];
	extern const char kUseViews[];
	extern const char kHideFrame[];
	extern const char kHideControls[];
	extern const char kHideTopMenu[];
	extern const char kWidevineCdmPath[];
	extern const char kSslClientCertificate[];
	extern const char kCRLSetsPath[];
	extern const char kLoadExtension[];
}

#endif