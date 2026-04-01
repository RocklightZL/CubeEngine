#pragma once

class EditorPage;

class View {
public:
    View(EditorPage& editorPage) : editorPage(editorPage){}
    virtual ~View() = default;

    virtual void render(float deltaTime) = 0;

protected:
    EditorPage& editorPage;
};