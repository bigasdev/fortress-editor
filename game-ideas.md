#### 19/04 backlog
- For the mini rog editor rework i need to be able to add the components in the editor screen, this should be my plan of action: 
    - Load all the components folder 
    - Display an array with all the components name 
    - Read the attributes from the component 
    - Save in the prefab 

## Some pretty neat things 
Today i learned that i can use selectable to do some pretty nice button/interactables in dear imgui like this: 
\\     if (ImGui::Checkbox(("##Active" + component.second.name).c_str(), &component.second.is_active)) {
    }
    ImGui::SameLine(); 
    ImGui::PushID(&component); 
    if (ImGui::Selectable(component.second.name.c_str())) {
      /*auto command = "start cmd /k nvim " + g_folder_path + "/src/components/" + component.second.name + ".hpp";
      system(command.c_str());*/
    }
    ImGui::PopID();
 \\
