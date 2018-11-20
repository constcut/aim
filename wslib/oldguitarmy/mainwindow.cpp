#include "mainwindow.h"
//#include "ui_mainwindow.h"

//Events includes

#include <QPainter>
#include <QKeyEvent>
#include <QMouseEvent>

#include <QDebug>

#include <vector>

#include "gview.h"

#include "apainter.h"

//ONLY FOR GESTURES
//and recog
#include "mainviews.h"
#include "tabviews.h"

#include <QTapAndHoldGesture>
#include <QTapGesture>
#include <QSwipeGesture>

#include <QPushButton>
#include <QStatusBar>

#include <QToolButton>
#include <QMenu>
#include <QToolBar>
#include <QPixmap>

#include <QDockWidget>
#include <QTextCodec>


#include <QStandardPaths>

#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <qendian.h>

#include <QTextBlock>

#include "g0/astreaming.h"

#include <android_helper.h>

#include <QMenuBar>

#include "gpannel.h"

#include <QScroller>
#include <QTapAndHoldGesture>

#include <QInputDialog>

#include "qthelper.h"

//later please incapsulate here:
#include "g0/rec.h"
#include "g0/fft.h"
#include "g0/waveanalys.h"

static AStreaming logger("qhelper");

//-------------------------------

void addToolButtonGrid(MainWindow *mainWindow,QDockWidget *dock, std::string button, std::string confValue="", bool secondLines=false);
QAction* addToolButton(QToolBar *toolBar, std::string button, std::string confValue);



void MainWindow::createMenuTool()
{
    menuToolBar = addToolBar("mainMenu");

    QWidget *sep1 = new QWidget(this);
    sep1->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep1);

    addToolButton(menuToolBar,"backview","esc");
    QAction *hideP1 = addToolButton(menuToolBar,"new","newtab");
    QAction *hideP2 = addToolButton(menuToolBar,"open",CONF_PARAM("Main.open"));

    QWidget *sep2 = new QWidget(this);
    sep2->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep2);


    addToolButton(menuToolBar,"tab","tabview");

    QWidget *sep3 = new QWidget(this);
    sep3->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep3);

    hideA1=addToolButton(menuToolBar,"tests","tests"); //HIDE

    QWidget *sep32 = new QWidget(this);
    sep32->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep32);


    hideA2=addToolButton(menuToolBar,"tap","tap");  //HIDE

    QWidget *sep31 = new QWidget(this);
    sep31->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep31);


    addToolButton(menuToolBar,"pattern","pattern");

    QWidget *sep4 = new QWidget(this);
    sep4->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep4);

    addToolButton(menuToolBar,"config","config");

    QWidget *sep5 = new QWidget(this);
    sep5->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep5);

    hideA3=addToolButton(menuToolBar,"record","rec"); //hide
    hideA4=addToolButton(menuToolBar,"morze","morze"); //hide

    QWidget *sep52 = new QWidget(this);
    sep52->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep52);


    hideA5=addToolButton(menuToolBar,"info","info"); //hide

    QWidget *sep51 = new QWidget(this);
    sep51->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep51);

    QAction *hideP3 = addToolButton(menuToolBar,"undo","undo");
    addToolButton(menuToolBar,"play",CONF_PARAM("TrackView.playMidi"));


    QWidget *sep6 = new QWidget(this);
    sep6->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep6);

    addToolButton(menuToolBar,"openPannel","openPannel");

    QWidget *sep7 = new QWidget(this);
    sep7->setSizePolicy(QSizePolicy::Expanding,
    QSizePolicy::Expanding);
    menuToolBar->addWidget(sep7);

    hideP1->setVisible(false);
    hideP2->setVisible(false);
    hideP3->setVisible(false);
    //hideP4->setVisible(false);
    hideA4->setVisible(false);

    hideA1->setVisible(false);




    this->connect(menuToolBar,SIGNAL(actionTriggered(QAction*)),
                  SLOT(actionNow(QAction*)));


    if (CONF_PARAM("toolBar")=="right")
    {
        menuToolBar->setOrientation(Qt::Vertical);
        addToolBar(Qt::RightToolBarArea,menuToolBar);
    }

    if (CONF_PARAM("toolBar")=="left")
    {
        menuToolBar->setOrientation(Qt::Vertical);
        addToolBar(Qt::LeftToolBarArea,menuToolBar);
    }

    if (CONF_PARAM("toolBar")=="top")
    {
        menuToolBar->setOrientation(Qt::Horizontal);
        addToolBar(Qt::TopToolBarArea,menuToolBar);
    }

    if (CONF_PARAM("toolBar")=="bottom")
    {
        menuToolBar->setOrientation(Qt::Horizontal);
        addToolBar(Qt::BottomToolBarArea,menuToolBar);
    }


    menuToolBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    menuToolBar->setMovable(false);
    menuToolBar->setFloatable(true);

    QScreen *screen = QApplication::screens().at(0);
    double autoCoef = screen->geometry().width() > screen->geometry().height() ?
                screen->geometry().width() : screen->geometry().height();

    autoCoef/=800;



    stringExtended menuStyleStr;
    int menuPlatformCoef = 1;

if (globals.platform == "windows") //check
{
    menuPlatformCoef = 4;
}

    menuStyleStr <<"QMenu { font-size:"<<(int)(autoCoef*30/menuPlatformCoef)<<"px;}QToolBar {border: 1px solid black;}";
    menuToolBar->setStyleSheet(menuStyleStr.c_str());//"QMenu { font-size:40px;}"); //QToolBar{spacing:15px;}

    int iconSize=36*autoCoef;//36


    if (globals.isMobile == false)
    {
        iconSize = 36; //36 debug
        autoCoef = 1.0;
    }


    menuToolBar->setIconSize(QSize(iconSize,iconSize));

    AConfig::getInstance()->setScaleCoef(autoCoef);


    if (CONF_PARAM("toolBar") == "0")
    {
        menuToolBar->hide();
    }

    if (CONF_PARAM("statusBar")=="0")
    {
        statusBar()->hide();
    }
    else
        statusBar()->show();

}


void MainWindow::createMainToolbar()
{
    if (CONF_PARAM("mainMenu") == "1")
    {
        //create menu

        QMenu *m1 = menuBar()->addMenu("tab");
        m1->addAction("tab");
        m1->addAction("opentrack");
        m1->addAction("new");
        m1->addAction("open");
        m1->addAction("save");

        QMenu *m1Addition = new QMenu(m1);

        m1->addMenu(m1Addition)->setIconText("export");;

        m1Addition->addAction("midi");
        m1Addition->addAction("text");

        m1->addAction("exit");

        QMenu *m2  = menuBar()->addMenu("inputs");
        m2->addAction("pattern");
        m2->addAction("tap");
        m2->addAction("rec");
        m2->addAction("morze");
        //piano roll


        QMenu *m3 = menuBar()->addMenu("edit");
        m3->addAction("copy");
        m3->addAction("cut");
        m3->addAction("paste");

        QMenu *m3Addition = new QMenu(m3);
        m3Addition->addAction("bufer1");
        m3Addition->addAction("bufer2");
        m3Addition->addAction("bufer3");

        m3->addMenu(m3Addition)->setIconText("chose bufer");

        QMenu *m3Addition2 = new QMenu(m3);
        m3Addition2->addAction("effects");

        QMenu *m3Addition3 = new QMenu(m3);
        m3Addition3->addAction("specif");

        m3->addMenu(m3Addition2)->setIconText("effects");
        m3->addMenu(m3Addition3)->setIconText("specif");;


        QMenu *m4 = menuBar()->addMenu("info");

        m4->addAction("info");
        m4->addAction("config");
        m4->addAction("about");
        m4->addAction("tests");

        menuBar()->setFont(QFont("Arial",30));

    }

    createMenuTool();

}

QDockWidget *MainWindow::createToolDock(std::string dockname,void *pannel)
{
    GPannel *pan = (GPannel*)pannel;

    QDockWidget *dockTab = new QDockWidget(dockname.c_str(),this);
    QMainWindow *dockWin = new QMainWindow(0);

    dockWin->setWindowFlags(Qt::Widget);
    QToolBar *men = dockWin->addToolBar(dockname.c_str());

    men->setMovable(false);
    men->setFloatable(false);


    QWidget *spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);

    men->addWidget(spacerWidget);

    QWidget* titleWidget = new QWidget(this);
    dockTab->setTitleBarWidget( titleWidget );

    QToolBar *men2 = 0;

    int middleBreak = pan->buttons.size()/2;

    if (middleBreak < 10)
    {
        middleBreak = 4;
    }

    if (CONF_PARAM("pannels")=="oneline")
    {
        middleBreak = 100; //force one line
    }

    for (int i =0 ;i < pan->buttons.size(); ++i)
    {

        if (i > middleBreak) //16?
        {
            if (men2 == 0)
            {
                dockWin->addToolBarBreak(); //need few new toolbars
                men2 = dockWin->addToolBar("second");
                men2->setMovable(false);
                men2->setFloatable(false);

                QWidget *spacerWidget2 = new QWidget(this);
                spacerWidget2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
                spacerWidget2->setVisible(true);

                men2->addWidget(spacerWidget2);
            }

            addToolButton(men2,pan->buttons[i].getText(),pan->buttons[i].getPressSyn());

        }
        else
        {   //first
            addToolButton(men,pan->buttons[i].getText(),pan->buttons[i].getPressSyn());
        }
    }

    dockTab->resize(dockTab->minimumSizeHint());
    dockTab->setWidget(dockWin);

    removeDockWidget(dockTab);

    int iconSize = menuToolBar->iconSize().width();
    men->setIconSize(QSize(iconSize,iconSize));


    //NewStyle style;
    //men->setStyle(&style);


    int iCoef = 4; //for the clip

    if (men2)
    {
        iCoef = 4;
        men2->setIconSize(QSize(iconSize,iconSize));

        //men2->setStyle(&style);

        this->connect(men2,SIGNAL(actionTriggered(QAction*)),
                      SLOT(actionNow(QAction*)));
    }


    dockWin->setMaximumHeight(iconSize*iCoef);

    dockWin->setMaximumWidth(maximumWidth());

    dockWin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    dockTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    this->connect(men,SIGNAL(actionTriggered(QAction*)),
                  SLOT(actionNow(QAction*)));

    dockTab->setAllowedAreas(Qt::BottomDockWidgetArea );

    addDockWidget(Qt::BottomDockWidgetArea,dockTab); //

    dockTab->show();


    return dockTab;
}

