#include <QMainWindow>
#include <QGraphicsScene>

class MainWindow : public QMainWindow {
public:
    MainWindow(QGraphicsScene* level, QWidget *parent = nullptr);
};