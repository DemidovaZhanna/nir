#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>
#include <unordered_map>

#include "graphicwindow.h"

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
    void onCellChanged(int row, int column);
    QString& eventProcess(QString& _out);

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
    QComboBox *inputMarker2;
    QComboBox *outputMarker3;
    QComboBox *outputMarker4;
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
    QComboBox *inputMarker;
    QComboBox *outputMarker;
    QComboBox *outputMarker2;
    QPushButton *newEdgeButton;
    QPushButton *removeEdgeButton;

    std::unordered_map<QString, QString> WeightIn; // формат: вх.верш + " " + вых.верш. , вх.вес
    std::unordered_map<QString, QStringList> WeightOut;

    std::vector<QString> _descrIn = {"T_CONNECT.REQ", "RECONNECT", "STOP_TRY_RECONNECT",
                                     "N_CONNECT.CONF", "T_DATA.REQ", "RESEND",
                                     "STOP_TRY_RESEND", "N_DATA.IND (disconnect)", "N_CONNECT.IND",
                                     "T_CONNECT.RESP", "N_DATA.IND", "T_DISCONNECT.REQ",
                                     "REDISCONNECT", "N_DISCONNECT.IND", "STOP_TRY_REDISCONNECT"};

    std::vector<QString> _descrOut = {"<none>", "N_CONNECT.REQ", "N_DISCONNECT.REQ", "T_DISCONNECT.IND",
                                     "T_CONNECT.CONF", "N_DATA.REQ", "T_CONNECT.IND",
                                     "N_CONNECT.RESP", "T_DATA.IND", "N_DATA.REQ (disconnect)"};

    std::vector<QString> _optionOut;
};

#endif // MAINWINDOW_H
