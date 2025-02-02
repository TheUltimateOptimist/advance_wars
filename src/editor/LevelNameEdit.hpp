#pragma once

#include <QLineEdit>

namespace editor 
{

class LevelNameEdit : public QLineEdit {
public:
    LevelNameEdit(const std::string& level_name, QWidget* parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent* event) override;
};

} // namespace editor