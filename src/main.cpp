#include "hack.h"
#include "ui.h"

#include <iostream>
#include <thread>

using namespace std;

DWORD64 print_func_offset = 0x159A;
DWORD64 g_cnt_offset = 0xE170;

void MemoryProcess(std::shared_ptr<Instance> ins)
{
    uint8_t buffer[7] = { 0 };
    ins->WriteMemory(g_cnt_offset, buffer, sizeof(buffer), sizeof(int));
    ins->ReadMemory(print_func_offset, buffer, sizeof(buffer), sizeof(buffer));
}

int main()
{
    auto env = Env("target.exe");
    env.Load();

    for (auto & instance : env.Instances()) {
        MemoryProcess(instance);
    }

    thread uiThread(ui_main);
    uiThread.detach();

    while (1) {
        getchar();
        env.Reload();
        for (auto & instance : env.Instances()) {
            MemoryProcess(instance);
        }
    }
    return 0;
}
