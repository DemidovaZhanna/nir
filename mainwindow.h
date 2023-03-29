#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>

#include "graphicwindow.h"

//enum _WeightIn
//{
//    T_CONNECT_REQ = 0,
//    RECONNECT = 1,
//    STOP_TRY_RECONNECT = 2,
//    N_CONNECT_CONF = 3,
//    T_DATA_REQ = 4,
//    RESEND = 5,
//    STOP_TRY_RESEND = 6,
//    N_DATA_IND_dis = 7,
//    N_CONNECT_IND = 8,
//    T_CONNECT_RESP = 9,
//    N_DATA_IND = 10,
//    T_DISCONNECT_REQ = 11,
//    REDISCONNECT = 12,
//    N_DISCONNECT_IND = 13,
//    STOP_TRY_REDISCONNECT = 14
//};

class QMenu;
class QAction;
class QLabel;
class QToolBar;
class QPushButton;
class QToolButton;
class QLineEdit;
class QComboBox;
class QColorDialog;
class QListWidget;
class QListWidgetItem;
class QGroupBox;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void showEditNode();
    void showEditEdge();
    void showEdgeTable();
    void hideEdgeTable();

    void addNode(Node *n);
    void addEdge(Edge *e);

    void removeEdge(Edge *e);
    void setActiveNode(Node *n);
    void setActiveEdge(Edge *e);

private slots:
    void newFile();
    void open();
    void save();
    void translate();

    void hideEditNode();
    void setNodeName();
    void setNodeWeight();
    void setNodeDesc();
    void setNodeColor();

    void hideEditEdge();
    void setEdgeWeight();
    void setEdgeSource();
    void setEdgeDest();
    void setEdgeDirection();

    void setEdgeDescrIn();
    void setEdgeDescrOut();

    void showSelector();
    void hideSelector();
    void addNode();
    void addEdge();
    void removeNode();
    void removeEdge();
    void nodeItemClicked(QListWidgetItem *item);
    void edgeItemClicked(QListWidgetItem *item);

private:
    void createActions();
    void createMenus();
    void createToolbar();
    void createEditNode();
    void createEditEdge();
    void createSelector();
    void createEdgeTable();
    void deleteEdgeTable();
    void createGraphicWindow(QString directory);
    void clearAll();

    Node* activeNode;
    Edge* activeEdge;
    GraphicWindow *graphic;

    /*file menu*/
    QMenu *fileMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *translateAct;
    QAction *exitAct;

    /*node editor*/
    QToolBar *nodeEdit;
    QLabel *nodeNameLabel;
    QLineEdit *nodeNameLine;
    QPushButton *nodeNameSet;
    QLabel *nodeWeightLabel;
    QLineEdit *nodeWeightLine;
    QPushButton *nodeWeightSet;
    QLabel *nodeDescLabel;
    QLineEdit *nodeDescLine;
    QPushButton *nodeDescSet;
    QPushButton *closeNodeEdit;

    /*edge editor*/
    QToolBar *edgeEdit;
    QLineEdit *edgeWeightLine;
    QLineEdit *edgeOutWLine;
    QPushButton *edgeSourceSet;
    QPushButton *edgeDestSet;
    QPushButton *edgeDirSet;
    QPushButton *edgeWeightSet;
    QPushButton *closeEdgeEdit;
    QComboBox *sourceNodes2;
    QComboBox *destNodes2;
    QComboBox *dir2;

    /*selector */
    QToolBar *selector;
    QListWidget *nodesTable;
    QListWidget *edgesTable;
    //edgeTable
    QToolBar *edgeTable;
    QComboBox *labelweight;
    QTableWidget *markerTable;
    QPushButton *edgenewWeightSet;
    QPushButton *edgenewWOutSet;
    //node add panel
    QGroupBox *nodeAddPanel;
    QLineEdit *addNodeName;
    QLineEdit *addNodeWeight;
    QLineEdit *addNodeDesc;
    QPushButton *newNodeButton;
    QPushButton *removeNodeButton;
    //edge add panel
    QGroupBox *edgeAddPanel;
    QComboBox *sourceNodes;
    QComboBox *destNodes;
    QComboBox *directionOfEdge;
    QLineEdit *addEdgeWeight;
    QLineEdit *addEdgeOutW;
    QPushButton *newEdgeButton;
    QPushButton *removeEdgeButton;

    std::map<QString, int> WeightIn;
    std::map<QString, int> WeightOut;

//    std::vector<int> labelWeight;

    std::vector<QString> _descrIn = {"T_CONNECT.REQ", "RECONNECT", "STOP_TRY_RECONNECT",
                                     "N_CONNECT.CONF", "T_DATA.REQ", "RESEND",
                                     "STOP_TRY_RESEND", "N_DATA.IND (disconnect)", "N_CONNECT.IND",
                                     "T_CONNECT.RESP", "N_DATA.IND", "T_DISCONNECT.REQ",
                                     "REDISCONNECT", "N_DISCONNECT.IND", "STOP_TRY_REDISCONNECT"};

    std::vector<QString> _descrOut = {"N_CONNECT.REQ", "N_DISCONNECT.REQ", "T_DISCONNECT.IND",
                                     "T_CONNECT.CONF", "N_DATA.REQ", "T_CONNECT.IND",
                                     "N_CONNECT.RESP", "T_DATA.IND", "N_DATA.REQ (disconnect)"};
};

#endif // MAINWINDOW_H
