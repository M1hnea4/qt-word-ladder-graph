#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <vector>
#include "WordLadderLogic.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_startGameBtn_clicked();
    void on_submitWordBtn_clicked();
    void on_hintBtn_clicked();
    void on_analyticsBtn_clicked();

private:
    Ui::MainWindow *ui;
    WordLadderLogic gameLogic;

    std::string currentPlayerName;
    std::string startWord;
    std::string currentWord;
    std::string targetWord;
    int hintsUsedCount;
    std::vector<std::string> matchHistory;
};
#endif // MAINWINDOW_H