#include "edge.h"
#include "node.h"
#include "graphicwindow.h"
#include "graphmanager.h"
#include "mainwindow.h"

void error(QString text)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error!");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

MainWindow::~MainWindow(){}

MainWindow::MainWindow()
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    setWindowTitle(tr("Graph Editor"));
    setMinimumSize(600, 480);
    showMaximized();
    update();

    createActions();
    createMenus();
    createSelector();
    createEditNode();
    createEditEdge();
}

/*file->new*/
void MainWindow::newFile()
{
    createGraphicWindow("");
}

/*file->open*/
void MainWindow::open()
{
    QString directory = QFileDialog::getOpenFileName(this,tr("Open.."),"/home/zhanna/temp",tr("Graph files(*.graph)"));
    if(directory.isEmpty())
        return;

    createGraphicWindow(directory);
}

/*create graphic view (empty or with loaded graph)*/
void MainWindow::createGraphicWindow(QString directory)
{
    clearAll();

    QList<GraphicWindow *> widgets = findChildren<GraphicWindow *>();
    foreach(GraphicWindow * widget, widgets)
    {
        delete widget;
    }

    if(directory=="")
    {
        graphic = new GraphicWindow(this,this,0);
    }
    else
    {
        graphic = new GraphicWindow(this,this,directory);
    }
    this->setCentralWidget(graphic);

    showSelector();
}

/*file->save*/
void MainWindow::save()
{
    QString directory = QFileDialog::getSaveFileName(this,tr("Save.."),"/home/zhanna/temp",tr("Graph files(*.graph)"));
    if(directory.isEmpty())
        return;

    QFile file(directory);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        error("Problems opening a file");
        return;
    }

    QTextStream out(&file);
    GraphManager *graph=graphic->getMngr();

    out<<"NODES\n";
    foreach(Node *n,graph->getNodes()){
        out<<n->getName()<<" "
           <<QString::number(n->getWeight())<<"\n";
    }
    out<<"EDGES\n";
    foreach(Edge *e,graph->getEdges()){
        out<<e->getSource()<<" "
           <<e->getDest()<<" "
           <<QString::number(e->getWeight())<<" "
           <<QString::number(static_cast<int>(e->getDirection()))<<"\n";
    }

}

/*connect menu buttons with actions*/
void MainWindow::createActions()
{
    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);

    saveAct = new QAction(tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, &QAction::triggered, this, &QWidget::close);
}

/*create file menu*/
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
}

/*clear all lists, line edits etc. */
void MainWindow::clearAll()
{
    activeNode=NULL;
    activeEdge=NULL;

    if(nodeNameLine->text() != "")
        nodeNameLine->clear();

    if(nodeWeightLine->text() != "")
        nodeWeightLine->clear();

    if(nodeDescLine->text() != "")
        nodeDescLine->clear();


    if(addNodeName->text() != "")
        addNodeName->clear();

    if(addNodeWeight->text() != "")
        addNodeWeight->clear();

    if(addNodeDesc->text() != "")
        addNodeDesc->clear();

    if(nodesTable->count() > 0)
        nodesTable->clear();

    if(edgesTable->count() > 0)
        edgesTable->clear();

    if(sourceNodes->count() > 0)
        sourceNodes->clear();

    if(destNodes->count() > 0)
        destNodes->clear();
}

