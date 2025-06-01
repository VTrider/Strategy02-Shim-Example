// dllmain.cpp : Defines the entry point for the DLL application.

#include <ScriptUtils.h>
#include <Windows.h>

#include <filesystem>
#include <iostream>

FARPROC originalExportAddr{};

void ResolveExports()
{
    std::filesystem::path originalDLL = std::filesystem::current_path().parent_path().parent_path().append("workshop\\content\\624970\\1325933293\\VSR\\Missions\\Strategy02_original.dll");
    HMODULE strategy02 = LoadLibrary(originalDLL.c_str());;
    if (!strategy02)
    {
        MessageBox(0, L"Failed to load original DLL", L"feuker", MB_OK | MB_SYSTEMMODAL);
    }
    originalExportAddr = GetProcAddress(strategy02, "GetMisnAPI");
    if (!originalExportAddr)
    {
        MessageBox(0, L"Failed to find original export", L"feuker", MB_OK | MB_SYSTEMMODAL);
    }
}

extern "C" __declspec(dllexport) __declspec(naked) void GetMisnAPI()
{
    __asm
    {
        jmp [originalExportAddr]
    }
}

// Now you can run whatever code you want, stat tracking, discord RPC etc.
void Payload()
{
    MessageBox(0, L"Hello from shim", L"Hello", MB_OK | MB_APPLMODAL);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        ResolveExports();
        Payload();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

