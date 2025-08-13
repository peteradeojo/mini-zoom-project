#ifndef JOINCALLWINDOW_H
#define JOINCALLWINDOW_H

#include <QMainWindow>

namespace Ui {
class JoinCallWindow;
}

class JoinCallWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JoinCallWindow(QWidget *parent = nullptr);
    ~JoinCallWindow();

private:
    Ui::JoinCallWindow *ui;
};

#endif // JOINCALLWINDOW_H
