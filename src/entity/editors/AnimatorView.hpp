#pragma once 

#include "IEditor.hpp"


class AnimatorView : public IEditor
{
public:
    AnimatorView();
    void show() override;
    void update() override;
    void dispose() override;
    void draw() override;

    void assets_child();

private:
};
