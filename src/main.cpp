#include "globals.hpp"
#include <exception>
#include <expected>
#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <sstream>

#include <hyprland/src/helpers/Monitor.hpp>
#include <unordered_map>

typedef std::expected<std::optional<std::string>, std::exception> DispatchRet;

template <typename T>
using Err = std::unexpected<T>;

std::unordered_map<MONITORID, WORKSPACEID> last_workspace;

DispatchRet setActiveWorkspaceOnMonitor(std::string arg_string) {
	std::stringstream args(arg_string);

	WORKSPACEID wsId;
	MONITORID mId;

	args >> wsId;
	if (args.fail()) {
		return Err(std::invalid_argument("Could not read workspace id"));
	}

	args >> mId;
	if (args.fail()) {
		return Err(std::invalid_argument("Could not read monitor id"));
	}

	auto mHandle = g_pCompositor->getMonitorFromID(mId);
	if (!mHandle) {
		return Err(std::invalid_argument(std::format("No monitor with id: {}", mId)));
	}

	auto wsHandle = g_pCompositor->getWorkspaceByID(wsId);
	if (!wsHandle) {
		wsHandle = g_pCompositor->createNewWorkspace(wsId, mId);
	}

	auto last = mHandle->activeWorkspaceID();

	g_pCompositor->moveWorkspaceToMonitor(wsHandle, mHandle, true);
	mHandle->changeWorkspace(wsHandle, false, true, true);

	last_workspace.insert_or_assign(mId, last);

	return std::nullopt;
}

DispatchRet getPrevWorkspaceOfMonitor(std::string arg_string) {
	std::stringstream args(arg_string);

	MONITORID mId;

	args >> mId;
	if (args.fail()) {
		return Err(std::invalid_argument("Could not read monitor id"));
	}

	auto mHandle = g_pCompositor->getMonitorFromID(mId);
	if (!mHandle) {
		return Err(std::invalid_argument(std::format("No monitor with id: {}", mId)));
	}

	WORKSPACEID wsId = -1;

	if (last_workspace.contains(mId)) {
		wsId = last_workspace.at(mId);
	}

	return std::format("{}", wsId);
}

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION()
{
	return HYPRLAND_API_VERSION;
}

void addDispatcher(const std::string s, const std::function<DispatchRet(std::string)> f) {
	HyprlandAPI::addDispatcherV2(PHANDLE, std::format("bttr:{}", s), [f](std::string s) {
		auto result = f(s);
		if (result.has_value()) {
			auto to_print = *result;
			if (to_print.has_value()) {
				return SDispatchResult { .success = false, .error = to_print.value() };
			} else {
				return SDispatchResult { .success = true };
			}
		} else {
			auto e = result.error();
			return SDispatchResult { .success = false, .error = e.what() };
		}
	});
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle)
{
	PHANDLE = handle;

	addDispatcher("workspacemonitor", setActiveWorkspaceOnMonitor);
	addDispatcher("prevworkspace", getPrevWorkspaceOfMonitor);
	return {BTTR_NAME, BTTR_DESC, BTTR_AUTHOR, BTTR_VERSION};
}

APICALL EXPORT void PLUGIN_EXIT()
{
}
