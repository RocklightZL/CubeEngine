#pragma once

class Page {
public:
    enum class Type {
        Guidance,
        Editor
    };

    Page() = default;
    virtual ~Page() = default;

    virtual void render(float delta) = 0;
    virtual Type getType() const = 0;
};