#include "hack.h"
#include "ui.h"

#include <memory>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

bool g_env_reload = false;
extern bool g_reload;
extern std::mutex mtx;
extern std::condition_variable cv;
unique_ptr<Env> env;

#ifdef DEBUG
DWORD64 print_func_offset = 0x159A;
DWORD64 g_cnt_offset = 0xE170;
#else
DWORD64 print_func_offset = 0x159A;
DWORD64 g_cnt_offset = 0x4040;
#endif
// 36 2 0 0 8b c3 eb
void MemoryProcess(std::shared_ptr<Instance> ins)
{
    uint8_t buffer[7] = { 0 };
    ins->WriteMemory(g_cnt_offset, buffer, sizeof(buffer), sizeof(int));
    ins->ReadMemory(print_func_offset, buffer, sizeof(buffer), sizeof(buffer));
}

void ReloadFunc()
{
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        while (!g_env_reload) {
            cv.wait(lock);
        }
        cout << "reloading" << endl;

        env->Reload();
        for (auto & instance : env->Instances()) {
            MemoryProcess(instance);
        }

        g_reload = true;
        g_env_reload = false;
        Sleep(1);
    }
}

void ConsoleFunc()
{
    while (true) {
        string line;
        getline(cin, line);
        if (line == "reload") {
            std::unique_lock<std::mutex> lock(mtx);
            g_env_reload = true;
            cv.notify_all();
        }
    }
}

int main()
{
    env = make_unique<Env>("target.exe");
    env->Load();

    for (auto & instance : env->Instances()) {
        MemoryProcess(instance);
    }

    thread uiThread(ui_main);
    uiThread.detach();
    thread reloadThread(ReloadFunc);
    reloadThread.detach();
    thread consoleThread(ConsoleFunc);
    consoleThread.join();

    return 0;
}
