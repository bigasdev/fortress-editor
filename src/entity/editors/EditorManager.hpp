#pragma once
#include "IEditor.hpp"
#include <memory>
#include <vector>

class EditorManager {
public:
    EditorManager() = default;
    ~EditorManager() = default;

    template<typename T>
    std::shared_ptr<T> get_editor() {
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
                m_selected_editor = specific_editor;
                return;
            }
        }
    }
    template<typename T>
    void close_editor() {
        for (const auto& editor : m_editors) {
            if (auto specific_editor = std::dynamic_pointer_cast<T>(editor)) {
                specific_editor->is_open = false;
                if (specific_editor == m_selected_editor) {
                    m_selected_editor = nullptr;
                }
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
                m_selected_editor = specific_editor;
            } else {
                if (!editor->block_close) {
                    editor->is_open = false;
                }
            }
        }

    }
    std::vector<std::shared_ptr<IEditor>> get_editors() {
        return m_editors;
    }
    std::shared_ptr<IEditor> get_selected_editor() {
        return m_selected_editor;
    }
    void close_all() {
        for (const auto& editor : m_editors) {
            editor->is_open = false;
            m_selected_editor = nullptr;
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
    std::shared_ptr<IEditor> m_selected_editor = nullptr;
};
