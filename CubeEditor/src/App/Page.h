#pragma once

class Page {
public:
    Page() = default;
    virtual ~Page() = default;

    virtual void render(float delta) = 0;
};