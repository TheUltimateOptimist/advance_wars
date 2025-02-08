/**
* SaveButton.cpp
*
* @date 29.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
*/

#include "SaveButton.hpp"

#include <QFileDialog>

#include "EventHandler.hpp"

namespace editor
{

SaveButton::SaveButton(const std::string& title, QWidget* parent)
    : QPushButton(title.c_str(), parent)
{
}

void SaveButton::mousePressEvent(QMouseEvent* event)
{
    QPushButton::mousePressEvent(event);
    QString file_path = QFileDialog::getSaveFileName(
        this, "Level Speichern", QDir::currentPath(), "HDF5 Files (*.h5)");
    if (!file_path.isEmpty())
    {
        EventHandler::send([file_path](EventHandler* e) { e->onLevelWriteRequested(file_path); });
    }
}

} // namespace editor