#pragma once

class Prefab {
public:
  Prefab() = default;
  ~Prefab() = default;

  void init();
  void update();
  void draw();
  void clean();

private:
};
