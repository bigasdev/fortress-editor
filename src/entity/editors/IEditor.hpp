#pragma once

class IEditor{
public:
    virtual void open() = 0;
    virtual void show() = 0;
    virtual void update() = 0;
    virtual void dispose() = 0;
    virtual void draw() = 0;
    virtual ~IEditor(){}

    bool is_open = false; 
    bool block_close = false;
};
