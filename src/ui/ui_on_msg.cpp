#include "ui.h"
#include "ui_on_msg.h"
#include "teleport_list_parser.h"

#include <condition_variable>
#include <thread>
#include <mutex>

using namespace std;

extern string g_select_node_txt;
extern char g_teleport_name[256];
extern shared_ptr<Category> g_select_category;
extern bool g_sync;
extern bool g_reload;
extern bool g_env_reload;
extern vector<shared_ptr<Category>> g_teleportPointLists;

std::mutex mtx;
std::condition_variable cv;

void ButtonAction_Add()
{

}

void ButtonAction_Edit()
{

}

void ButtonAction_Delete()
{

}

void ButtonAction_Save()
{

}

void ButtonAction_Reload()
{
    std::unique_lock<std::mutex> lock(mtx);
    g_env_reload = true;
    // g_reload = true; // set true in ReloadThread
    cv.notify_all();
}

void ButtonAction_Teleport()
{
    if (g_select_category) {
        g_select_category->Print();
        if (g_select_category->point)
            strcpy(g_teleport_name, g_select_category->point->ToStringWithoutXYZ().c_str());
    }
}
