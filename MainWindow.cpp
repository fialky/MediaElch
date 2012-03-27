#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QDebug>
#include <QDir>
#include <QPainter>

#include "data/MediaCenterInterface.h"
#include "data/ScraperInterface.h"
#include "ExportDialog.h"
#include "Manager.h"
#include "MovieImageDialog.h"
#include "MovieSearch.h"
#include "QuestionDialog.h"
#include "SettingsDialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_progressBar = new QProgressBar(ui->statusBar);
    m_progressBar->hide();
    m_aboutDialog = new AboutDialog(ui->centralWidget);
    m_exportDialog = new ExportDialog(ui->centralWidget);
    m_filterWidget = new FilterWidget(ui->mainToolBar);
    setupToolbar();

    Manager::instance();
    SettingsDialog::instance(ui->centralWidget);

    if (SettingsDialog::instance()->mainWindowSize().isValid()) {
        resize(SettingsDialog::instance()->mainWindowSize());
    }
    if (!SettingsDialog::instance()->mainWindowPosition().isNull()) {
        move(SettingsDialog::instance()->mainWindowPosition());
    }

    Manager::instance()->movieFileSearcher()->setMovieDirectories(SettingsDialog::instance()->movieDirectories());

    connect(Manager::instance()->movieFileSearcher(), SIGNAL(moviesLoaded()), this, SLOT(progressFinished()));
    connect(Manager::instance()->movieFileSearcher(), SIGNAL(progress(int,int)), this, SLOT(progressProgress(int,int)));
    connect(Manager::instance()->movieFileSearcher(), SIGNAL(searchStarted(QString)), this, SLOT(progressStarted(QString)));
    connect(Manager::instance()->movieFileSearcher(), SIGNAL(moviesLoaded()), this, SLOT(setActionExportEnabled()));
    connect(Manager::instance()->movieFileSearcher(), SIGNAL(searchStarted(QString)), this, SLOT(setActionExportDisabled()));
    connect(ui->filesWidget, SIGNAL(movieSelected(Movie*)), ui->movieWidget, SLOT(setMovie(Movie*)));
    connect(ui->filesWidget, SIGNAL(movieSelected(Movie*)), ui->movieWidget, SLOT(setEnabledTrue()));
    connect(ui->filesWidget, SIGNAL(noMovieSelected()), ui->movieWidget, SLOT(clear()));
    connect(ui->filesWidget, SIGNAL(noMovieSelected()), ui->movieWidget, SLOT(setDisabledTrue()));
    connect(ui->filesWidget, SIGNAL(setRefreshButtonEnabled(bool)), m_actionRefreshFiles, SLOT(setEnabled(bool)));
    connect(ui->movieWidget, SIGNAL(actorDownloadProgress(int,int)), this, SLOT(progressProgress(int,int)));
    connect(ui->movieWidget, SIGNAL(actorDownloadStarted(QString)), this, SLOT(progressStarted(QString)));
    connect(ui->movieWidget, SIGNAL(actorDownloadFinished()), this, SLOT(progressFinished()));
    connect(ui->movieWidget, SIGNAL(movieChangeCanceled()), ui->filesWidget, SLOT(restoreLastSelection()));
    connect(ui->movieWidget, SIGNAL(setActionSaveEnabled(bool)), this, SLOT(setActionSaveEnabled(bool)));
    connect(ui->movieWidget, SIGNAL(setActionSearchEnabled(bool)), this, SLOT(setActionSearchEnabled(bool)));
    connect(m_filterWidget, SIGNAL(sigFilterTextChanged(QString)), ui->filesWidget, SLOT(setFilter(QString)));

    if (SettingsDialog::instance()->firstTime()) {
        ui->filesWidget->showFirstTime();
        ui->movieWidget->showFirstTime();
    }

    MovieSearch::instance(ui->centralWidget);
    MovieImageDialog::instance(ui->centralWidget);
    QuestionDialog::instance(ui->centralWidget);
    Manager::instance()->movieFileSearcher()->start();

#ifdef Q_WS_WIN
    setStyleSheet(styleSheet() + " #centralWidget { border-bottom: 1px solid rgba(0, 0, 0, 100); } ");
#endif
}

MainWindow::~MainWindow()
{
    SettingsDialog::instance()->setMainWindowSize(size());
    SettingsDialog::instance()->setMainWindowPosition(pos());
    delete SettingsDialog::instance();
    delete ui;
}

