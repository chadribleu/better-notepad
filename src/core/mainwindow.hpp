#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QTabWidget>
#include <QTabBar>
#include <QWidget>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QPushButton>

#include <QFileDialog>
#include <QFontDialog> // Temporary
#include <QMessageBox>

#include <QFile>
#include <QFileInfo>

#include "../framework/commonres.hpp"

// Temporary
#include <QPlainTextEdit>

#include "../components/menubar.hpp"
#include "../components/toolbars.hpp"

class MainWindow : public QMainWindow
{

    Q_OBJECT

public:
    enum class FileStatus{
        Opened,
        AlreadyOpened,
        Closed,
        FailedToOpen,
        Canceled,
        Saved,
        Unsaved,
        FailedToSave
    };

    MainWindow();

private:
    MenuBar *m_menubar;
    ToolBar *m_toolbar;

    // Tab manager
    QTabWidget *m_tabbar;

    QWidget *m_page;
    QWidget* m_tabButtonsPanel;

    QPushButton *m_addTabButton;

    QVector<QPlainTextEdit*> m_tarea;

    // Data
    QString m_filters = tr("Plain text files (*.txt, *.text);;comma-separated value files(*.csv);;Extensible markup language files (*.xml);;All files(*.*)");
    QString m_default_dir = QDir::homePath();

    QVector<QString> m_cur_doc_path;
    QVector<QString> m_cur_doc_name;
    QVector<QString> m_cur_doc_ext;

signals:
    void FileOpened(QFile &file);

private slots:
    // Tabs
    void NewTabFrom(QFile &f_path);
    void CloseCurrentTab(int index);

    // Toolbar and menubar
    //FileStatus NewFile();
    MainWindow::FileStatus Open();
    //FileStatus OpenSaveFileDialog();
    //FileStatus SaveChange();
    //FileStatus Save();
};

#endif