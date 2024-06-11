#include "hack.h"

#include <windows.h>
#include <Psapi.h>
#include <tlhelp32.h>

#include <codecvt>
#include <locale>
#include <iostream>
#include <vector>

struct EnumWindowParam {
    std::wstring windowName;
    std::vector<HANDLE> handles;
};

std::string wstring_to_string(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstr);
}

int getPID(const std::string& name)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (!Process32First(snapshot, &entry)) return NULL;

    do {
        if (strcmp(entry.szExeFile, name.c_str()) == 0) {
            CloseHandle(snapshot);
            return entry.th32ProcessID;
        }
    } while (Process32Next(snapshot, &entry));

    CloseHandle(snapshot);
    return NULL;
}

DWORD64 GetModule(HANDLE hProcess, const std::string& name)
{
    HMODULE hMods[1024];
    DWORD cbNeeded;

    if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded)) {
        for (int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(hProcess, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR))) {
                std::string modName = szModName;
                if (modName.find(name) != std::string::npos) {
                    return (DWORD64)hMods[i];
                }
            }
        }
    }
    return NULL;
}

static bool EnablePrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid)) {
        std::cerr << "LookupPrivilegeValue error: " << GetLastError() << std::endl;
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = bEnablePrivilege ? SE_PRIVILEGE_ENABLED : 0;
    std::cout << tp.Privileges[0].Attributes << std::endl;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
        std::cerr << "AdjustTokenPrivileges error: " << GetLastError() << std::endl;
        return false;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        std::cerr << "The token does not have the specified privilege." << std::endl;
        return false;
    }

    return true;
}

bool Env::Privilege(HANDLE *hToken)
{
    // 打开当前进程的令牌
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, hToken)) {
        std::cerr << "OpenProcessToken error: " << GetLastError() << std::endl;
        return false;
    }

    // 启用SeDebugPrivilege权限
    if (!EnablePrivilege(*hToken, SE_DEBUG_NAME, TRUE)) {
        CloseHandle(*hToken);
        std::cerr << "Enable Privilege Failed" << std::endl;
        return false;
    }

    return true;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    TCHAR windowTitle[256];
    EnumWindowParam* param = (EnumWindowParam*)(lParam);
    GetWindowText(hwnd, windowTitle, sizeof(windowTitle) / sizeof(TCHAR));
    
#ifdef _UNICODE
    std::wstring windowTitleStr = windowTitle;
#else
    std::wstring windowTitleStr = std::wstring(windowTitle, windowTitle + _tcslen(windowTitle));
#endif
    // if (windowTitleStr == param->windowName) {
    if (windowTitleStr.find(param->windowName) != std::wstring::npos) {
        std::wcout << windowTitleStr << std::endl;
        DWORD processID;
        GetWindowThreadProcessId(hwnd, &processID);
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
        if (hProcess != NULL) {
            param->handles.push_back(hProcess);
        }
    }
    return TRUE;
}

Env::Env(DWORD pid) : m_pid(pid)
{
    HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_pid);
    if (processHandle == NULL) {
        std::cerr << "OpenProcess error: " << GetLastError() << std::endl;
        return;
    }
    m_handles.push_back(processHandle);
    return;
}

Env::Env(LPCTSTR windowName) : m_windowName(std::wstring(windowName, windowName + _tcslen(windowName)))
{
    EnumWindowParam param;
    param.windowName = m_windowName;
    EnumWindows(EnumWindowsProc, (LPARAM)&param);
    m_handles = param.handles;
}

Env::Env(std::wstring windowName) : m_windowName(windowName)
{
    EnumWindowParam param;
    param.windowName = m_windowName;
    EnumWindows(EnumWindowsProc, (LPARAM)&param);
    m_handles = param.handles;
}

Env::~Env()
{
    for (auto handle : m_handles) {
        CloseHandle(handle);
    }
    CloseHandle(m_hToken);
    std::cout << "Env::~Env()" << std::endl;
}

void Env::Load()
{
    if (m_handles.size() == 0) {
        std::cerr << "not valid game instance exist" << std::endl;
        return;
    }
    if (!Privilege(&m_hToken)) {
        exit(-1);
    }
    for (auto handle : m_handles) {
        m_instances.emplace_back(std::make_shared<Instance>(handle, m_windowName));
    }
}

void Env::Reload()
{
    m_instances.clear();

    for (auto handle : m_handles) {
        CloseHandle(handle);
    }

    if (m_windowName.size() == 0) {
        m_handles.clear();
        HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_pid);
        if (processHandle == NULL) {
            std::cerr << "OpenProcess error: " << GetLastError() << std::endl;
            return;
        }
        m_handles.push_back(processHandle);
    } else {
        EnumWindowParam param;
        param.windowName = m_windowName;
        EnumWindows(EnumWindowsProc, (LPARAM)&param);
        m_handles = param.handles;
    }

    for (auto handle : m_handles) {
        m_instances.emplace_back(std::make_shared<Instance>(handle, m_windowName));
    }
}

Instance::Instance(HANDLE handle, std::wstring windowName) : m_handle(handle), m_windowName(windowName)
{
    m_baseAddr = GetModule(handle, wstring_to_string(m_windowName));
}

Instance::~Instance()
{
    CloseHandle(m_handle);
    std::cout << "Instance::~Instance()" << std::endl;
}

bool Instance::WriteMemory(uint64_t offset, uint8_t *buffer, size_t buffer_size, size_t bytesWritten)
{
    BOOL result = WriteProcessMemory(m_handle, (LPVOID)(m_baseAddr + offset), buffer, buffer_size, &bytesWritten);
    if (result == FALSE) {
        std::cerr << "WriteProcessMemory error: " << GetLastError() << std::endl;
        return false;
    }
    std::cout << "Wrote " << bytesWritten << " bytes." << std::endl;
    return true;
}

bool Instance::ReadMemory(uint64_t offset, uint8_t *buffer, size_t buffer_size,size_t readBytes)
{
    BOOL rpmResult = ReadProcessMemory(m_handle, (LPVOID)(m_baseAddr + offset), buffer, buffer_size, &readBytes);

    if (rpmResult == FALSE) {
        std::cerr << "ReadProcessMemory error: " << GetLastError() << std::endl;
        return false;
    }

    std::cout << "ReadProcessMemory success, read bytes " << readBytes << std::endl;
    for (SIZE_T i = 0; i < readBytes; ++i) {
        std::cout << std::hex << static_cast<uint16_t>(buffer[i]) << " ";
    }
    std::cout << std::endl;
    return true;
}