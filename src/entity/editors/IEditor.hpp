#pragma once

class IEditor{
public:
    virtual void show() = 0;
    virtual void update() = 0;
    virtual void dispose() = 0;
    virtual void draw() = 0;
    virtual ~IEditor(){}
};
