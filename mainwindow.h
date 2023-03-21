#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets>
#include <QFileDialog>
#include <QMessageBox>

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
};

#endif // MAINWINDOW_H
