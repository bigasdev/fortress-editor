#include "DeleteAction.hpp"
#include "../../tools/boxer.h"

DeleteAction::~DeleteAction() {}

void DeleteAction::execute() {
  for(auto it = m_asset_entities.begin(); it != m_asset_entities.end(); ++it) {
    if((it->first) == m_entity_name) {
      auto selection = boxer::show(("Are you sure you want to delete : " + m_entity_name + "? This action is irreversible").c_str(), "Delete", boxer::Style::Warning, boxer::Buttons::YesNo);
      if(selection == boxer::Selection::Yes) {
        m_asset_entities.erase(it);
      }
      break;
    }
  }
}

void DeleteAction::undo() {
}
