#include "SaveButton.hpp"
#include "EventBroker.hpp"
#include <QFileDialog>

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
        this, "Level Speichern", QDir::homePath(), "HDF5 Files (*.h5);;All Files (*.*)");
    EventBroker::send([file_path](EventBroker* e) { e->onLevelWriteRequested(file_path); });
}

} // namespace editor