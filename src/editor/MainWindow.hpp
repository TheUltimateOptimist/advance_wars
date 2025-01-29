#include <QMainWindow>
#include "LevelScene.hpp"

class MainWindow : public QMainWindow {
public:
    MainWindow(LevelScene* level, QWidget *parent = nullptr);
};