#include "globals.hpp"
#include <functional>
#include <sstream>

void setActiveWorkspaceOnMonitor(std::string arg_string) {
	std::stringstream args(arg_string);

	WORKSPACEID wsId;
	MONITORID mId;

	args >> wsId;
	if (args.fail()) {
		throw std::invalid_argument("Could not read workspace id");
	}

	args >> mId;
	if (args.fail()) {
		throw std::invalid_argument("Could not read monitor id");
	}

	auto mHandle = g_pCompositor->getMonitorFromID(mId);
	if (!mHandle) {
		throw std::invalid_argument(std::format("No monitor with id: {}", mId));
		return;
	}

	auto wsHandle = g_pCompositor->getWorkspaceByID(wsId);
	if (!wsHandle) {
		wsHandle = g_pCompositor->createNewWorkspace(wsId, mId);
	}

	g_pCompositor->moveWorkspaceToMonitor(wsHandle, mHandle, true);
	mHandle->changeWorkspace(wsHandle, false, true, true);
}

// Do NOT change this function.
APICALL EXPORT std::string PLUGIN_API_VERSION()
{
	return HYPRLAND_API_VERSION;
}

void addDispatcher(const std::string s, const std::function<void(std::string)> f) {
	HyprlandAPI::addDispatcher(PHANDLE, std::format("bttr_{}", s), f);
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle)
{
	PHANDLE = handle;

	addDispatcher("workspacemonitor", setActiveWorkspaceOnMonitor);
	return {BTTR_NAME, BTTR_DESC, BTTR_AUTHOR, BTTR_VERSION};
}

APICALL EXPORT void PLUGIN_EXIT()
{
}
