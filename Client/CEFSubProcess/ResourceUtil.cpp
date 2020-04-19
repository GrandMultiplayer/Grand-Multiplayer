#include "ResourceUtil.h"

#include "include/base/cef_logging.h"
#include "include/cef_stream.h"
#include "include/wrapper/cef_byte_read_handler.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include "Resource.h"

namespace
{
	bool LoadBinaryResource(int binaryId, DWORD& dwSize, LPBYTE& pBytes)
	{
		HINSTANCE hInst = GetModuleHandle(NULL);
		HRSRC hRes =
			FindResource(hInst, MAKEINTRESOURCE(binaryId), MAKEINTRESOURCE(256));
		if (hRes)
		{
			HGLOBAL hGlob = LoadResource(hInst, hRes);
			if (hGlob)
			{
				dwSize = SizeofResource(hInst, hRes);
				pBytes = (LPBYTE)LockResource(hGlob);
				if (dwSize > 0 && pBytes)
					return true;
			}
		}

		return false;
	}

	// Provider of binary resources.
	class BinaryResourceProvider : public CefResourceManager::Provider
	{
	public:
		BinaryResourceProvider(const std::string& url_path,
			const std::string& resource_path_prefix)
			: url_path_(url_path), resource_path_prefix_(resource_path_prefix)
		{
			DCHECK(!url_path.empty());
			if (!resource_path_prefix_.empty() &&
				resource_path_prefix_[resource_path_prefix_.length() - 1] != '/')
			{
				resource_path_prefix_ += "/";
			}
		}

		bool OnRequest(scoped_refptr<CefResourceManager::Request> request) OVERRIDE
		{
			CEF_REQUIRE_IO_THREAD();

			const std::string& url = request->url();
			if (url.find(url_path_) != 0L)
			{
				// Not handled by this provider.
				return false;
			}

			CefRefPtr<CefResourceHandler> handler;

			std::string relative_path = url.substr(url_path_.length());
			if (!relative_path.empty())
			{
				if (!resource_path_prefix_.empty())
					relative_path = resource_path_prefix_ + relative_path;

				CefRefPtr<CefStreamReader> stream =
					GetBinaryResourceReader(relative_path.data());
				if (stream.get())
				{
					handler = new CefStreamResourceHandler(
						request->mime_type_resolver().Run(url), stream);
				}
			}

			request->Continue(handler);
			return true;
		}

	private:
		std::string url_path_;
		std::string resource_path_prefix_;

		DISALLOW_COPY_AND_ASSIGN(BinaryResourceProvider);
	};

}

int GetResourceId(const char* resource_name)
{
	// Map of resource labels to BINARY id values.
	static struct _resource_map
	{
		const char* name;
		int id;
	} resource_map[] = {
		{"binding.html", IDS_BINDING_HTML},
		{"dialogs.html", IDS_DIALOGS_HTML},
		{"draggable.html", IDS_DRAGGABLE_HTML},
		{"drm.html", IDS_DRM_HTML},
		{"extensions/set_page_color/icon.png",
		 IDS_EXTENSIONS_SET_PAGE_COLOR_ICON_PNG},
		{"extensions/set_page_color/manifest.json",
		 IDS_EXTENSIONS_SET_PAGE_COLOR_MANIFEST_JSON},
		{"extensions/set_page_color/popup.html",
		 IDS_EXTENSIONS_SET_PAGE_COLOR_POPUP_HTML},
		{"extensions/set_page_color/popup.js",
		 IDS_EXTENSIONS_SET_PAGE_COLOR_POPUP_JS},
		{"logo.png", IDS_LOGO_PNG},
		{"localstorage.html", IDS_LOCALSTORAGE_HTML},
		{"menu_icon.1x.png", IDS_MENU_ICON_1X_PNG},
		{"menu_icon.2x.png", IDS_MENU_ICON_2X_PNG},
		{"osr_test.html", IDS_OSRTEST_HTML},
		{"other_tests.html", IDS_OTHER_TESTS_HTML},
		{"pdf.html", IDS_PDF_HTML},
		{"pdf.pdf", IDS_PDF_PDF},
		{"performance.html", IDS_PERFORMANCE_HTML},
		{"performance2.html", IDS_PERFORMANCE2_HTML},
		{"preferences.html", IDS_PREFERENCES_HTML},
		{"response_filter.html", IDS_RESPONSE_FILTER_HTML},
		{"server.html", IDS_SERVER_HTML},
		{"transparency.html", IDS_TRANSPARENCY_HTML},
		{"urlrequest.html", IDS_URLREQUEST_HTML},
		{"websocket.html", IDS_WEBSOCKET_HTML},
		{"window.html", IDS_WINDOW_HTML},
		{"window_icon.1x.png", IDS_WINDOW_ICON_1X_PNG},
		{"window_icon.2x.png", IDS_WINDOW_ICON_2X_PNG},
		{"xmlhttprequest.html", IDS_XMLHTTPREQUEST_HTML},
	};

	for (size_t i = 0; i < sizeof(resource_map) / sizeof(_resource_map); ++i)
	{
		if (!strcmp(resource_map[i].name, resource_name))
			return resource_map[i].id;
	}

	return 0;
}

bool LoadBinaryResource(const char* resource_name, std::string& resource_data)
{
	int resource_id = GetResourceId(resource_name);
	if (resource_id == 0)
		return false;

	DWORD dwSize;
	LPBYTE pBytes;

	if (LoadBinaryResource(resource_id, dwSize, pBytes))
	{
		resource_data = std::string(reinterpret_cast<char*>(pBytes), dwSize);
		return true;
	}

	NOTREACHED();  // The resource should be found.
	return false;
}

CefRefPtr<CefStreamReader> GetBinaryResourceReader(const char* resource_name)
{
	int resource_id = GetResourceId(resource_name);
	if (resource_id == 0)
		return NULL;

	DWORD dwSize;
	LPBYTE pBytes;

	if (LoadBinaryResource(resource_id, dwSize, pBytes))
	{
		return CefStreamReader::CreateForHandler(
			new CefByteReadHandler(pBytes, dwSize, NULL));
	}

	NOTREACHED();  // The resource should be found.
	return NULL;
}

CefResourceManager::Provider* CreateBinaryResourceProvider(
	const std::string& url_path,
	const std::string& resource_path_prefix)
{
	return new BinaryResourceProvider(url_path, resource_path_prefix);
}