QMenu *MainWindow::createToolMenu(void *pannel)
{
    GPannel *pan = (GPannel*)pannel;

    QMenu *menu = new QMenu(0);

    std::string iconsSet;
     if (CONF_PARAM("iconsSet")=="1")
         iconsSet = ":/icons/";
     else
         iconsSet = ":/icons2/";

    for (int i =0 ;i < pan->buttons.size(); ++i)
    {
        std::string button = pan->buttons[i].getText();
        std::string iconPlace = iconsSet+ button + std::string(".png");

        QIcon icon(iconPlace.c_str());

        std::string press = pan->buttons[i].getPressSyn();

        menu->addAction(icon,press.c_str());
    }

    return menu;
}

void MainWindow::createUI()
{
    GPannel *pan1 = new GTabPannel(1,2,3);
    GPannel *pan2 = new GTrackPannel(1,2,3);
    GPannel *pan3 = new GEffectsPannel(1,2,3);
    GPannel *pan4 = new GClipboardPannel(1,2,3);

    menu1 = createToolMenu(pan1);
    menu2 = createToolMenu(pan2);
    menu3 = createToolMenu(pan3);
    menu4 = createToolMenu(pan4);

    createMainToolbar();
    createFloDocks();

    statusLabel = new QLabel(this);
    statusLabelSecond = new QLabel(this);
    //statusLabelThird = new QLabel(this);

    win = new GQCombo(this);
    win->addItem("welcome");

    if (CONF_PARAM("")=="1")
    win->addItem("tabview"); //tab

    win->addItem("trackview");
    //win->addItem("track");
    win->addItem("tap");
    win->addItem("rec");
    win->addItem("pattern");
    win->addItem("tests");
    win->addItem("config");
    win->addItem("info");
    //win->addItem("bend_view");
    //win->addItem("change_view");
    //win->addItem("chord_view");
    //win->addItem("openPannel");
    //our new more
    win->setElementNum(0);


    GQCombo *win2 = new GQCombo(this);
    win2->addItem("menu");
    win2->addItem("track / bar");
    win2->addItem("effects");
    win2->addItem("edit / clipboard");
    win2->addItem("num / move"); //tab
    win2->addItem("all");

    win2->setElementNum(0);


    statusBar()->addWidget(statusLabel,4);
    statusBar()->addWidget(statusLabelSecond,4);
    //statusBar()->addWidget(statusLabelThird,2);
    statusBar()->addWidget(win,3);
    statusBar()->addWidget(win2,3);



    initAudioInput();
    initAudioOutput();


    //showHelp();

    if (center == 0)
        center = new CenterView();

    if (centerScroll==0)
        centerScroll = new QScrollArea(this);

    centerScroll->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//refact - could be styled
    /*
   QString style = "QScrollBar:vertical {\
            width: 45px;\
            margin: 21px 0 21px 0;}";
    centerScroll->setStyleSheet(style);
    */

    center->setFatherScroll(centerScroll);

    win->setKeyPress(center);
    win->setPushItem(true);//REFACT - rename
    win2->setKeyPress(center);
    win2->setPushItem(true);

    double coef = AConfig::getInstance()->getScaleCoef();

    int initWidth = 770 * coef;
    int initHeightMin = 300 * coef;

    center->setMinimumHeight(initHeightMin);
    center->setMinimumWidth(initWidth);;

    center->setGeometry(0,0,initWidth, initHeightMin);
    center->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    centerScroll->setWidget(center);
    centerScroll->setWidgetResizable(true);
/*
#ifndef __ANDROID_API__
    QScroller::grabGesture(centerScroll, QScroller::LeftMouseButtonGesture);
#else
    QScroller::grabGesture(centerScroll, QScroller::TouchGesture);
#endif
*/

    center->setStyleSheet("background-color:rgba(0,0,0,0);");

    setCentralWidget(centerScroll);

    centerScroll->show();
    center->show();


    setDockOptions( QMainWindow::AllowTabbedDocks |  QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

    setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );



    if (CONF_PARAM("pannels")=="tabbed")
    {
        QDockWidget *tabDock = createToolDock("tab",pan1);
        QDockWidget *trackDock = createToolDock("track",pan2);
        QDockWidget *effectsDock = createToolDock("effects",pan3);
        QDockWidget *clipDock = createToolDock("clipboard",pan4);


        QDockWidget *userDock = createToolDock("user",new GUserPannel(1,2,3));
        //Menu from pannel QMenu *tabMenu = createToolMenu("tab",pan1);

        dock5 = tabDock;
        dock6 = trackDock;
        dock7 = effectsDock;
        dock8 = clipDock;
        dock9 = userDock;

        tabifyDockWidget(trackDock,tabDock);
        tabifyDockWidget(trackDock,effectsDock);
        tabifyDockWidget(trackDock,clipDock);
        tabifyDockWidget(trackDock,userDock);
        tabifyDockWidget(userDock,trackDock);


        dock5->hide();
        dock6->hide();
        dock7->hide();
        dock8->hide();
        dock9->hide();
    }
    else  if (CONF_PARAM("pannels")=="oneline")  //classic is banned
    {       
        GPannel *pan1=new GNumPannel();
        //GPannel *pan2=new GMovePannel(); //for refact future //make too toolbars and tool bar break
        GPannel *pan3=new GEditPannel();
        //GPannel *pan4=new GClipboardPannel(1,2,3);
        //GPannel *pan5=new GBeatPannel();
        GPannel *pan6=new GNotePannel();
        GPannel *pan7=new GTrackNewPannel();
        //GPannel *pan8=new GBarPannel();
        GPannel *pan9=new GUserPannel(1,2,3);

        dock5 = createToolDock("num / move",pan1);
        //dock6 = createToolDock("move",pan2);
        dock6 = createToolDock("edit / clipboard",pan3);
        //dock7 = createToolDock("clip",pan4);
        dock7 = createToolDock("effects",pan6);
        //QDockWidget *dock8 = createToolDock("note",pan6);
        dock8 = createToolDock("track / bar",pan7);
        //QDockWidget *dock10 = createToolDock("bar",pan8);
        dock9 = createToolDock("menu",pan9);

        tabifyDockWidget(dock9,dock5);
         tabifyDockWidget(dock9,dock6);
          tabifyDockWidget(dock9,dock7);
           tabifyDockWidget(dock9,dock8);




    }


    if (CONF_PARAM("pannels")=="docked")
    {
        dock->show();
        dock2->show();
        dock3->show();
        dock4->show();
    }
}

