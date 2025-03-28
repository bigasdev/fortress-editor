#include "DBView.hpp"
#include "../../imgui/imgui_impl_opengl3.h"

DBView::DBView(){
}

void DBView::show(){
  ImGui::Begin("Db View", nullptr, 0);
  ImGui::Text("Hello World");
  ImGui::End();
}

void DBView::update(){
}

void DBView::dispose(){
}

void DBView::draw(){
}