void MainWindow::setupToolbar()
{
    setUnifiedTitleAndToolBarOnMac(true);

    QPixmap spanner(":/img/spanner.png");
    QPixmap info(":/img/info.png");
    QPixmap refresh(":/img/arrow_circle_right.png");
    QPixmap exportDb(":/img/folder_in.png");
    QPixmap quit(":/img/stop.png");
    QPixmap search(":/img/magnifier.png");
    QPixmap save(":/img/save.png");
    QPainter p;
    p.begin(&search);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(search.rect(), QColor(0, 0, 0, 100));
    p.end();
    p.begin(&save);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(save.rect(), QColor(0, 0, 0, 100));
    p.end();
    p.begin(&refresh);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(refresh.rect(), QColor(0, 0, 0, 100));
    p.end();
    p.begin(&spanner);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(spanner.rect(), QColor(0, 0, 0, 100));
    p.end();
    p.begin(&exportDb);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(exportDb.rect(), QColor(0, 0, 0, 100));
    p.end();
    p.begin(&info);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(info.rect(), QColor(0, 0, 0, 100));
    p.end();
    p.begin(&quit);
    p.setCompositionMode(QPainter::CompositionMode_SourceIn);
    p.fillRect(quit.rect(), QColor(0, 0, 0, 100));
    p.end();

    m_actionSearch = new QAction(QIcon(search), tr("Search"), this);
    m_actionSave = new QAction(QIcon(save), tr("Save"), this);
    m_actionRefreshFiles = new QAction(QIcon(refresh), tr("Reload"), this);
    m_actionRefreshFiles->setToolTip(tr("Reload Movie List"));
    m_actionSettings = new QAction(QIcon(spanner), tr("Preferences"), this);
    m_actionExport = new QAction(QIcon(exportDb), tr("Export"), this);
    m_actionExport->setToolTip(tr("Export Movie Database"));
    m_actionAbout = new QAction(QIcon(info), tr("About"), this);
    m_actionQuit = new QAction(QIcon(quit), tr("Quit"), this);
    ui->mainToolBar->addAction(m_actionSearch);
    ui->mainToolBar->addAction(m_actionSave);
    ui->mainToolBar->addAction(m_actionRefreshFiles);
    ui->mainToolBar->addAction(m_actionSettings);
    ui->mainToolBar->addAction(m_actionExport);
    ui->mainToolBar->addAction(m_actionAbout);
    ui->mainToolBar->addAction(m_actionQuit);
    ui->mainToolBar->addWidget(m_filterWidget);

    connect(m_actionSearch, SIGNAL(triggered()), ui->movieWidget, SLOT(startScraperSearch()));
    connect(m_actionSave, SIGNAL(triggered()), ui->movieWidget, SLOT(saveInformation()));
    connect(m_actionRefreshFiles, SIGNAL(triggered()), ui->filesWidget, SLOT(startSearch()));
    connect(m_actionSettings, SIGNAL(triggered()), this, SLOT(execSettingsDialog()));
    connect(m_actionExport, SIGNAL(triggered()), m_exportDialog, SLOT(exec()));
    connect(m_actionAbout, SIGNAL(triggered()), m_aboutDialog, SLOT(exec()));
    connect(m_actionQuit, SIGNAL(triggered()), qApp, SLOT(quit()));

    setActionSearchEnabled(false);
    setActionSaveEnabled(false);
    setActionExportDisabled(true);

#ifdef Q_WS_WIN
    ui->mainToolBar->setStyleSheet("QToolButton {border: 0; padding: 5px;} QToolBar { border-bottom: 1px solid rgba(0, 0, 0, 100); }");
#endif
}

void MainWindow::setActionSaveEnabled(bool enabled)
{
    m_actionSave->setEnabled(enabled);
}

void MainWindow::setActionSearchEnabled(bool enabled)
{
    m_actionSearch->setEnabled(enabled);
}

void MainWindow::setActionExportEnabled(bool enabled)
{
    m_actionExport->setEnabled(enabled);
}

void MainWindow::setActionExportDisabled(bool disabled)
{
    m_actionExport->setDisabled(disabled);
}

void MainWindow::execSettingsDialog()
{
    int result = SettingsDialog::instance()->exec();
    if (result == QDialog::Accepted && !Manager::instance()->movieFileSearcher()->isRunning()) {
        Manager::instance()->movieFileSearcher()->start();
    }
}

void MainWindow::progressStarted(QString msg)
{
    ui->filesWidget->disableRefresh();
    ui->statusBar->addWidget(m_progressBar);
    if (!msg.isEmpty()) {
        QLabel *label = new QLabel(msg);
        m_progressLabels.append(label);
        ui->statusBar->addWidget(label);
    }
    m_progressBar->show();
    m_progressBar->setValue(0);
}

void MainWindow::progressProgress(int current, int max)
{
    m_progressBar->setRange(0, max);
    m_progressBar->setValue(current);
}

void MainWindow::progressFinished()
{
    if (Manager::instance()->movieModel()->movies().size() > 0)
        ui->filesWidget->hideFirstTime();
    ui->filesWidget->enableRefresh();
    if (m_progressBar->isVisible())
        ui->statusBar->removeWidget(m_progressBar);
    foreach (QLabel *label, m_progressLabels) {
        ui->statusBar->removeWidget(label);
        label->deleteLater();
    }
    m_progressLabels.clear();
}