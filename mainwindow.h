#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QString fileName;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void save(void);
    void saveAs(void);
    void open(void);
    void closeEvent(QCloseEvent *event);
    QLabel *statusLabelRow, *statusLabelCol, *statusLabelSize;
    void updateStatusBarPos(void);
    void updateStatusBarSize(void);
    void changeFont(void);

    QString getLastDir(void) const;
    void setLastDir(void) const;
};
#endif // MAINWINDOW_H
