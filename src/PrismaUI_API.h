/*
* For modders: Copy this file into your own project if you wish to use this API.
*/
#pragma once

#include <functional>
#include <queue>
#include <stdint.h>
#include <string>
#include <iostream>

typedef std::string PrismaView;

namespace PRISMA_UI_API
{
	constexpr const auto PrismaUIPluginName = "PrismaUI";

	using PluginHandle = SKSE::PluginHandle;
	using ActorHandle = RE::ActorHandle;

	enum class InterfaceVersion : uint8_t
	{
		V1
	};

	// PrismaUI modder interface v1
	class IVPrismaUI1
	{
	public:
		// Create view. Initial directory of "htmlPath" is "Skyrim Special Edition/Data/PrismaUI/views/".
		virtual PrismaView CreateView(std::string htmlPath, std::function<void(PrismaView)> onDomReadyCallback = nullptr) noexcept = 0;

		// Send JavaScript code to Web UI. Accepts callback with return value at second argument.
		virtual void Invoke(PrismaView view, std::string script, std::function<void(std::string)> callback = nullptr) noexcept = 0;

		// Register JavaScript listener for view. Receives one argument in a callback.
		virtual void RegisterJSListener(PrismaView view, std::string fnName, std::function<void(std::string)> callback) noexcept = 0;

		// Returns true if view has focus.
		virtual bool HasFocus(PrismaView view) noexcept = 0;

		// Focus view (show cursor and open FocusMenu as Skyrim Menu).
		virtual bool Focus(PrismaView view, bool pauseGame = false) noexcept = 0;

		// Unfocus view (hide cursor and disable FocusMenu).
		virtual void Unfocus(PrismaView view) noexcept = 0;

		// Get scrolling distance in pixels.
		virtual int GetScrollingPixelSize(PrismaView view) noexcept = 0;

		// Set scrolling size in pixels. DEFAULT: 28
		virtual void SetScrollingPixelSize(PrismaView view, int pixelSize) noexcept = 0;

		// Returns true if view exists.
		virtual bool IsValid(PrismaView view) noexcept = 0;

		// Totally remove view from memory and unfocus it.
		virtual void Destroy(PrismaView view) noexcept = 0;
	};

	typedef void* (*_RequestPluginAPI)(const InterfaceVersion interfaceVersion);

	/// <summary>
	/// Request the PrismaUI API interface.
	/// Recommended: Send your request during or after SKSEMessagingInterface::kMessage_PostLoad to make sure the dll has already been loaded
	/// </summary>
	/// <param name="a_interfaceVersion">The interface version to request</param>
	/// <returns>The pointer to the API singleton, or nullptr if request failed</returns>
	[[nodiscard]] inline void* RequestPluginAPI(const InterfaceVersion a_interfaceVersion = InterfaceVersion::V1)
	{
		auto pluginHandle = GetModuleHandle("PrismaUI.dll");
		_RequestPluginAPI requestAPIFunction = (_RequestPluginAPI)GetProcAddress(pluginHandle, "RequestPluginAPI");

		if (requestAPIFunction) {
			return requestAPIFunction(a_interfaceVersion);
		}

		return nullptr;
	}
}