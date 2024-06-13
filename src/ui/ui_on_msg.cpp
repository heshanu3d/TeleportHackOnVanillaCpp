#include "ui.h"
#include "ui_on_msg.h"

using namespace std;

extern string g_select_node_txt;
extern char g_teleport_name[256];
extern shared_ptr<Category> g_select_category;
extern bool g_sync;

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

void ButtonAction_Teleport()
{
    if (g_select_category) {
        g_select_category->Print();
        if (g_select_category->point)
            strcpy(g_teleport_name, g_select_category->point->ToStringWithoutXYZ().c_str());
    }
}
