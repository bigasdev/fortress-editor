#pragma once 

#include "IAction.hpp"

#include <string>
#include <map>
#include "../data/EntityData.hpp"

class DeleteAction :  public IAction{
public:
  DeleteAction(std::string entity_name, std::map<std::string, EntityData>& asset_entities) : m_entity_name(entity_name), m_asset_entities(asset_entities) {}
  ~DeleteAction();

  void execute() override;
  void undo() override;
private:
  std::string m_entity_name;
  std::map<std::string, EntityData>& m_asset_entities;
};
