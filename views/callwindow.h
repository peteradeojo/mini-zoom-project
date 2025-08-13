#ifndef CALLWINDOW_H
#define CALLWINDOW_H

#include <QMainWindow>

namespace Ui {
class CallWindow;
}

class CallWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CallWindow(QWidget *parent = nullptr);
    ~CallWindow();

private:
    Ui::CallWindow *ui;
};

#endif // CALLWINDOW_H