/*create node editor*/
void MainWindow::createEditNode()
{
    nodeEdit=new QToolBar(tr("Node edit"),this);
    this->addToolBar(Qt::LeftToolBarArea,nodeEdit);
    nodeEdit->setMovable(false);
    nodeEdit->addWidget(new QLabel("\nNode editor"));
    nodeEdit->addSeparator();

    nodeNameLabel=new QLabel("Name: ");
    nodeEdit->addWidget(nodeNameLabel);
    nodeNameLine=new QLineEdit();
    nodeEdit->addWidget(nodeNameLine);
    nodeNameSet=new QPushButton("Set",this);
    connect(nodeNameSet, SIGNAL (released()), this, SLOT(setNodeName()));
    nodeEdit->addWidget(nodeNameSet);
    nodeEdit->addSeparator();

    nodeWeightLabel=new QLabel("State: ");
    nodeEdit->addWidget(nodeWeightLabel);
    nodeWeightLine=new QLineEdit();
    nodeWeightLine->setValidator( new QIntValidator(0,100));
    nodeEdit->addWidget(nodeWeightLine);
    nodeWeightSet=new QPushButton("Set",this);
    connect(nodeWeightSet, SIGNAL (released()), this, SLOT(setNodeWeight()));
    nodeEdit->addWidget(nodeWeightSet);
    nodeEdit->addSeparator();

    nodeDescLabel=new QLabel("Description: ");
    nodeEdit->addWidget(nodeDescLabel);
    nodeDescLine=new QLineEdit();
    nodeEdit->addWidget(nodeDescLine);
    nodeDescSet=new QPushButton("Set",this);
    connect(nodeDescSet, SIGNAL (released()), this, SLOT(setNodeDesc()));
    nodeEdit->addWidget(nodeDescSet);
    nodeEdit->addSeparator();

    closeNodeEdit=new QPushButton("Close editor",this);
    connect(closeNodeEdit, SIGNAL (released()), this, SLOT(hideEditNode()));
    nodeEdit->addWidget(closeNodeEdit);
    hideEditNode();
}

/*hide node editor*/
void MainWindow::hideEditNode(){
    nodeEdit->setVisible(false);
    removeNodeButton->setEnabled(false);
}

/*show node editor*/
void MainWindow::showEditNode(){
    hideEditEdge();
    nodeEdit->setVisible(true);
    nodeNameLine->setText(activeNode->getName());
    nodeWeightLine->setText(QString::number(activeNode->getWeight()));
    nodeDescLine->setText(activeNode->getDesc());
    removeNodeButton->setEnabled(true);
}

/*set new name for selected node*/
void MainWindow::setNodeName()
{
    if(sourceNodes->findText(nodeNameLine->text())==-1)
    {
        QString oldName=activeNode->getName();
        nodesTable->currentItem()->setText(" Number: "+nodeNameLine->text()+" State: "+QString::number(activeNode->getWeight()));
        activeNode->setName(nodeNameLine->text());

        int pos=sourceNodes->findText(oldName);
        sourceNodes->setItemText(pos,activeNode->getName());
        sourceNodes->update();

        pos=destNodes->findText(oldName);
        destNodes->setItemText(pos,activeNode->getName());
        destNodes->update();

        pos=sourceNodes2->findText(oldName);
        sourceNodes2->setItemText(pos,activeNode->getName());
        sourceNodes2->update();

        pos=destNodes2->findText(oldName);
        destNodes2->setItemText(pos,activeNode->getName());
        destNodes2->update();

        for(int i = 0; i < edgesTable->count(); i++)
        {
            QListWidgetItem *item=edgesTable->item(i);
            QString text=item->text();
            text.replace(" "+oldName+" "," "+activeNode->getName()+" ");
            item->setText(text);
        }

    }
    else if(nodeNameLine->text()=="")
    {
        error("Type number first!");
    }
    else
    {
        error("There already is node with that number!");
    }
}

/*set new weight for selected node*/
void MainWindow::setNodeWeight()
{
    if(nodeWeightLine->text()!="")
    {
        nodesTable->currentItem()->setText(" Number: "+activeNode->getName()+" State: "+nodeWeightLine->text());
        activeNode->setWeight(nodeWeightLine->text().toInt());
    }
    else
        error("Type state first!");
}

/*set new description for selected node*/
void MainWindow::setNodeDesc()
{
    if(nodeDescLine->text()!="")
        activeNode->setDesc(nodeDescLine->text());

    else
        error("Type decription first!");
}

/*set new color for selected node*/
void MainWindow::setNodeColor()
{
    QColorDialog *coldiag = new QColorDialog(activeNode->getColor());
    activeNode->setColor(coldiag->getColor());
    delete coldiag;
}

