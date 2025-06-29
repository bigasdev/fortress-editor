#pragma once

class Prefab {
public:
  Prefab() = default;
  ~Prefab() = default;

  void init();
  void update();
  void side_draw();
  void draw();
  void clean();

private:
};