void MainWindow::recreateUI()
{

    dock5->setVisible(false);
    dock6->setVisible(false);
    dock7->setVisible(false);
    dock8->setVisible(false);
    dock9->setVisible(false);


    removeDockWidget(dock5);
    removeDockWidget(dock6);
    removeDockWidget(dock7);
    removeDockWidget(dock8);
    removeDockWidget(dock9);

    /*
    delete dock5;
    delete dock6;
    delete dock7;
    delete dock8;
    delete dock9;
    */

    dock5->deleteLater();
    dock6->deleteLater();
    dock7->deleteLater();
    dock8->deleteLater();
    dock9->deleteLater();


    dock5=0;
    dock6=0;
    dock7=0;
    dock8=0;
    dock9=0;



    //delete menuToolBar;
    removeToolBar(menuToolBar);
    //delete menuToolBar;
    createMenuTool();


    if (CONF_PARAM("pannels")=="tabbed")
    {


        GPannel *pan1 = new GTabPannel(1,2,3);
        GPannel *pan2 = new GTrackPannel(1,2,3);
        GPannel *pan3 = new GEffectsPannel(1,2,3);
        GPannel *pan4 = new GClipboardPannel(1,2,3);
        QDockWidget *tabDock = createToolDock("tab",pan1);
        QDockWidget *trackDock = createToolDock("track",pan2);
        QDockWidget *effectsDock = createToolDock("effects",pan3);
        QDockWidget *clipDock = createToolDock("clipboard",pan4);


        QDockWidget *userDock = createToolDock("user",new GUserPannel(1,2,3));
        //Menu from pannel QMenu *tabMenu = createToolMenu("tab",pan1);

        dock5 = tabDock;
        dock6 = trackDock;
        dock7 = effectsDock;
        dock8 = clipDock;
        dock9 = userDock;

        tabifyDockWidget(trackDock,tabDock);
        tabifyDockWidget(trackDock,effectsDock);
        tabifyDockWidget(trackDock,clipDock);
        tabifyDockWidget(trackDock,userDock);
        tabifyDockWidget(userDock,trackDock);

    }
    else  if (CONF_PARAM("pannels")=="oneline")  //classic is banned
    {
        GPannel *pan1=new GNumPannel();
        GPannel *pan3=new GEditPannel();
        GPannel *pan6=new GNotePannel();
        GPannel *pan7=new GTrackNewPannel();
        GPannel *pan9=new GUserPannel(1,2,3);

        dock5 = createToolDock("num / move",pan1);
        dock6 = createToolDock("edit / clipboard",pan3);
        dock7 = createToolDock("effects",pan6);
        dock8 = createToolDock("track / bar",pan7);
        dock9 = createToolDock("menu",pan9);

        tabifyDockWidget(dock9,dock5);
        tabifyDockWidget(dock9,dock6);
        tabifyDockWidget(dock9,dock7);
        tabifyDockWidget(dock9,dock8);
    }


}

bool MainWindow::eventFilter(QObject *object, QEvent *e)
{
    if (e->type() == QEvent::KeyPress)
      {
          QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);

          if ((keyEvent->key() == Qt::Key_Right)
              ||(keyEvent->key() == Qt::Key_Left)
                  ||(keyEvent->key() == Qt::Key_Up)
                  ||(keyEvent->key() == Qt::Key_Down)
                  ||(keyEvent->key() == Qt::Key_Space))
          {
            keyPressEvent(keyEvent);
            return true;
          }


          //return true;
      }
     return false;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    MasterView(),
    center(0),centerScroll(0)
{
    dock5=0;
    dock6=0;
    dock7=0;
    dock8=0;
    dock9=0;
    win=0;

    AClipboard::
    setCurrentClip(&clip1);

    qApp->installEventFilter(this);

    createUI();
}




void MainWindow::resizeEvent(QResizeEvent* event)
{

    if (geometry().width() < geometry().height())
    {
        //menuToolBar->set
        //hideA1->setVisible(false);
        hideA2->setVisible(false);
        hideA3->setVisible(false);
        //hideA4->setVisible(false);
        hideA5->setVisible(false);
    }
    else
    {
        //hideA1->setVisible(true);
        hideA2->setVisible(true);
        hideA3->setVisible(true);
        //hideA4->setVisible(true);
        hideA5->setVisible(true);
    }
}


int MainWindow::getCurrentViewType()
{
    GView *gview = getChild();
    MainView *mv = dynamic_cast<MainView*>(gview);

    if (mv)
    {
        gview = mv->getCurrenView();

        if (dynamic_cast<TabView*>(gview) != 0)
            return 1;

        if (dynamic_cast<TrackView*>(gview) != 0)
            return 2;

        if (dynamic_cast<PatternInput*>(gview) != 0)
            return 3;

        if (dynamic_cast<TapRyView*>(gview) != 0)
            return 4;

        if (dynamic_cast<RecordView*>(gview) != 0)
            return 5;

        if (dynamic_cast<ConfigView*>(gview) != 0)
            return 6;

        if (dynamic_cast<InfoView*>(gview) != 0)
            return 7;

        if (dynamic_cast<InfoView*>(gview) != 0)
            return 7;

        if (dynamic_cast<TestsView*>(gview) != 0)
            return 8;

        if (dynamic_cast<MorzeInput*>(gview) != 0)
            return 9;

    }
    return 0;
}


void MainWindow::threadFinished()
{
    if (isPlaylistHere())
    {
        goOnPlaylist();
    }
}

void MainWindow::pushForceKey(std::string keyevent)
{
    if (keyevent=="openPannel")
    {
        if (dock5)
        {
            if (dock5->isVisible())
            {
                dock5->setVisible(false);
                dock6->setVisible(false);
                dock7->setVisible(false);
                dock8->setVisible(false);
                dock9->setVisible(false);
            }
            else
            {
                dock5->setVisible(true);
                dock6->setVisible(true);
                dock7->setVisible(true);
                dock8->setVisible(true);
                dock9->setVisible(true);
            }
        }
    }
    if (keyevent=="playMidi")
    {
        if (center)
            center->pushForceKey(CONF_PARAM("TrackView.playMidi"));
    }
    if (keyevent=="menu") //REFACT - make checkable
    {
        dock5->setVisible(false);
        dock6->setVisible(false);
        dock7->setVisible(false);
        dock8->setVisible(false);
        dock9->setVisible(true);
    }
    if (keyevent=="track / bar")
    {
        dock5->setVisible(false);
        dock6->setVisible(false);
        dock7->setVisible(false);
        dock8->setVisible(true);
        dock9->setVisible(false);
    }
    if (keyevent=="effects")
    {
        dock5->setVisible(false);
        dock6->setVisible(false);
        dock7->setVisible(true);
        dock8->setVisible(false);
        dock9->setVisible(false);
    }
    if (keyevent=="edit / clipboard")
    {
        dock5->setVisible(false);
        dock6->setVisible(true);
        dock7->setVisible(false);
        dock8->setVisible(false);
        dock9->setVisible(false);
    }
    if (keyevent=="num / move")
    {
        dock5->setVisible(true);
        dock6->setVisible(false);
        dock7->setVisible(false);
        dock8->setVisible(false);
        dock9->setVisible(false);
    }
    if (keyevent=="all")
    {
        dock5->setVisible(true);
        dock6->setVisible(true);
        dock7->setVisible(true);
        dock8->setVisible(true);
        dock9->setVisible(true);
    }

    if (keyevent=="start_audioi") //"openPannel"
    {
        startAudioInput();
    }

    if (keyevent=="stop_audioi") //"openPannel"
    {
        stopAudioInput();
    }

    if (keyevent=="stop_record_output")
    {
        stopAudioOutput();
        notifyAndroid();
    }

    if (keyevent.substr(0,19)=="start_record_output") //cut it with substr
    {
        std::string stdFileName = keyevent.substr(20);
        startAudioOutput(stdFileName);
    }
}

