#include "hack.h"

#include <iostream>

int main()
{
    DWORD64 print_func_offset = 0x159A;
    DWORD64 g_cnt_offset = 0xE170;

    auto env = Env("target.exe");
    env.Load();

    for (auto & instance : env.Instances()) {
        uint8_t buffer[7] = { 0 };
        instance->WriteMemory(g_cnt_offset, buffer, sizeof(buffer), sizeof(int));
        instance->ReadMemory(print_func_offset, buffer, sizeof(buffer), sizeof(buffer));
    }
    getchar();
    return 0;
}