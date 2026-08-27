#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QDateTime>
#include <fstream>
#include <set>
#include <sstream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    srand(time(nullptr));

    std::vector<std::string> dictionary;
    std::ifstream file("words.txt");

    if (file.is_open()) {
        std::string word;
        while (file >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::toupper);
            dictionary.push_back(word);
        }
        file.close();
    } else {
        dictionary = {"SAME", "CAME", "CASE", "CAST", "COST"};++++++++++++++++++++++++++++++++++++++++++++++++++++
    }

    gameLogic.buildGraph(dictionary);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    std::string start = ui->lineEdit->text().toUpper().toStdString();
    std::string target = ui->lineEdit_2->text().toUpper().toStdString();

    std::vector<std::string> path = gameLogic.findShortestPath(start, target);

    if (path.empty()) {
        ui->textEdit->setText("No path found or invalid words!");
    } else {
        QString result = "Path found:\n";
        for (const std::string& word : path) {
            result += QString::fromStdString(word) + " -> ";
        }
        result.chop(4);
        ui->textEdit->setText(result);
    }
}

void MainWindow::on_startGameBtn_clicked()
{
    currentPlayerName = ui->nameInput->text().toStdString();
    if (currentPlayerName.empty()) {
        ui->gameHistoryDisplay->setText("Please enter your name first!");
        return;
    }

    startWord = gameLogic.getRandomWord();
    targetWord = gameLogic.getRandomWord();

    while (startWord == targetWord || gameLogic.findShortestPath(startWord, targetWord).empty()) {
        targetWord = gameLogic.getRandomWord();
    }

    currentWord = startWord;
    hintsUsedCount = 0;
    matchHistory.clear();
    matchHistory.push_back(startWord);

    ui->currentWordLabel->setText(QString::fromStdString("Current: " + currentWord));
    ui->targetWordLabel->setText(QString::fromStdString("Target: " + targetWord));
    ui->gameHistoryDisplay->setText("Game Started! Good Luck.");
}

void MainWindow::on_submitWordBtn_clicked()
{
    std::string nextWord = ui->wordInput->text().toUpper().toStdString();
    ui->wordInput->clear();

    if (nextWord.length() != currentWord.length()) {
        ui->gameHistoryDisplay->append("Error: Word length must be the same!");
        return;
    }

    if (!gameLogic.isValidWord(nextWord)) {
        ui->gameHistoryDisplay->append("Error: Word does not exist in dictionary!");
        return;
    }

    int differences = 0;
    for (size_t i = 0; i < currentWord.length(); ++i) {
        if (currentWord[i] != nextWord[i]) {
            differences++;
        }
    }

    if (differences != 1) {
        ui->gameHistoryDisplay->append("Error: You must change exactly ONE letter!");
        return;
    }

    currentWord = nextWord;
    matchHistory.push_back(currentWord);
    ui->currentWordLabel->setText(QString::fromStdString("Current: " + currentWord));
    ui->gameHistoryDisplay->append(QString::fromStdString("Moved to: " + currentWord));

    if (currentWord == targetWord) {
        ui->gameHistoryDisplay->append("\nCONGRATULATIONS! You reached the target word.");

        int optimalMoves = gameLogic.findShortestPath(startWord, targetWord).size() - 1;
        int playerMoves = matchHistory.size() - 1;

        std::string filename = currentPlayerName + ".csv";
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << startWord << ","
                 << targetWord << ","
                 << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString() << ","
                 << hintsUsedCount << ","
                 << playerMoves << ","
                 << optimalMoves << ",\"";

            for (size_t i = 0; i < matchHistory.size(); ++i) {
                file << matchHistory[i] << (i < matchHistory.size() - 1 ? "-" : "");
            }
            file << "\"\n";
            file.close();
        }
        ui->gameHistoryDisplay->append("Game saved to " + QString::fromStdString(filename));
    }
}

void MainWindow::on_hintBtn_clicked()
{
    std::vector<std::string> path = gameLogic.findShortestPath(currentWord, targetWord);
    if (path.size() > 1) {
        std::string nextOptimal = path[1];
        hintsUsedCount++;

        QString richText = "Hint: ";
        for (size_t i = 0; i < nextOptimal.length(); ++i) {
            if (nextOptimal[i] != currentWord[i]) {
                richText += "<span style='color:red;'><b>" + QString(nextOptimal[i]) + "</b></span>";
            } else {
                richText += QString(nextOptimal[i]);
            }
        }
        ui->gameHistoryDisplay->append(richText);
    } else {
        ui->gameHistoryDisplay->append("No hint available.");
    }
}

void MainWindow::on_analyticsBtn_clicked()
{
    std::string searchName = ui->analyticsNameInput->text().toStdString();
    std::string filename = searchName + ".csv";
    std::ifstream file(filename);

    if (!file.is_open()) {
        ui->analyticsDisplay->setText("No game history found for this user.");
        return;
    }

    std::set<std::string> uniqueWords;
    std::string line;
    int totalGames = 0;

    while (std::getline(file, line)) {
        totalGames++;
        size_t startQuote = line.find("\"");
        size_t endQuote = line.rfind("\"");
        if (startQuote != std::string::npos && endQuote != std::string::npos) {
            std::string wordList = line.substr(startQuote + 1, endQuote - startQuote - 1);
            std::stringstream ss(wordList);
            std::string singleWord;
            while (std::getline(ss, singleWord, '-')) {
                uniqueWords.insert(singleWord);
            }
        }
    }
    file.close();

    QString output = QString::fromStdString("User: " + searchName + "\n");
    output += "Total games played: " + QString::number(totalGames) + "\n";
    output += "Unique words used across all games: " + QString::number(uniqueWords.size()) + "\n\nList of unique words:\n";
    for (const auto& w : uniqueWords) {
        output += QString::fromStdString(w) + " ";
    }

    ui->analyticsDisplay->setText(output);
}