void MainWindow::actionNow(QAction *action)
{
  QString textOut;
  textOut=action->iconText();

  if (textOut.size()>0)
  LOG( <<"Action now trig: "<<textOut.toStdString());

/* fixed:
  //bug
  if (textOut==CONF_PARAM("TrackView.playMidi").c_str())
  {

     keyMute.lock();
     static clock_t lastClick = 0;
     clock_t now = getTime();

     clock_t diff = now-lastClick;



     LOG(<<"Press on play last dif"<<diff);
     //hotfix:

     if (lastClick)
         if (diff < 1000)
         {
             logger << "Too short";
         }

     //if (CONF_PARAM("crashOnPlayHotFix")=="1")
       //thread()->msleep(1000);

    //thread()->msleep(1000); //check

     lastClick = now;
     keyMute.unlock();
  }*/

  if (textOut=="tab")
  {
      if (dock)
      {
          if (dock->isHidden())
          {
              dock->setGeometry(this->pos().x()+50,this->pos().y()+390,750,110);
              dock->show();
          }
          else
            dock->hide();
      }
  }

  if (textOut=="track_dock")
  {
      if (dock3)
      {
          if (dock3->isHidden())
          {
              dock3->setGeometry(this->pos().x()+50,this->pos().y()+390,650,110);
              dock3->show();
          }
          else
            dock3->hide();
      }
  }

  if (textOut=="effects")
  {
      if (dock4)
      {
          if (dock4->isHidden())
          {
              dock4->setGeometry(this->pos().x()+50,this->pos().y()+390,750,110);
              dock4->show();
          }
          else
            dock4->hide();
      }
  }

  if (textOut=="clipboard")
  {
      if (dock2)
      {
          if (dock2->isHidden())
          {
              dock2->setGeometry(this->pos().x()+50,this->pos().y()+390,500,60);
              dock2->show();
          }
          else
            dock2->hide();
      }
  }

  if (textOut=="zoomIn")
  {
      double currentScale = AConfig::getInstance()->getScaleCoef();

      if (currentScale <= 4.0)
      {
          currentScale += 0.25;
          AConfig::getInstance()->setScaleCoef(currentScale);

          int initWidth = 770 * currentScale;
          int initHeightMin = 300 * currentScale;
          int initHeightMax = 500 * currentScale;

          center->setMinimumHeight(initHeightMin);
          //center->setMaximumHeight(initHeightMax);
          center->setMinimumWidth(initWidth);

          center->setGeometry(0,0,initWidth, initHeightMin);
      }
  }
  if (textOut=="zoomOut")
  {
      double currentScale = AConfig::getInstance()->getScaleCoef();

      if (currentScale >= 0.5)
      {
          currentScale -= 0.25;
          AConfig::getInstance()->setScaleCoef(currentScale);

          int initWidth = 770 * currentScale;
          int initHeightMin = 300 * currentScale;
          int initHeightMax = 500 * currentScale;

          center->setMinimumHeight(initHeightMin);
         // center->setMaximumHeight(initHeightMax);
          center->setMinimumWidth(initWidth);

          center->setGeometry(0,0,initWidth, initHeightMin);
      }
  }

  if (textOut == "clip1")
    AClipboard::setCurrentClip(&clip1);

  if (textOut == "clip2")
    AClipboard::setCurrentClip(&clip2);

  if (textOut == "clip3")
    AClipboard::setCurrentClip(&clip3);


  if (textOut=="start_audioi") //"openPannel"
  {  
      startAudioInput();
      return;
  }

  if (textOut=="stop_audioi") //"openPannel"
  {
      stopAudioInput();
      return;
  }

  if (textOut=="stop_record_output")
  {
      stopAudioOutput();
      notifyAndroid();
      return;
  }

  if (textOut.mid(0,19)=="start_record_output") //cut it with substr
  {
      QString fileName = textOut.mid(20);
      std::string stdFileName = fileName.toStdString();
      startAudioOutput(stdFileName);
      return;
  }

  if (textOut == "exit")
  {
      exit(0);
  }

  if (textOut == "darkSkin")
  {
    DarkSkin skin;
    skin.setIntoConfig(AConfig::getInstance());
#ifndef __ANDROID_API__
    recreateUI(); //to remember the bug
#endif
    update();
    return;
  }
  if (textOut == "lightSkin")
  {
      LightSkin skin;
      skin.setIntoConfig(AConfig::getInstance());
#ifndef __ANDROID_API__
    recreateUI(); //refact return back
#endif
    update();
    return;
  }
  if (textOut == "classicSkin")
  {
      ClassicSkin skin;
      skin.setIntoConfig(AConfig::getInstance());
#ifndef __ANDROID_API__
    recreateUI(); //refact return back
#endif
    update();
    return;
  }
  if (textOut == "classicInvSkin")
  {
      ClassicInvertedSkin skin;
      skin.setIntoConfig(AConfig::getInstance());
#ifndef __ANDROID_API__
    recreateUI(); //refact return back
#endif
    update();
    return;
  }

  if (textOut == "save config")
  {
    AFile confFile;
    std::string confFileName = std::string(getTestsLocation()) + "g.config";
    confFile.open(confFileName,false);
    AConfig::getInstance()->save(confFile);
    return;
  }

  if (textOut == "recreateUI")
  {
      recreateUI();
      return;
  }

  if (textOut=="openPannel")
  {

      if (dock5)
      {
          if (dock5->isVisible())
          {
              dock5->setVisible(false);
              dock6->setVisible(false);
              dock7->setVisible(false);
              dock8->setVisible(false);
              dock9->setVisible(false);
          }
          else
          {
              dock5->setVisible(true);
              dock6->setVisible(true);
              dock7->setVisible(true);
              dock8->setVisible(true);
              dock9->setVisible(true);
          }
      }
      else
      {

          if (CONF_PARAM("pannels")=="tabbed")
          {
          GPannel *pan1 = new GTabPannel(1,2,3);
          GPannel *pan2 = new GTrackPannel(1,2,3);//memory leak
          GPannel *pan3 = new GEffectsPannel(1,2,3);
          GPannel *pan4 = new GClipboardPannel(1,2,3);//memleak


          QDockWidget *tabDock = createToolDock("tab",pan1);
          QDockWidget *trackDock = createToolDock("track",pan2);
          QDockWidget *effectsDock = createToolDock("effects",pan3);
          QDockWidget *clipDock = createToolDock("clipboard",pan4);


           QDockWidget *userDock = createToolDock("user",new GUserPannel(1,2,3));
          //Menu from pannel QMenu *tabMenu = createToolMenu("tab",pan1);

          dock5 = tabDock;
          dock6 = trackDock;
          dock7 = effectsDock;
          dock8 = clipDock;
          dock9 = userDock;

          tabifyDockWidget(trackDock,tabDock);
          tabifyDockWidget(trackDock,effectsDock);
          tabifyDockWidget(trackDock,clipDock);
          tabifyDockWidget(trackDock,userDock);
          tabifyDockWidget(userDock,trackDock);
          }



      }




      update();
      if (CONF_PARAM("pannels")!="classic")
      return; //think about this pair..update in some destructor
  }

  if (textOut=="only user")
  {
          dock5->setVisible(false);
          dock6->setVisible(false);
          dock7->setVisible(false);
          dock8->setVisible(false);
          dock9->setVisible(true);
          return;
  }

  if (textOut=="only clipboard")
  {
          dock5->setVisible(false);
          dock6->setVisible(false);
          dock7->setVisible(false);
          dock8->setVisible(true);
          dock9->setVisible(false);
          return;
  }

  if (textOut=="only effects")
  {
          dock5->setVisible(false);
          dock6->setVisible(false);
          dock7->setVisible(true);
          dock8->setVisible(false);
          dock9->setVisible(false);
          return;
  }

  if (textOut=="only track")
  {
          dock5->setVisible(false);
          dock6->setVisible(true);
          dock7->setVisible(false);
          dock8->setVisible(false);
          dock9->setVisible(false);
          return;
  }

  if (textOut=="only tab")
  {
          dock5->setVisible(true);
          dock6->setVisible(false);
          dock7->setVisible(false);
          dock8->setVisible(false);
          dock9->setVisible(false);
          return;
  }

  if (textOut == "all pannels")
  {
      dock5->setVisible(true);
      dock6->setVisible(true);
      dock7->setVisible(true);
      dock8->setVisible(true);
      dock9->setVisible(true);
      return;
  }

  //int viewBeforeRun = getCurrentViewType();

  //MAIN HANDLING OF KEY EVENTS THROUGH ALL THE GVIEWs

  {


     // keyMute.lock();


      if (textOut.size() > 0)
      center->pushForceKey(textOut.toStdString());


      //keyMute.unlock();
  }
  //==================================================
  ///MAIN HANDLING

  update();
}

void MainWindow::moveEvent(QMoveEvent *ev)
{
    int x1 = ev->pos().x();
    int y1 = ev->pos().y();

    int x2 = ev->oldPos().x();
    int y2 = ev->oldPos().y();

    if (dock)
    {
        int dockX = dock->pos().x();
        int dockY = dock->pos().y();

        int newDX = x1-x2 + dockX;
        int newDY = y1-y2 + dockY;

        dock->setGeometry(newDX,newDY,750,110);
    }
    //dock->setGeometry(this->pos().x()+50,this->pos().y()+390,750,110);
}


void MainWindow::setStatusBarMessage(int index, std::string text, int timeOut)
{
    QString uniText = QTextCodec::codecForLocale()->toUnicode(text.c_str());
   if (index==0)
   {
       //if (timeOut==0) timeOut = 10000;

      statusBar()->showMessage(uniText,timeOut);
   }
   if (index==1)
       statusLabel->setText(uniText);
   if (index==2)
       statusLabelSecond->setText(uniText);
   if (index==3)
   {
       //missing third one
       logger<<"No 3rd status label now!!";
    //   statusLabelThird->setText(uniText);
   }


}

MainWindow::~MainWindow()
{
   //d el ete u i;
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);


    QFont defaultFont("Times");//arial-9
    defaultFont.setPixelSize(14);
    //defaultFont.setWeight();
    painter.setFont(defaultFont);

    Painter abstractedPainer(&painter);

    if (getChild())
        abstractedPainer.fillRect(0,0,width(),height(),CONF_PARAM("colors.background"));
    else
    {

    }

    abstractedPainer.drawImage(0,0,"bg");

    abstractedPainer.changeColor(CONF_PARAM("colors.default"));


    double scaleCoef = AConfig::getInstance()->getScaleCoef();

    qreal scaleX = scaleCoef;
    qreal scaleY = scaleCoef;

    painter.scale(scaleX,scaleY);


    if (getChild())
    getChild()->draw(&abstractedPainer);
    else
    {
         //center->draw(&abstractedPainer);
    }

    ///////////////////////////////////////



}




void MainWindow::mouseReleaseEvent(QMouseEvent *mEvent)
{
   double scaleCoef = AConfig::getInstance()->getScaleCoef();

   int xPress = mEvent->pos().x()  / scaleCoef;
   int yPress = mEvent->pos().y() / scaleCoef;

   int xDiff = lastPressX - xPress;
   int yDiff = lastPressY - yPress;

   bool isMoved = ((abs(xDiff)+abs(yDiff))>20);

   int gesturesMlt = CONF_PARAM("gestures")=="classic"?-1:1;


   if ((abs(xDiff*5) < abs(yDiff)) && (isMoved))
   {
       //vertical definitly
       yDiff*=gesturesMlt;
       if (getChild())
        getChild()->ongesture(yDiff,false);


   }
   else if ((abs(xDiff) > abs(yDiff*5)) && (isMoved))
   {
       //horizontal
       xDiff*=gesturesMlt;
       if ((abs(xDiff)+abs(yDiff))>20)
       if (getChild())
      getChild()->ongesture(xDiff,true);

   }
   else
   {
       //some other
       //press
       if (getChild())
        getChild()->onclick(xPress,yPress);

   }

   update();
}


