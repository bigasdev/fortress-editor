#pragma once
#include "IEditor.hpp"
#include <memory>
#include <vector>

class EditorManager {
public:
    EditorManager() = default;
    ~EditorManager() = default;

    template<typename T>
    T get_editor() {
        for (const auto& editor : m_editors) {
            if (auto specific_editor = std::dynamic_pointer_cast<T>(editor)) {
                return specific_editor;
            }
        }
        return nullptr;
    }
    template<typename T>
    void open_editor() {
        for (const auto& editor : m_editors) {
            if (auto specific_editor = std::dynamic_pointer_cast<T>(editor)) {
                specific_editor->is_open = true;
                specific_editor->open();
                return;
            }
        }
    }
    template<typename T>
    void close_editor() {
        for (const auto& editor : m_editors) {
            if (auto specific_editor = std::dynamic_pointer_cast<T>(editor)) {
                specific_editor->is_open = false;
                return;
            }
        }
    }
    template<typename T>
    void open_and_close_all() {
        for (const auto& editor : m_editors) {
            if (auto specific_editor = std::dynamic_pointer_cast<T>(editor)) {
                specific_editor->is_open = true;
                specific_editor->open();
            } else {
                if (!editor->block_close) {
                    editor->is_open = false;
                }
            }
        }
    }
    void close_all() {
        for (const auto& editor : m_editors) {
            editor->is_open = false;
        }
    }

    void add_editor(std::shared_ptr<IEditor> editor) {
        m_editors.push_back(editor);
    }

    void show() {
        for (const auto& editor : m_editors) {
            if (editor->is_open) {
                editor->show();
            }
        }
    }
    void update() {
        for (const auto& editor : m_editors) {
            if (editor->is_open) {
                editor->update();
            }
        }
    }
    void dispose() {
        for (const auto& editor : m_editors) {
            if (editor->is_open) {
                editor->dispose();
            }
        }
    }
    void draw() {
        for (const auto& editor : m_editors) {
            if (editor->is_open) {
                editor->draw();
            }
        }
    }


private:
    std::vector<std::shared_ptr<IEditor>> m_editors;
};