/*select node*/
void MainWindow::setActiveNode(Node *n)
{
    activeNode=n;
}

/*create edge editor*/
void MainWindow::createEditEdge()
{
    edgeEdit=new QToolBar(tr("Edge edit"),this);
    this->addToolBar(Qt::LeftToolBarArea,edgeEdit);
    edgeEdit->setMovable(false);
    edgeEdit->addWidget(new QLabel("\nEdge editor"));
    edgeEdit->addSeparator();

    edgeSourceSet=new QPushButton("Set");
    edgeDestSet=new QPushButton("Set");
    edgeDirSet=new QPushButton("Set");
    edgeWeightSet=new QPushButton("Set");
    closeEdgeEdit=new QPushButton("Close editor");

    sourceNodes2 = new QComboBox();
    destNodes2 = new QComboBox();
    dir2 = new QComboBox();
    dir2->insertItem(0,QString("SOURCE_TO_DEST"));
    dir2->insertItem(1,QString("DEST_TO_SOURCE"));
    dir2->insertItem(2,QString("TWO_WAY"));

    connect(edgeSourceSet, SIGNAL (released()), this, SLOT(setEdgeSource()));
    connect(edgeDestSet, SIGNAL (released()), this, SLOT(setEdgeDest()));
    connect(edgeDirSet, SIGNAL (released()), this, SLOT(setEdgeDirection()));
    connect(edgeWeightSet, SIGNAL (released()), this, SLOT(setEdgeWeight()));
    connect(closeEdgeEdit, SIGNAL (released()), this, SLOT(hideEditEdge()));

    edgeEdit->addWidget(new QLabel("Source: "));
    edgeEdit->addWidget(sourceNodes2);
    edgeEdit->addWidget(edgeSourceSet);
    edgeEdit->addSeparator();

    edgeEdit->addWidget(new QLabel("Destination: "));
    edgeEdit->addWidget(destNodes2);
    edgeEdit->addWidget(edgeDestSet);
    edgeEdit->addSeparator();

    edgeEdit->addWidget(new QLabel("Direction: "));
    edgeEdit->addWidget(dir2);
    edgeEdit->addWidget(edgeDirSet);
    edgeEdit->addSeparator();

    edgeEdit->addWidget(new QLabel("Marker: "));
    edgeWeightLine=new QLineEdit();
    edgeWeightLine->setValidator( new QIntValidator(0,100));
    edgeEdit->addWidget(edgeWeightLine);
    edgeEdit->addWidget(edgeWeightSet);
    edgeEdit->addSeparator();

    edgeEdit->addWidget(closeEdgeEdit);

    hideEditEdge();
}

/*show edge editor*/
void MainWindow::showEditEdge()
{
    hideEditNode();

    edgeEdit->setVisible(true);

    edgeWeightLine->setText(QString::number(activeEdge->getWeight()));

    int sourcepos=sourceNodes2->findText(activeEdge->getSource());
    int destpos=destNodes2->findText(activeEdge->getDest());
    int dirpos=activeEdge->getDirection();

    sourceNodes2->setCurrentIndex(sourcepos);
    destNodes2->setCurrentIndex(destpos);
    dir2->setCurrentIndex(dirpos);

    removeEdgeButton->setEnabled(true);
}

/*hide edge editor*/
void MainWindow::hideEditEdge()
{
    edgeEdit->setVisible(false);
    removeEdgeButton->setEnabled(false);
}

/*set new weight for selected edge*/
void MainWindow::setEdgeWeight()
{
    if(edgeWeightLine->text()!="")
    {
        QString item=" "+activeEdge->getSource();
        if(activeEdge->getDirection()==SOURCE_TO_DEST)
        {
            item+=" -> ";
        }
        else if(activeEdge->getDirection()==DEST_TO_SOURCE)
        {
            item+=" <- ";
        }
        else if(activeEdge->getDirection()==TWO_WAY)
        {
            item+=" <-> ";
        }
        item+=activeEdge->getDest()+" Marker: "+edgeWeightLine->text();

        edgesTable->currentItem()->setText(item);
        activeEdge->setWeight(edgeWeightLine->text().toInt());
    }
    else
        error("Type marker first!");
}