void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    //if (center)
       // center->keyPressEvent(event);

    int value = event->key();
    char key = (char)value;
    char c_style[2];

    c_style[0] = tolower(key);
    c_style[1] = 0;

    std::string singleKey = "";
    singleKey = c_style;



    //if (value==Qt::Key_Back)
    //    singleKey = "backview";


    //no escape no more
    if (value == Qt::Key_Right)
         singleKey =">>>";
    else if (value == Qt::Key_Left)
         singleKey ="<<<";
    else if (value == Qt::Key_Up)
         singleKey ="^^^";
    else if (value == Qt::Key_Down)
         singleKey ="vvv";
    else if (value == Qt::Key_Space)
         singleKey ="spc";
    else if (value == Qt::Key_Return)
         singleKey ="ent";
    else if (value == Qt::Key_Delete)
         singleKey ="del";
    else if (value == Qt::Key_Escape)
         singleKey ="esc";
    else if (value == Qt::Key_Back)
        singleKey = "esc"; //key back !!
    else if (value == Qt::Key_Insert)
        singleKey = "ins";


    //INSERT


    if (event->modifiers() & Qt::ControlModifier)
        singleKey = std::string("ctrl+") + singleKey;

    if (event->modifiers() & Qt::AltModifier)
        singleKey = std::string("alt+") + singleKey;

    if (event->modifiers() & Qt::ShiftModifier)
        singleKey = std::string("shift+") + singleKey;

    //QString titileNow(singleKey.c_str());
    //this->setWindowTitle(titileNow);
     //repaint();


    if (center)
        center->pushForceKey(singleKey);

    event->accept();


     //push to touch function
        this->repaint();
}


//-------------------------Touch handle-------------------------------------------
/*void MainWindow::handlePanelAction(int action) // int row
{

}*/

void MainWindow::mousePressEvent( QMouseEvent * event )
{
    //if (center)
       // center->mousePressEvent(event);

    int xPress = event->localPos().x(); //event->globalX();
    int yPress = event->localPos().y(); //event->globalY();


    if(event->buttons()&Qt::RightButton)
    {
            //not used yet
    }

    if(event->buttons()&Qt::LeftButton)
    {
        double scaleCoef = AConfig::getInstance()->getScaleCoef();

        lastPressX = xPress/scaleCoef;
        lastPressY = yPress/scaleCoef;

        //stringExtended sX;
        //sX<<"x="<<xPress<<"; y="<<yPress;

        //this->setWindowTitle(sX.c_str());
        // if (getChild())
        //getChild()->onclick(xPress,yPress);
    }

    //may be not always??

}


void MainWindow::mouseDoubleClickEvent( QMouseEvent * event )
{
    //if (center)
        //center->mouseDoubleClickEvent(event);

    //calculate button position
    //push to touch function

   if(event->buttons()&Qt::LeftButton)
   {
       int xPress = event->localPos().x(); //event->globalX();
       int yPress = event->localPos().y(); //event->globalY();

        if (getChild())
            getChild()->ondblclick(xPress,yPress);
   }
   //may be not always??
   update();
}

//-------------------------Menus--------------------------------------------------

void MainWindow::on_action800x480_triggered()
{
    int x = this->geometry().x();
    int y = this->geometry().y();
    this->setGeometry(x,y,800,480);
    getChild()->setW(800);
    getChild()->setH(480);
}

void MainWindow::on_action1024x600_triggered()
{
    int x = this->geometry().x();
    int y = this->geometry().y();
    this->setGeometry(x,y,1024,600);
    getChild()->setW(1024);
    getChild()->setH(600);
}

void MainWindow::on_action1280x720_triggered()
{
    int x = this->geometry().x();
    int y = this->geometry().y();
    this->setGeometry(x,y,1280,720);
    getChild()->setW(1280);
    getChild()->setH(720);
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

//
//-------------------------gestures------------------------

bool MainWindow::gestureEvent(QGestureEvent *event)
{
    stringExtended  sX;

    //if (event-)

    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
    {
        QPinchGesture *pinchGest = (QPinchGesture*)pinch;

        if (pinch->state() == Qt::GestureFinished) //Finished)
        {
           qreal scale = pinchGest->totalScaleFactor();
           double dScale = scale;
           //dScale += 1.0;

           double nowScale = AConfig::getInstance()->getScaleCoef();


           if (CONF_PARAM("turnPinchZoomOn")=="1")
           {
                   //
                   stringExtended line;
                   line<< "Scale "<<dScale<<"; with conf "<<nowScale;
                   setStatusBarMessage(0,line.c_str(),2000);
                //update();

                   if (scale < 0.15)
                   {
                       //very low
                       dScale *= 5.0;
                       nowScale *=dScale;
                   }

                   if (scale > 5.0)
                   {
                       dScale /= 5.0; //then mult
                       nowScale *=dScale;
                   }

                   AConfig::getInstance()->setScaleCoef(nowScale);
                   update();
           }


           if ((nowScale >= 0.5) && (nowScale <= 5.0))
           {
               /*
               GView *gview = getChild();
               MainView *mv = dynamic_cast<MainView*>(gview);

               if (mv)
               {
                   gview = mv->getCurrenView();
                   ConfigView *cView = dynamic_cast<ConfigView*>(gview);

                   if (cView != 0)
                   {
                        AConfig::getInstance()->setScaleCoef(nowScale);
                        update();
                   }
               }
               */
           }

        }
    }



    sX << "gestureEvent():" << event->gestures().size();

    if (QGesture *swipe = event->gesture(Qt::SwipeGesture))
    {
        sX << "Swipe"; //swipeTriggered(static_cast<QSwipeGesture *>(swipe));
        QSwipeGesture *swipeGesture = static_cast<QSwipeGesture *>(swipe);
        if (swipeGesture->horizontalDirection())
        {
            sX <<"Vert";
            if (dock5->isVisible()==false)
            {
                dock5->setVisible(true);
                dock6->setVisible(true);
                dock7->setVisible(true);
                dock8->setVisible(true);
                dock9->setVisible(true);
            }
        }
    }

    if(QGesture *tap = event->gesture(Qt::TapGesture))
    {
        sX <<"Tap ";
        QTapGesture *tapGesture = static_cast<QTapGesture*>(tap);
        sX<<tapGesture->position().x()<<" "<<tapGesture->position().y();

    }

    if (QGesture *tapNhold = event->gesture(Qt::TapAndHoldGesture))
    {
        sX <<"Tap and hold "<<(int)tapNhold;


        QTapAndHoldGesture *tapGest = dynamic_cast<QTapAndHoldGesture*>(tapNhold);

        //if(tapGest->state() == Qt::GestureFinished)

        {
            if (CONF_PARAM("tapAndHoldCopy")=="1")
            {

            LOG(<<"Tap and hold x="<<tapGest->position().x()<<"; y="
                 <<tapGest->position().y());

            //QTapAndHoldGesture *tap2 = static_cast<QTapAndHoldGesture *>(tapNhold);
            //sX<<tap2->position.x()<<" "<<tap2->position.y();

            if (center)
            center->ondblclick(tapGest->position().x(),tapGest->position().y());
            //not sure but why no

            event->accept();
            }
        }
    }

    if (QGesture *pan = event->gesture(Qt::PanGesture))
    {
        /*
        QPanGesture *panGesture = static_cast<QPanGesture*>(pan);
        //panGesture->
        sX <<"Pan gesture "<<panGesture->offset().x()<<" "
             <<panGesture->offset().y();

        int absX = panGesture->offset().x();
        int absY = panGesture->offset().y();

        if (absX < 0) absX *= -1;
        if (absY < 0) absY *= -1;

        if (panGesture->state() == Qt::GestureFinished)
        {
            if (absX > absY)
            {
                //horizontal offset
                 if (getChild())
                getChild()->ongesture(panGesture->offset().x(), true);
            }
            else
            {
                //vertical offset
                 if (getChild())
                getChild()->ongesture(panGesture->offset().y(), false);
            }

            update();
        }
        */
    }

    //setWindowTitle(sX.c_str());


    /*
    else if (QGesture *pan = event->gesture(Qt::PanGesture))
        std::cout << "Pan"; //panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        std::cout << "Pinch"; //pinchTriggered(static_cast<QPinchGesture *>(pinch));
    */

    return true;
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::Gesture)
        return gestureEvent(static_cast<QGestureEvent*>(event));
    return QWidget::event(event);
}

///////////////Audio inputs///////////////////////////////////
/// \brief MainWindow::startAudioInput
///
///

const int BufferSize = 4096;

AudioInfo::AudioInfo(const QAudioFormat &format, QObject *parent)
    :   QIODevice(parent)
    ,   m_format(format)
    ,   m_maxAmplitude(0)
    ,   m_level(0.0)

{
    switch (m_format.sampleSize()) {
    case 8:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 255;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 127;
            break;
        default:
            break;
        }
        break;
    case 16:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 65535;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 32767;
            break;
        default:
            break;
        }
        break;

    case 32:
        switch (m_format.sampleType()) {
        case QAudioFormat::UnSignedInt:
            m_maxAmplitude = 0xffffffff;
            break;
        case QAudioFormat::SignedInt:
            m_maxAmplitude = 0x7fffffff;
            break;
        case QAudioFormat::Float:
            m_maxAmplitude = 0x7fffffff; // Kind of
        default:
            break;
        }
        break;

    default:
        break;
    }
}

