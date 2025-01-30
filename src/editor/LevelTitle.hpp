#include <QLabel>
#include "EventBroker.hpp"

class LevelTitle : public QLabel, EventBroker {
public:
    LevelTitle(int width, int height, const std::string& name, QWidget *parent = nullptr);
    void onLevelNameUpdated(std::string name) override;   
    void setTitle(const std::string& name);
private:
    int width;
    int height;
};