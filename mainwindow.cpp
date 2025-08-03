#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFontDialog>
#include <QFont>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("My Notepad😊");
    statusLabelRow = new QLabel("Row: 1");
    statusLabelCol = new QLabel("Column: 1");
    statusLabelSize = new QLabel("Character number: 0");
    ui->statusbar->addPermanentWidget(statusLabelRow);
    ui->statusbar->addPermanentWidget(statusLabelCol);
    ui->statusbar->addWidget(statusLabelSize);
    // ui->statusbar->showMessage("This is a message!", 1000);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAs);
    // connect(this, &MainWindow::closeEvent, this, &MainWindow::closeEvent);
    connect(ui->textEdit, &QTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBarPos);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::updateStatusBarSize);
    connect(ui->actionFont, &QAction::triggered, this, &MainWindow::changeFont);
    connect(ui->actionAbout, &QAction::triggered, ui->statusbar, [this]() {
        static int cnt = 0;
        cnt++;
        this->ui->statusbar->showMessage(QString("%1 times. Keep clicking!").arg(cnt), 500);
    });
}

MainWindow::~MainWindow()

{
    delete ui;
}

QString MainWindow::getLastDir(void) const {
    QSettings settings("MyNotepad", "Notepad");
    settings.beginGroup("MainWindow");
    const QString lastDir = settings.value("lastDir", QString()).toString();
    settings.endGroup();
    return lastDir;
}

void MainWindow::setLastDir(void) const {
    QSettings settings("MyNotepad", "Notepad");
    settings.beginGroup("MainWindow");
    settings.setValue("lastDir", QFileInfo(this->fileName).absolutePath());
    settings.endGroup();
}

void MainWindow::open(void) {
    fileName = QFileDialog::getOpenFileName(this, "Open File", getLastDir(), "Text Files (*.txt);;All Files (*)");
    if(fileName.isEmpty())
        return;
    this->setWindowTitle(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading: " << file.errorString();
        return;
    }
    QTextStream in(&file);
    QString content = in.readAll();
    ui->textEdit->setText(content);

    setLastDir();
}

void MainWindow::save(void) {
    if(fileName.isEmpty())
        fileName = QFileDialog::getSaveFileName(this, "Open File", getLastDir(), "Text Files (*.txt);;All Files (*)");
    if(fileName.isEmpty())
        return;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing: " << file.errorString();
        return;
    }
    QTextStream out(&file);
    QString content = ui->textEdit->toPlainText();
    out << content;

    setLastDir();
}

void MainWindow::saveAs(void) {
    QString fileNametmp = QFileDialog::getSaveFileName(this, "Open File", getLastDir(), "Text Files (*.txt);;All Files (*)");
    if(fileNametmp.isEmpty()) return;

    QFile file(fileNametmp);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing: " << file.errorString();
        return;
    }

    QTextStream out(&file);
    out << ui->textEdit->toPlainText();
    fileName = fileNametmp;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (ui->textEdit->document()->isModified()) {
        auto ret = QMessageBox::question(this, "Exit", "You have unsaved changes. Save before exiting?",
                                         QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (ret == QMessageBox::Yes)
            save();
        else if (ret == QMessageBox::Cancel)
            event->ignore();
    }
}

void MainWindow::updateStatusBarPos(void) {
    auto cursor = ui->textEdit->textCursor();
    this->statusLabelRow->setText("Row: " + QString::number(cursor.blockNumber()+1));
    this->statusLabelCol->setText("Column: " + QString::number(cursor.columnNumber()+1));

}

void MainWindow::updateStatusBarSize(void) {
    this->statusLabelSize->setText("Character number: " + QString::number(ui->textEdit->toPlainText().length()));
}

void MainWindow::changeFont(void) {
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit->setFont(font);
}