AudioInfo::~AudioInfo()
{
}

void AudioInfo::start()
{ open(QIODevice::WriteOnly);}

void AudioInfo::stop()
{close();}

qint64 AudioInfo::readData(char *data, qint64 maxlen)
{   Q_UNUSED(data)
    Q_UNUSED(maxlen)

    //used for output data stream

    return 0;
}



qint64 AudioInfo::writeData(const char *data, qint64 len)
{

        //int audioLen = len;

        //static int portionCounter=0;
        //++portionCounter;

        collector += QByteArray(data,len);


        //Here is place for tuner
        static int lastSize = 0;


        if (collector.size() - lastSize > 4100)
        {

            lastSize = collector.size();
            //if it does slows anything - then use it less freq
            //may be also have a param
            ///bool tuner;



            short *sourceData = (short*)collector.data();

            int fullSize = collector.size()/2;
            int minusLastFrame = fullSize-2049;

            FFT fft(2048);
            fft.transform(&sourceData[minusLastFrame]);
            fft.countAmplitude();
            fft.findPeaks(7.8125/2.0);
            std::vector<Peak> *peaks=fft.getPeaks();

            LocalFreqTable localFreq;
            localFreq.addPeaks(peaks);
            localFreq.sortPeaks();
            localFreq.voteNewWay();

            std::vector<LocalFreqTable::LFTvote> *votes = localFreq.getRezultVotes();

            double freq = (*votes)[0].rFreq;
            //using pointer from outside we will
            //set found freq later

            ///tuner->setFreq(freq);
            //Then - the tuner sets the value into the
            //GLabel - and after it pushes repaint request

            TunerInstance::getInst()->setFreq(freq);
        }


        int border = 16000*30; //each 60 seconds in smallest

        if (collector.size() > border*10) //format*bitrate*minute
        {

            ///QByteArray compress = qCompress(collector,7);
            QString defaultRecFile = QString(getTestsLocation()) + QString("record.temp");
            QFile f; f.setFileName(defaultRecFile);
            ///int compressedSize = compress.size();

            if (f.open(QIODevice::Append))
            {
                ///GOOD THING _ TO COMPRESS _ BUT NOW WE USE WAY MORE SIMPLE
                /*//
                QByteArray fromInt;
                fromInt = QByteArray::number(compressedSize);

                f.write(fromInt);
                f.write(compress);

                LOG( << "Compressed wroten: "<<compressedSize);

                //*/
                f.write(collector);
                f.flush();
                f.close();
            }
            else
                logger << "Open file for raw record error;";


            collector.clear();

            //ATTENTION HERE _ IT COULD BE USED FOR paid version - just skip from here

        }

    int fullLen = collector.size();
    int cutLen = len;

    //exceed log
    //if (0)
    LOG( << "Wroten audio data "<<cutLen<<"; in bufer "<<fullLen);
    //emit update();
    return len;
}
/// AUDIO INPUT FINISHED
// NOW OUTPUT

AudioSpeaker::AudioSpeaker(const QAudioFormat &format,
                     QObject *parent)
    :   QIODevice(parent)
    ,   m_pos(0)
{
    //if (format.isValid())
        //generateData(format, durationUs, sampleRate);
}

AudioSpeaker::~AudioSpeaker()
{
}

void AudioSpeaker::start()
{
    open(QIODevice::ReadOnly);
}

void AudioSpeaker::stop()
{
    m_pos = 0;
    close();
}

void AudioSpeaker::generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate)
{
    const int channelBytes = format.sampleSize() / 8;
    const int sampleBytes = format.channelCount() * channelBytes;

    qint64 length = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
                        * durationUs / 100000;

    Q_ASSERT(length % sampleBytes == 0);
    Q_UNUSED(sampleBytes); // suppress warning in release builds

    m_buffer.resize(length);
    unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());
    int sampleIndex = 0;

    while (length) {
        const qreal x = 0; //qSin(2 * M_PI * sampleRate * qreal(sampleIndex % format.sampleRate()) / format.sampleRate());
        for (int i=0; i<format.channelCount(); ++i) {
            if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt) {
                const quint8 value = static_cast<quint8>((1.0 + x) / 2 * 255);
                *reinterpret_cast<quint8*>(ptr) = value;
            } else if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt) {
                const qint8 value = static_cast<qint8>(x * 127);
                *reinterpret_cast<quint8*>(ptr) = value;
            } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt) {
                quint16 value = static_cast<quint16>((1.0 + x) / 2 * 65535);
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<quint16>(value, ptr);
                else
                    qToBigEndian<quint16>(value, ptr);
            } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt) {
                qint16 value = static_cast<qint16>(x * 32767);
                if (format.byteOrder() == QAudioFormat::LittleEndian)
                    qToLittleEndian<qint16>(value, ptr);
                else
                    qToBigEndian<qint16>(value, ptr);
            }

            ptr += channelBytes;
            length -= channelBytes;
        }
        ++sampleIndex;
    }
}

qint64 AudioSpeaker::readData(char *data, qint64 len)
{
    qint64 total = 0;
    if (!m_buffer.isEmpty()) {
        while (len - total > 0) {
            const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
            memcpy(data + total, m_buffer.constData() + m_pos, chunk);
            m_pos = (m_pos + chunk) % m_buffer.size();
            total += chunk;
        }
    }
    return total;
}

qint64 AudioSpeaker::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
}

qint64 AudioSpeaker::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}


void AudioSpeaker::setAudioBufer(QByteArray &aStream)
{
    m_buffer.clear();
    m_buffer += aStream;
}

#define audioSampleRateG 8000

void MainWindow::initAudioInput()
{
    QAudioFormat format;
    format.setSampleRate(audioSampleRateG);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format)) {
        logger << "Default format not supported - trying to use nearest";
        format = info.nearestFormat(format);
    }

    audioInfo  = new AudioInfo(format, this);
    //connect(audioInfo, SIGNAL(update()), SLOT(refreshDisplay()));
    audioInput = new QAudioInput(QAudioDeviceInfo::defaultInputDevice(), format, this);

    ///audioInput->setNotifyInterval(100);
    ///audioInput->setBufferSize();

    //m_volumeSlider->setValue(m_audioInput->volume() * 100);
}


void MainWindow::audioTimerSlot()
{
    /*
    if (m_audioOutput && m_audioOutput->state() != QAudio::StoppedState) {
        int chunks = m_audioOutput->bytesFree()/m_audioOutput->periodSize();
        while (chunks) {
           const qint64 len = m_generator->read(m_buffer.data(), m_audioOutput->periodSize());
           if (len)
               m_output->write(m_buffer.data(), len);
           if (len != m_audioOutput->periodSize())
               break;
           --chunks;
        }
    }
    //*/
}

void MainWindow::initAudioOutput()
{
    //this->connect(audioPushTimer, SIGNAL(timeout()), SLOT(audioTimerSlot()));

    QAudioFormat format;
    format.setSampleRate(audioSampleRateG);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultInputDevice());
    if (!info.isFormatSupported(format)) {
        logger << "Default format not supported - trying to use nearest";
        format = info.nearestFormat(format);
    }

    //create generator
    audioSpeaker = new AudioSpeaker(format);

    audioOutput = new QAudioOutput(QAudioDeviceInfo::defaultOutputDevice(), format, this);

}

void MainWindow::startAudioOutput(std::string localName)
{
    QFile audioFile;
    QString defaultRecFile = QString(getTestsLocation()) + QString(localName.c_str());
    audioFile.setFileName(defaultRecFile);

    if (audioFile.open(QIODevice::ReadOnly) == false)
        logger << "Failed to open audio for output";

    QByteArray allBytes = audioFile.readAll();
    audioFile.close();

    audioSpeaker->setAudioBufer(allBytes);

    audioSpeaker->start();
    audioOutput->start(audioSpeaker);

   //may be there is a way to wait..
}

void MainWindow::stopAudioOutput()
{
    audioSpeaker->stop();
    audioOutput->stop();
}

void MainWindow::startAudioInput()
{
    ///QString defaultRecFile = QString(getTestsLocation()) + QString("record.graw");

    ///if (QFile::exists(defaultRecFile))
    ///        QFile::remove(defaultRecFile);

    audioInfo->collector.clear();


    //audio_logger<<"Audio input started";
    audioInfo->start();
    audioInput->start(audioInfo);

}

