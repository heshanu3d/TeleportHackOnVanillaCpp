#pragma once

#include <windows.h>
#include <tchar.h>

#include <vector>
#include <string>
#include <memory>

class Instance;

class Env {
public:
    Env(DWORD pid);
    Env(LPCTSTR windowName);
    Env(std::wstring windowName);
    ~Env();
    void Load();
    std::vector<std::shared_ptr<Instance>> Instances() {
        return m_instances;
    }
private:
    DWORD m_pid;
    std::vector<HANDLE> m_handles;
    std::vector<std::shared_ptr<Instance>> m_instances;
    std::wstring m_windowName;
    HANDLE m_hToken;
};

class Instance {
public:
    Instance(HANDLE handle, std::wstring windowName);
    ~Instance();
    bool WriteMemory(uint64_t offset, uint8_t *buffer, size_t buffer_size, size_t bytesWritten);
    bool ReadMemory(uint64_t offset, uint8_t *buffer, size_t buffer_size, size_t readBytes);
private:
    HANDLE m_handle;
    DWORD64 m_baseAddr;
    std::wstring m_windowName;
};

bool Privilege();

