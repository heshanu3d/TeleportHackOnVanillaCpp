#include "hack.h"
#include "ui.h"

#include <iostream>
#include <thread>

using namespace std;

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
        string line;
        getline(cin, line);
        if (line == "print") {
            cout << line << endl;
        } else {
            env.Reload();
            for (auto & instance : env.Instances()) {
                MemoryProcess(instance);
            }
        }
    }
    return 0;
}
