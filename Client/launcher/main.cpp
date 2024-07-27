#include <iostream>

#include <Shared/sdk/version.hpp>
#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/UTF8.hpp>
#include <Shared/sdk/SharedUtils/Path.hpp>

namespace Shared = NightMTA::Shared;
namespace Utils = NightMTA::SharedUtil;
namespace Windows = Utils::Windows;

using SString = Utils::SString;

int WINAPI WinMain (
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	if(!Windows::WindowsVersion::IsWin10OrGreater()) {
		Windows::WinMessageBox("Unsupported Windows version.", MB_ICONERROR);
		return 1;
	}

	Windows::SetAppID(Utils::UTF8::FromUTF8(Shared::Version::MTA_NAME));
	
	const auto previousErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

	SString loaderFilename = "loader" MTA_LIB_SUFFIX ".dll";

	SString mtaPath = Utils::Path::PathJoin(Utils::Path::GetSystemCurrentDirectory(), "mta");
	SString loaderPath = Utils::Path::PathJoin(mtaPath, loaderFilename);

	std::cout << mtaPath << std::endl;
	std::cout << loaderPath << std::endl;

	SetErrorMode(previousErrorMode);
	return 0;
}

int main() {
    WinMain(0,0,0,0);
}