/*set new source for selected edge*/
void MainWindow::setEdgeSource()
{
    for(int i=0;i<edgesTable->count();i++)
    {
        QString text=edgesTable->item(i)->text();
        QStringList list = text.split(" ");
        QString source = list.at(1);
        QString dest = list.at(3);
        if(source==sourceNodes2->currentText()&&destNodes2->currentText()==dest)
        {
            error("There already is edge between selected nodes!");
            return;
        }
        if(dest==sourceNodes2->currentText()&&destNodes2->currentText()==source)
        {
            error("There already is edge between selected nodes!");
            return;
        }
    }

    Node *dest=activeEdge->getDestNode();
    int weight=activeEdge->getWeight();
    Direction dir=activeEdge->getDirection();

    QListWidgetItem* item = edgesTable->currentItem();
    delete item;

    delete activeEdge;

    graphic->getMngr()->addItem(new Edge(graphic->getMngr()->getNodeByName(sourceNodes2->currentText()),dest,weight,dir,graphic));
    graphic->scene()->addItem(graphic->getMngr()->getLastEdge());
    activeEdge=graphic->getMngr()->getLastEdge();
}

/*set new destination for selected edge*/
void MainWindow::setEdgeDest()
{
    for(int i=0;i<edgesTable->count();i++){

        QString text=edgesTable->item(i)->text();
        QStringList list = text.split(" ");
        QString source = list.at(1);
        QString dest = list.at(3);
        if(source==sourceNodes2->currentText()&&destNodes2->currentText()==dest)
        {
            error("There already is edge between selected nodes!");
            return;
        }
        if(dest==sourceNodes2->currentText()&&destNodes2->currentText()==source)
        {
            error("There already is edge between selected nodes!");
            return;
        }
    }

    Node *source=activeEdge->getSourceNode();
    int weight=activeEdge->getWeight();
    Direction dir=activeEdge->getDirection();

    QListWidgetItem* item = edgesTable->currentItem();
    delete item;

    delete activeEdge;

    graphic->getMngr()->addItem(new Edge(source,graphic->getMngr()->getNodeByName(destNodes2->currentText()),weight,dir,graphic));
    graphic->scene()->addItem(graphic->getMngr()->getLastEdge());
    activeEdge=graphic->getMngr()->getLastEdge();
}

/*set new direction for selected edge*/
void MainWindow::setEdgeDirection()
{
    activeEdge->setDirection(static_cast<Direction>(dir2->currentIndex()));
}

/*select edge*/
void MainWindow::setActiveEdge(Edge *e)
{
    activeEdge=e;
}

