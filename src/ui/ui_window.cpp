#include "imgui.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <functional>
#include <map>

#include "ui.h"
#include "ui_on_msg.h"
#include "teleport_list_parser.h"

using namespace std;

string g_select_node_txt;
char g_teleport_name[256];
shared_ptr<Category> g_select_category = nullptr;
bool g_sync = false;
bool g_reload = false;
vector<shared_ptr<Category>> g_teleportPointLists;

enum ControlType {
    CTRL_BUTTON,
    CTRL_CHECK_BOX,
    CTRL_INPUT_TEXT,
    CTRL_LAYOUT_SAMELINE,
};

struct ButtonInitParam {
    ButtonInitParam(string txt_, function<void()> action_, ImVec2 size_) : txt(txt_), action(action_), size(size_) {}
    string txt;
    function<void()> action;
    ImVec2 size;
};

struct CheckBoxInitParam {
    CheckBoxInitParam(string txt_, bool *var_) : txt(txt_), var(var_) {}
    string txt;
    bool *var;
};

struct InputTextInitParam {
    InputTextInitParam(string txt_, char *buf_, size_t buf_size_, ImGuiInputTextFlags flags_) : txt(txt_), buf(buf_), buf_size(buf_size_), flags(flags_) {}
    string txt;
    char *buf;
    size_t buf_size;
    ImGuiInputTextFlags flags;
};

void add_select_node(std::string txt, shared_ptr<Category> category, std::function<void(string)> doubleClickCallback = [](string s){std::cout << "double click " << s << std::endl;},
    std::function<void(string)> singleClickCallback = [](string s){std::cout << "single click " << s << std::endl;})
{
    if (ImGui::Selectable(txt.c_str(), g_select_node_txt == txt, ImGuiSelectableFlags_AllowDoubleClick)) {
        if (ImGui::IsMouseDoubleClicked(0))
            doubleClickCallback(txt);
        else {
            singleClickCallback(txt);
            if (category->point)
                strcpy(g_teleport_name, category->point->ToStringWithoutXYZ().c_str());
            g_select_node_txt = txt;
            g_select_category = category;
        }
    }
}

void add_button(std::string txt, function<void()> action, const ImVec2& size = ImVec2(0, 0))
{
    if (ImGui::Button(txt.c_str(), size) && action)
        action();
}

void add_tree_node(shared_ptr<Category> category)
{
    if (category->point && category->depth > 1)
        add_select_node(category->point->ToString().c_str(), category);
}

void show_teleport_list_by_rootCategory(shared_ptr<Category> category)
{
    string list_name = "Teleport List --------" + category->name + "--------";
    if (ImGui::TreeNode(list_name.c_str())) {
        if (ImGui::TreeNode("More")) {
            for (auto &[k, v] : category->categories) {
                if (v->depth == 1 && v->point)
                    add_select_node(v->point->ToString().c_str(), v);
            }
            ImGui::TreePop();
        }        
        for (auto &[k, v] : category->categories) {
            if (ImGui::TreeNode(v->name.c_str())) {
                v->Traverse(add_tree_node);
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}

vector<pair<ControlType, void*>> rightChildCtrlInitParam = {
    make_pair(CTRL_INPUT_TEXT, new InputTextInitParam("", g_teleport_name, IM_ARRAYSIZE(g_teleport_name), ImGuiInputTextFlags_EscapeClearsAll)),
    make_pair(CTRL_BUTTON, new ButtonInitParam("Add", ButtonAction_Add, ImVec2(50, 20))),
    make_pair(CTRL_LAYOUT_SAMELINE, nullptr),
    make_pair(CTRL_BUTTON, new ButtonInitParam("Edit", ButtonAction_Edit, ImVec2(50, 20))),
    make_pair(CTRL_LAYOUT_SAMELINE, nullptr),
    make_pair(CTRL_BUTTON, new ButtonInitParam("Delete", ButtonAction_Delete, ImVec2(50, 20))),
    make_pair(CTRL_LAYOUT_SAMELINE, nullptr),
    make_pair(CTRL_CHECK_BOX, new CheckBoxInitParam("Sync", &g_sync)),
    make_pair(CTRL_BUTTON, new ButtonInitParam("Teleport", ButtonAction_Teleport, ImVec2(120, 50))),
    make_pair(CTRL_BUTTON, new ButtonInitParam("Save", ButtonAction_Save, ImVec2(50, 20))),
    make_pair(CTRL_BUTTON, new ButtonInitParam("Reload", ButtonAction_Reload, ImVec2(50, 20))),
};

void CreateCtrlByParams(vector<pair<ControlType, void*>> &initParams)
{
    static map<ControlType, function<void(void*)> > CtrlCreateMethod = {
        make_pair(CTRL_BUTTON, [](void *baseParam){
            auto ctrlParam = static_cast<ButtonInitParam*>(baseParam);
            add_button(ctrlParam->txt, ctrlParam->action, ctrlParam->size); 
        }),
        make_pair(CTRL_LAYOUT_SAMELINE, [](void *baseParam){
            ImGui::SameLine();
        }),
        make_pair(CTRL_CHECK_BOX, [](void *baseParam){
            auto ctrlParam = static_cast<CheckBoxInitParam*>(baseParam);
            ImGui::Checkbox(ctrlParam->txt.c_str(), ctrlParam->var); 
        }),
        make_pair(CTRL_INPUT_TEXT, [](void *baseParam){
            auto ctrlParam = static_cast<InputTextInitParam*>(baseParam);
            ImGui::InputText(ctrlParam->txt.c_str(), ctrlParam->buf, ctrlParam->buf_size, ctrlParam->flags);
        }),
    };
    for (auto &param : initParams) {
        CtrlCreateMethod[param.first](param.second);
    }
}

void TeleportWindow(bool &show)
{
    static bool firstRun = true;
    static float f = 0.0f;

    if (firstRun) {
        firstRun = false;
        g_reload = true;
    }
    if (g_reload) {
        g_reload = false;
        auto parser = new TeleportListParser();
        parser->Init();
        g_teleportPointLists = parser->Lists();
    }

    // ImGui::SetNextWindowSize(ImVec2(800, 600));
    ImGui::Begin("Another Window", &show);

    // ImGui::SetNextItemOpen(true);

    if (ImGui::BeginTabBar("MyTabBar")) {
        if (ImGui::BeginTabItem("Teleport")) {
            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.6f, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border);
                for (auto category : g_teleportPointLists) {
                    show_teleport_list_by_rootCategory(category);
                }
            ImGui::EndChild();
            ImGui::PopStyleVar();

            ImGui::SameLine();

            ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
            ImGui::BeginChild("ChildR", ImVec2(0, ImGui::GetContentRegionAvail().y), ImGuiChildFlags_Border);
            CreateCtrlByParams(rightChildCtrlInitParam);
            ImGui::EndChild();
            ImGui::PopStyleVar();

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    ImGui::End();
}