void MainWindow::stopAudioInput()
{
    //audio_logger<<"Audio input stopped";
    audioInfo->stop();
    audioInput->stop();


    QDateTime timeDate(QDateTime::currentDateTime());
    QString st = timeDate.toString(Qt::ISODate);
    std::string time = st.toStdString();
    size_t pos1 = time.find(":",0);
    size_t pos2 = time.find(":",pos1+1);
    //size_t pos3 = time.find(":",pos2+1);

    const std::string rep = ".";
    time.replace(pos1,1,rep);
    time.replace(pos2,1,rep);
    //time.replace(pos3,1,rep);

    std::string saveName = "record.graw";

    QString recordNameV = "rec_";
            recordNameV += time.c_str();
            recordNameV += ".graw";

    bool ok=false;
    QString inputedText = QInputDialog::getText(0,"Input record name","Input record name:",QLineEdit::Normal,recordNameV,&ok);

    if (ok)
    {
        saveName = inputedText.toStdString();
        if (saveName.find(".graw")==std::string::npos)
            saveName += ".graw";
    }
    else
    {
        //default
        //ask do you want to delete?
    }


    //save the rest of file
    ///QByteArray compress = qCompress(audioInfo->collector,7);
    QString defaultRecFile = QString(getTestsLocation()) + QString(saveName.c_str());
    QFile f; f.setFileName(defaultRecFile);
    ///int compressedSize = compress.size();

    if (f.open(QIODevice::Append))
    {
        /*
        QByteArray fromInt;
        fromInt = QByteArray::number(compressedSize);

        f.write(fromInt);
        f.write(compress);
        */

        ////logger << "Compressed wroten: "<<compressedSize;
        ///

        LOG( << "Collector size was "<<audioInfo->collector.size());

        f.write(audioInfo->collector);
        f.flush();
        f.close();
    }
    else
        logger << "Open file for raw record error;";


    audioInfo->collector.clear();

    //check up

    startAudioOutput(saveName);
}

///// ///////////////////////////////
//==================================panels===============================================


QAction* MainWindow::addToolButton(QToolBar *toolBar, std::string button, std::string confValue)
{

    std::string iconPlace = "";
    //std::string(getTestsLocation()) + std::string("Icons/") + button + std::string(".png");

    std::string iconsSet;
    if (CONF_PARAM("iconsSet")=="1")
        iconsSet = ":/icons/";
    else
        iconsSet = ":/icons2/";

    iconPlace = iconsSet+ button + std::string(".png");



    QIcon icon(iconPlace.c_str());


    QPixmap iPix = icon.pixmap(72,72);
    QImage imageIcon = iPix.toImage();


    if (imageIcon.isNull())
    {
        logger << "Image empty";
    }
    else
    {
        if (CONF_PARAM("invertImages")=="1")
        {
            imageIcon.invertPixels();
        }
    }

    //no invert while

    QImage *imgs =(QImage*)AConfig::getInstance()->imageLoader.getImage(button);
    QPixmap result;
    if (imgs)
    {
        QImage scI = *imgs;
    QPixmap newPix(72,72);
    result = newPix.fromImage(scI);
    }
    //
    //satan mode
    //newPix.fill(QColor("red"));
    QIcon iconNew(result); //newPix);



    QString actionName = button.c_str();
    if (confValue.size())
        actionName = confValue.c_str();

    QAction *act = toolBar->addAction(iconNew,actionName);

    stringExtended sX;
    sX << button.c_str() <<" ["<<confValue<<"]";

    act->setToolTip(sX.c_str());

    if (actionName=="esc")
    {
        QMenu *menu = new QMenu(toolBar);
        menu->addAction("exit");
        menu->addAction("darkSkin");
        menu->addAction("lightSkin");
        //menu->addAction("classicInvSkin");
        menu->addAction("classicSkin");
        act->setMenu(menu);

    }

    if (actionName=="rec")
    {
          QMenu *menu = new QMenu(toolBar);
          menu->addAction("start_audioi");
          menu->addAction("stop_audioi");
          menu->addAction("start_record_output");
          menu->addAction("stop_record_output");
          act->setMenu(menu);
    }

    if (actionName == "tabview")
    {
        QMenu *menu = new QMenu(toolBar);

        QIcon icon2(":/icons/new.png");
        menu->addAction(icon2,"newtab");

        QIcon icon1(":/icons/open.png");
        menu->addAction(icon1,QString(CONF_PARAM("Main.open").c_str()));

        QIcon icon3(":/icons/save.png");
        menu->addAction(icon3,"quicksave");

        menu->addAction("export_midi");

        menu->addAction("save as");

        act->setMenu(menu);
    }



    if (actionName=="openPannel")
    {
        QMenu *menu = new QMenu(toolBar);

        //classic of dock
        //QAction *pannelType = new QAction("docked",menu);
        //pannelType->setCheckable(true);
        //menu->addAction(pannelType);

        QMenu *docksMenu = new QMenu("docks",menu);
        docksMenu->addAction("tab");
        docksMenu->addAction("track");
        docksMenu->addAction("effects");
        docksMenu->addAction("clipboard");


        //docksMenu->addAction("set position");


        //QAction *moveType = new QAction("movable",docksMenu);
        //moveType->setCheckable(true);
        //docksMenu->addAction(moveType);

        menu->addMenu(docksMenu);

        QMenu *pans = new QMenu(menu);
        pans->addAction("only tab");
        pans->addAction("only track");
        pans->addAction("only effects");
        pans->addAction("only clipboard");
        pans->addAction("only user");
        pans->addAction("all pannels");

        menu->addMenu(pans)->setIconText("pannels");

        QMenu *m6 = new QMenu(menu); //menuBar()->addMenu("pannels");
        m6->addMenu(menu1)->setIconText("tab");
        m6->addMenu(menu2)->setIconText("track_dock");
        m6->addMenu(menu3)->setIconText("effects");
        m6->addMenu(menu4)->setIconText("clipboard");

        pannelsMenu = m6;
        menu->addMenu(pannelsMenu)->setIconText("menu");



        //hide show dock pannels by one!!

        act->setMenu(menu);



    }

    if (button=="pattern")
    {
        //inputs group
        std::string icon1Place = std::string(getTestsLocation()) + std::string("Icons/")
                + std::string("tap.png");
        QIcon icon1(icon1Place.c_str());

        std::string icon2Place = std::string(getTestsLocation()) + std::string("Icons/")
                + std::string("record.png");
        QIcon icon2(icon2Place.c_str());

        std::string icon3Place = std::string(getTestsLocation()) + std::string("Icons/")
                + std::string("morze.png");
        QIcon icon3(icon3Place.c_str());


        QMenu *menu = new QMenu(toolBar);

        QAction *act1 = menu->addAction(icon1,"tap"); //tap
        act1->setText("tap");
        //act1->set

        QAction *act2 = menu->addAction(icon2,"rec"); //recording
        QAction *act3 = menu->addAction(icon3,"morze"); //morze input
        menu->addAction(icon,"pattern"); //and itself

        act->setMenu(menu);
    }

    if (button=="play")
    {
        QMenu *menu = new QMenu(toolBar);
        menu->addAction("playMidi");
        menu->addAction("playAMusic");
        menu->addAction("playMerge");
        menu->addAction("playFromStart");
        menu->addAction("goToStart");
        act->setMenu(menu);
    }

    if (button=="config")
    {
        //conf group
        std::string icon1Place = std::string(getTestsLocation()) + std::string("Icons/")
                + std::string("info.png");
        QIcon icon1(icon1Place.c_str());

        std::string icon2Place = std::string(getTestsLocation()) + std::string("Icons/")
                + std::string("tests.png");
        QIcon icon2(icon2Place.c_str());

        QMenu *menu = new QMenu(toolBar);
        QAction *act1 = menu->addAction(icon1,"info");
        QAction *act2 = menu->addAction(icon2,"tests");

        menu->addAction(icon,"config");
        menu->addAction("save config");
        menu->addAction("recreateUI");

        act->setMenu(menu);
    }

    if (confValue==CONF_PARAM("TrackView.deleteNote"))
    {
        QMenu *menu = new QMenu(toolBar);
        menu->addAction("delete bar");
        menu->addAction("delete selected bars");
        menu->addAction("delete selected beats");
        act->setMenu(menu);
    }

    if (confValue == "signs")
    {
        QMenu *menu = new QMenu(toolBar);
        menu->addAction("set till the end"); //signs
        menu->addAction("set for selected"); //signs too
        act->setMenu(menu);
    }

    return act;
}

void addToolButtonGrid(MainWindow *mainWindow,QDockWidget *dock, std::string button, std::string confValue="", bool secondLines=false)
{
    static QWidget *wi = 0;


    static int counter=-1;
    static int counter2=-1;

    if (confValue.empty())
        confValue=button;

    if (mainWindow == 0)
    {
        if (dock == 0)
        {
            counter=-1;
            counter2=-1;
            wi=0;
            return;

        }
    }


    if (wi == 0)
    {
        wi = new QWidget(dock);
        //wi->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        dock->setWidget(wi);

        wi->setGeometry(0,0,700,100);
    }

    QToolButton *but = new QToolButton(wi);
    std::string iconPlace = "";//std::string(getTestsLocation()) + std::string("Icons/") + button + std::string(".png");
    iconPlace = ":/icons/"+ button + std::string(".png");
    QIcon icon(iconPlace.c_str());
    but->setIcon(icon);

    //QAction *act = new QAction(icon,confValue.c_str(),wi);


    QString actionName = button.c_str();
    if (confValue.size())
        actionName = confValue.c_str();


    but->setDefaultAction(new QAction(icon,confValue.c_str(),dock));
    but->setAccessibleName(confValue.c_str());
    but->setIconSize(QSize(50,50));


    //but->addAction(new QAction(confValue.c_str(),dock));

    mainWindow->connect(but,SIGNAL(triggered(QAction*)),
                  SLOT(actionNow(QAction*)));


    int thisCounter = 0;

    if (secondLines)
        thisCounter =++counter2;
    else
        thisCounter =++counter;

    int yPos = secondLines ? 55 : 0;
    int xPos = thisCounter*50;

    but->setGeometry(xPos,yPos,50,50);

}