/*create selector (for selecting node or edge and adding node or edge)*/
void MainWindow::createSelector()
{
    selector=new QToolBar(tr("Selector"),this);
    this->addToolBar(Qt::RightToolBarArea,selector);
    selector->setMovable(false);
    selector->addSeparator();

    selector->addWidget(new QLabel("Nodes:"));
    nodesTable=new QListWidget();
    nodesTable->setMaximumWidth(250);
    nodesTable->setMaximumHeight(250);
    connect(nodesTable, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(nodeItemClicked(QListWidgetItem *)));
    selector->addWidget(nodesTable);
    removeNodeButton=new QPushButton("Remove selected node");
    connect(removeNodeButton,SIGNAL(released()),this,SLOT(removeNode()));
    selector->addWidget(removeNodeButton);
    selector->addSeparator();

    nodeAddPanel=new QGroupBox();
    QVBoxLayout *layout=new QVBoxLayout();

    layout->addWidget(new QLabel("Name: "));
    addNodeName=new QLineEdit();
    layout->addWidget(addNodeName);
    layout->addWidget(new QLabel("State: "));
    addNodeWeight=new QLineEdit();
    addNodeWeight->setValidator( new QIntValidator(0,100));
    layout->addWidget(addNodeWeight);
    layout->addWidget(new QLabel("Description: "));
    addNodeDesc=new QLineEdit();
    layout->addWidget(addNodeDesc);
    newNodeButton=new QPushButton("Add new Node..");
    connect(newNodeButton, SIGNAL (released()), this, SLOT(addNode()));
    layout->addWidget(newNodeButton);
    nodeAddPanel->setLayout(layout);
    selector->addWidget(nodeAddPanel);
    selector->addSeparator();

    selector->addWidget(new QLabel("\nEdges:"));
    edgesTable=new QListWidget();;
    edgesTable->setMaximumWidth(250);
    edgesTable->setMaximumHeight(250);
    connect(edgesTable, SIGNAL(itemPressed(QListWidgetItem *)), this, SLOT(edgeItemClicked(QListWidgetItem *)));
    selector->addWidget(edgesTable);
    removeEdgeButton=new QPushButton("Remove selected edge");
    connect(removeEdgeButton,SIGNAL(released()),this,SLOT(removeEdge()));
    selector->addWidget(removeEdgeButton);
    selector->addSeparator();

    edgeAddPanel=new QGroupBox();
    QVBoxLayout *layout2=new QVBoxLayout();
    layout2->addWidget(new QLabel("Source: "));
    sourceNodes=new QComboBox();
    layout2->addWidget(sourceNodes);
    layout2->addWidget(new QLabel("Destination: "));
    destNodes=new QComboBox();
    layout2->addWidget(destNodes);
    layout2->addWidget(new QLabel("Direction: "));
    directionOfEdge=new QComboBox();
    directionOfEdge->insertItem(0,QString("SOURCE_TO_DEST"));
    directionOfEdge->insertItem(1,QString("DEST_TO_SOURCE"));
    directionOfEdge->insertItem(2,QString("TWO_WAY"));
    layout2->addWidget(directionOfEdge);
    layout2->addWidget(new QLabel("Marker: "));
    addEdgeWeight=new QLineEdit();
    addEdgeWeight->setValidator( new QIntValidator(0,100));
    layout2->addWidget(addEdgeWeight);
    newEdgeButton=new QPushButton("Add new edge..");
    connect(newEdgeButton, SIGNAL (released()), this, SLOT(addEdge()));
    layout2->addWidget(newEdgeButton);
    edgeAddPanel->setLayout(layout2);
    selector->addWidget(edgeAddPanel);

    selector->addSeparator();

    removeNodeButton->setEnabled(false);
    removeEdgeButton->setEnabled(false);

    hideSelector();
}

/*show selector*/
void MainWindow::showSelector()
{
    //if(openSelector->isChecked())
        selector->setVisible(true);
    //else
        //hideSelector();
}

/*hide selector */
void MainWindow::hideSelector()
{
    selector->setVisible(false);
}

/*add new node to graph (from panel)*/
void MainWindow::addNode()
{
    if(sourceNodes->findText(addNodeName->text())==-1 && addNodeWeight->text()!="" && addNodeName->text()!="")
        graphic->addNode(addNodeName->text(),addNodeWeight->text().toInt(), addNodeDesc->text());
    else if(addNodeWeight->text()=="")
        error("Type weight first!");
    else if(addNodeName->text()=="")
        error("Type name first!");
    else if(addNodeDesc->text()=="")
        error("Type description first!");

    else
        error("There already is node with that name!");
}

