#include <QLineEdit>
#include "EventBroker.hpp"

class LevelNameEdit : public QLineEdit, public EventBroker {
public:
    LevelNameEdit(const std::string& level_name, QWidget* parent = nullptr);
protected:
    void keyPressEvent(QKeyEvent* event) override;
};