void MainWindow::createFloDocks()
{
    dock = new QDockWidget(this,Qt::Window | Qt::WindowStaysOnTopHint |
                           Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint); //("pannel",this);


    addToolButtonGrid(0,0,"",""); //new dock

    QLabel *labelTitle = new QLabel("pannel",dock);
    dock->setTitleBarWidget(labelTitle);


    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
                      |QDockWidget::DockWidgetVerticalTitleBar);

    //dock->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    //dock->setWindowFlags( Qt::WindowSystemMenuHint | Qt::WindowTitleHint);

    dock->setAllowedAreas(Qt::NoDockWidgetArea);

    this->addDockWidget(Qt::NoDockWidgetArea ,  dock );
    //this->tabifyDockWidget(dock,dockWidget2);


     QToolBar *navigation2=0;
     QToolBar *navigation=0;


    addToolButtonGrid(this,dock,"play",CONF_PARAM("TrackView.playAMusic"));

    for (int i = 0; i < 10; ++i)
    {
     stringExtended sX; sX <<i;
     addToolButtonGrid(this,dock,sX.c_str(),sX.c_str());
    }



    addToolButtonGrid(this,dock,"prevBar",CONF_PARAM("TrackView.prevBar"));
    addToolButtonGrid(this,dock,"upString",CONF_PARAM("TrackView.stringUp"));
    addToolButtonGrid(this,dock,"nextBar",CONF_PARAM("TrackView.nextBar"));




    addToolButtonGrid(this,dock,"play",CONF_PARAM("TrackView.playMidi"),true);

     addToolButtonGrid(this,dock,"qp","q",true);
     addToolButtonGrid(this,dock,"qm","w",true); //refact conf prarm it
     addToolButtonGrid(this,dock,"p","p",true);
     addToolButtonGrid(this,dock,"del",CONF_PARAM("TrackView.deleteNote"),true);
     addToolButtonGrid(this,dock,"-3-","-3-",true);
     addToolButtonGrid(this,dock,"leeg","leeg",true);
     addToolButtonGrid(this,dock,"x","dead",true);
     addToolButtonGrid(this,dock,"newBar","newBar",true);
     addToolButtonGrid(this,dock,"save","quicksave",true);//CONF_PARAM("TrackView.save")
     addToolButtonGrid(this,dock,"open","quickopen",true); //CONF_PARAM("TrackView.quickOpen")



    addToolButtonGrid(this,dock,"prevBeat",CONF_PARAM("TrackView.prevBeat"),true);
    addToolButtonGrid(this,dock,"downString",CONF_PARAM("TrackView.stringDown"),true);
    addToolButtonGrid(this,dock,"nextBeat",CONF_PARAM("TrackView.nextBeat"),true);



    dock->setGeometry(90,390,750,110);
    dock->show();


    QDockWidget *dock2 = new QDockWidget(this,Qt::Window | Qt::WindowStaysOnTopHint |
                           Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint); //("pannel",this);

    QLabel *labelTitle2 = new QLabel("clips ",dock2);
    dock2->setTitleBarWidget(labelTitle2);

    dock2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
                      |QDockWidget::DockWidgetVerticalTitleBar);

    dock2->setAllowedAreas(Qt::NoDockWidgetArea);

    this->addDockWidget(Qt::NoDockWidgetArea ,  dock2 );

    addToolButtonGrid(0,0,"",""); //new dock

    addToolButtonGrid(this,dock2,"copy","copy"); //must config it
    addToolButtonGrid(this,dock2,"copyBeat","copyBeat");
    addToolButtonGrid(this,dock2,"copyBars","copyBars");
    addToolButtonGrid(this,dock2,"cut","cut");
    addToolButtonGrid(this,dock2,"paste","paste");
    addToolButtonGrid(this,dock2,"clip1","clip1");
    addToolButtonGrid(this,dock2,"clip2","clip2");
    addToolButtonGrid(this,dock2,"clip3","clip3");

    dock2->setGeometry(90,290,440,50);
    dock2->show();



    QDockWidget *dock3 = new QDockWidget(this,Qt::Window | Qt::WindowStaysOnTopHint |
                           Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint); //("pannel",this);

    QLabel *labelTitle3 = new QLabel("tabs  ",dock3);
    dock3->setTitleBarWidget(labelTitle3);

    dock3->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
                      |QDockWidget::DockWidgetVerticalTitleBar);

    dock3->setAllowedAreas(Qt::NoDockWidgetArea);

    this->addDockWidget(Qt::NoDockWidgetArea ,  dock3);

    addToolButtonGrid(0,0,"",""); //new dock

    addToolButtonGrid(this,dock3,"next",">>>"); //must config it
    addToolButtonGrid(this,dock3,"prev","<<<");
    addToolButtonGrid(this,dock3,"^","^^^");
    addToolButtonGrid(this,dock3,"V","vvv");
    addToolButtonGrid(this,dock3,"goToN");
    addToolButtonGrid(this,dock3,"opentrack");
    addToolButtonGrid(this,dock3,"newTrack");
    addToolButtonGrid(this,dock3,"deleteTrack");

    addToolButtonGrid(this,dock3,"deleteTrack");
    addToolButtonGrid(this,dock3,"marker");
    addToolButtonGrid(this,dock3,"44","signs");


    addToolButtonGrid(this,dock3,"repBegin","|:",true);
    addToolButtonGrid(this,dock3,"repEnd",":|",true);
    addToolButtonGrid(this,dock3,"alt","",true);
    addToolButtonGrid(this,dock3,"bpm","",true);
    addToolButtonGrid(this,dock3,"instr","",true);
    addToolButtonGrid(this,dock3,"volume","",true);
    addToolButtonGrid(this,dock3,"pan","",true);
    addToolButtonGrid(this,dock3,"name","",true);
    addToolButtonGrid(this,dock3,"tune","",true);

    addToolButtonGrid(this,dock3,"mute","",true);
    addToolButtonGrid(this,dock3,"solo","",true);


    dock3->setGeometry(90,290,600,110);
    dock3->show();


    QDockWidget *dock4 = new QDockWidget(this,Qt::Window | Qt::WindowStaysOnTopHint |
                                         Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint);

    QLabel *labelTitle4 = new QLabel("effects",dock4);
    dock4->setTitleBarWidget(labelTitle4);

    dock4->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable
                      |QDockWidget::DockWidgetVerticalTitleBar);

    dock4->setAllowedAreas(Qt::NoDockWidgetArea);

    addToolButtonGrid(0,0,"",""); //new dock

    addToolButtonGrid(this,dock4,"vib",CONF_PARAM("effects.vibrato")); //must config it
    addToolButtonGrid(this,dock4,"sli",CONF_PARAM("effects.slide"));
    addToolButtonGrid(this,dock4,"ham",CONF_PARAM("effects.hammer"));
    addToolButtonGrid(this,dock4,"lr",CONF_PARAM("effects.letring"));
    addToolButtonGrid(this,dock4,"pm",CONF_PARAM("effects.palmmute"));
    addToolButtonGrid(this,dock4,"harm",CONF_PARAM("effects.harmonics"));
    addToolButtonGrid(this,dock4,"trem",CONF_PARAM("effects.tremolo"));
    addToolButtonGrid(this,dock4,"trill",CONF_PARAM("effects.trill"));
    addToolButtonGrid(this,dock4,"stok",CONF_PARAM("effects.stokatto"));
    addToolButtonGrid(this,dock4,"tapp");
    addToolButtonGrid(this,dock4,"slap");
    addToolButtonGrid(this,dock4,"pop");
    addToolButtonGrid(this,dock4,"fadeIn",CONF_PARAM("effects.fadein"));

    addToolButtonGrid(this,dock4,"upm","up m",true);
    addToolButtonGrid(this,dock4,"downm","down m",true);
    addToolButtonGrid(this,dock4,"acc",CONF_PARAM("effects.accent"),true);
    addToolButtonGrid(this,dock4,"downm","down m",true);
    addToolButtonGrid(this,dock4,"acc",CONF_PARAM("effects.accent"),true);
    addToolButtonGrid(this,dock4,"hacc","h acc",true);
    addToolButtonGrid(this,dock4,"bend","",true);
    addToolButtonGrid(this,dock4,"chord","",true);
    addToolButtonGrid(this,dock4,"txt","text",true);
    addToolButtonGrid(this,dock4,"changes","",true);
    addToolButtonGrid(this,dock4,"upstroke","",true);
    addToolButtonGrid(this,dock4,"downstroke","",true);


    this->addDockWidget(Qt::RightDockWidgetArea ,  dock4);


    dock->hide();
    dock2->hide();
    dock3->hide();
    dock4->hide();

    this->dock2 = dock2;
    this->dock3 = dock3;
    this->dock4 = dock4;

    //DOCKs are hiden now
    //dock = dock2 = 0;
}