/*add new edge to graph (from panel)*/
void MainWindow::addEdge()
{
    if(addEdgeWeight->text()=="")
    {
        error("Type marker first!");
    }
    else
    {
        for(int i=0;i<edgesTable->count();i++)
        {
            QString text=edgesTable->item(i)->text();
            QStringList list = text.split(" ");
            QString source = list.at(1);
            QString dest = list.at(3);
            if(source==sourceNodes->currentText()&&destNodes->currentText()==dest)
            {
                error("There already is edge between selected nodes!");
                return;
            }
            if(dest==sourceNodes->currentText()&&destNodes->currentText()==source)
            {
                error("There already is edge between selected nodes!");
                return;
            }
        }

        Node *s=graphic->getMngr()->getNodeByName(sourceNodes->currentText());
        Node *d=graphic->getMngr()->getNodeByName(destNodes->currentText());
        Direction dir=static_cast<Direction>(directionOfEdge->currentText().toInt());
        int weight=addEdgeWeight->text().toInt();
        graphic->addEdge(s,d,weight,dir);
    }
}

/*add new node to nodes list (when graph is loaded from file)*/
void MainWindow::addNode(Node *n)
{
    QString item=" Name: " +n->getName()+" State: "+QString::number(n->getWeight());

    nodesTable->addItem(new QListWidgetItem(item));
    sourceNodes->addItem(QString(n->getName()));
    destNodes->addItem(QString(n->getName()));
    sourceNodes2->addItem(QString(n->getName()));
    destNodes2->addItem(QString(n->getName()));
}

/*add new edge to edges list (when graph is loaded from file)*/
void MainWindow::addEdge(Edge *e)
{
    QString item=" "+e->getSource();
    if(e->getDirection()==SOURCE_TO_DEST)
    {
        item+=" -> ";
    }
    else if(e->getDirection()==DEST_TO_SOURCE)
    {
        item+=" <- ";
    }
    else if(e->getDirection()==TWO_WAY)
    {
        item+=" <-> ";
    }
    item+=e->getDest()+" Marker: "+QString::number(e->getWeight());
    edgesTable->addItem(new QListWidgetItem(item));
}

/*remove node*/
void MainWindow::removeNode()
{
    QString name=nodesTable->currentItem()->text().split(" ").at(2);
    delete nodesTable->currentItem();

    int pos=sourceNodes->findText(name);
    sourceNodes->removeItem(pos);

    pos=destNodes->findText(name);
    destNodes->removeItem(pos);

    pos=sourceNodes2->findText(name);
    sourceNodes2->removeItem(pos);

    pos=destNodes2->findText(name);
    destNodes2->removeItem(pos);

    activeNode->removeThis();
    hideEditNode();
}

/*remove edge*/
void MainWindow::removeEdge()
{
    delete edgesTable->currentItem();
    activeEdge->removeThis();
    hideEditEdge();
}

/*remove edge*/
void MainWindow::removeEdge(Edge *e)
{
    QString sourceNode=e->getSource();
    QString destNode=e->getDest();

    for(int i=0;i<edgesTable->count();i++)
    {
        QString text=edgesTable->item(i)->text();
        QStringList list = text.split(" ");
        QString source = list.at(1);
        QString dest = list.at(3);
        if(source==sourceNode && destNode==dest)
        {
            delete edgesTable->item(i);
            continue;
        }
        if(dest==sourceNode && destNode==source)
        {
            delete edgesTable->item(i);
            continue;
        }
    }
}

/*select active node*/
void MainWindow::nodeItemClicked(QListWidgetItem *item)
{
    GraphicWindow *poleZGrafem = this->findChild<GraphicWindow *>();
    GraphManager *graph = poleZGrafem->getMngr();

    QStringList list = item->text().split(" ");
    QString name = list.at(2);

    activeNode=graph->getNodeByName(name);
    activeNode->selectThis();
    activeNode->update();
}

/*select active edge*/
void MainWindow::edgeItemClicked(QListWidgetItem *item)
{
    GraphicWindow *poleZGrafem = this->findChild<GraphicWindow *>();
    GraphManager *graph = poleZGrafem->getMngr();

    QStringList list = item->text().split(" ");
    QString source = list.at(1);
    QString dest = list.at(3);

    activeEdge=graph->getEdgeBySourceDest(source,dest);
    activeEdge->select();
    activeEdge->update();
}

