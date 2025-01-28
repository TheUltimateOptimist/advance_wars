#include <QToolBar>

class TopBar : public QToolBar {
public: 
    TopBar(const std::string& level_name, int level_width, int level_height, QWidget *parent = nullptr);
};