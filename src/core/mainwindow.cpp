#include "mainwindow.hpp"
// Set palet avec le thème sombre de Windows
// Handle same path file

MainWindow::MainWindow(){
    setMinimumSize(800, 600);

    // Menubar init
    m_menubar = new MenuBar(this);
    m_toolbar = new ToolBar(this, ToolBar::ActID::Actions, true, true, Qt::ToolButtonStyle::ToolButtonTextBesideIcon);

    setMenuBar(m_menubar);
    addToolBar(m_toolbar);

    QFile tempfile("");

    // ========================= CREATE A NEW TAB BAR
    m_tabbar = new QTabWidget;
    m_tabbar->setTabsClosable(true);
    m_tabbar->setDocumentMode(true);

    setCentralWidget(m_tabbar);


    // ========================== CONNECT BUTTONS
    connect(m_tabbar, &QTabWidget::tabCloseRequested, this, &MainWindow::CloseCurrentTab);
    connect(this, &MainWindow::FileOpened, this, &MainWindow::NewTabFrom);

    connect(m_menubar, &MenuBar::OpenPressed, this, &MainWindow::Open);
    connect(m_toolbar, &ToolBar::OpenPressed, this, &MainWindow::Open);

    // ========================== CREATE A NEW PAGE IN THE TAB BAR
    NewTabFrom(tempfile);
}

void MainWindow::NewTabFrom(QFile &f_path){

    // ============= CREATE THE PAGE WIDGET
    // Utilisation de ::size -1 car cela retourne le nombre d'élément et non le dernier index (qui est de 3 pour 4 éléments par exemple)


    m_tarea.push_back(new QPlainTextEdit);
    int index = m_tarea.size() - 1;
    m_tarea.at(index)->setFrameStyle(0x000); // Get the last index

    QVBoxLayout *pageLayout = new QVBoxLayout; // We don't need access to the current QVBoxLayout widget
    pageLayout->setContentsMargins(0, 0, 0, 0);
    pageLayout->addWidget(m_tarea.at(index)); // Add the current text area widget

    m_page = new QWidget;
    m_page->setLayout(pageLayout);

    // ============= Update the current document path

    QFileInfo fileInfo(f_path);
    QString file_content = f_path.readAll();

    m_cur_doc_path.push_back(fileInfo.absolutePath());
    m_cur_doc_name.push_back(fileInfo.baseName());
    m_cur_doc_ext.push_back(fileInfo.suffix());

    if(!file_content.isEmpty()){
        m_tarea[index]->setPlainText(file_content);
    }

    // ============= Set to the current tab the current page

    m_tabbar->addTab(m_page, m_cur_doc_name.isEmpty() ? tr("Untitled Document %1").arg(index+1) : m_cur_doc_name[index] + "." + m_cur_doc_ext[index]);
    m_tabbar->setCurrentIndex(index);
}

void MainWindow::CloseCurrentTab(int index){
    // Grey background with text "Drag and drop a document here to start"
    if(m_tabbar->count() > 1){
        m_cur_doc_ext.erase(m_cur_doc_ext.cbegin() + index);
        m_cur_doc_path.erase(m_cur_doc_path.cbegin() + index);
        m_cur_doc_name.erase(m_cur_doc_name.cbegin() + index);

        m_tabbar->removeTab(index);
    }
}

MainWindow::FileStatus MainWindow::Open(){
    // Check save change here
    QString path = QFileDialog::getOpenFileName(this, tr("Open a file..."), QString() /*Dir in settings*/, "All files (*.*)");
    if(path == ""){
        return FileStatus::Canceled;
    }
    else{
        // Create a new file and emit UpdateCurrentDocument
        QFile document(path);
        if(!document.open(QFileDevice::ReadWrite)){
            QMessageBox::information(this, "Notepad with toolbars", tr("Unable to open the file in\n%1").arg(path));
            return FileStatus::FailedToOpen;
        }
        emit FileOpened(document);
    }
    return FileStatus::Opened;
}