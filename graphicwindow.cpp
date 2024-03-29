#include "graphicwindow.h"
#include "graphmanager.h"
#include "node.h"
#include "mainwindow.h"

using namespace std;

/*create empty graphic widget and graph manager*/
GraphicWindow::GraphicWindow(QWidget *parent,MainWindow *wind,int a): QGraphicsView(parent)
{
    mngr = new GraphManager();
    wndw = wind;

    int scale=-800;

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(scale, scale, -scale, -scale);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    this->scale(qreal(1), qreal(1));
    setMinimumSize(scale/-2,scale/-2);
}

/*create graphic widget with graph loaded from file*/
GraphicWindow::GraphicWindow(QWidget *parent,MainWindow *wind,QString fileName): QGraphicsView(parent)
{
    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    mngr = new GraphManager();
    wndw = wind;

    int scale=-800;

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(scale, scale, -scale, -scale);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    this->scale(qreal(1), qreal(1));
    setMinimumSize(scale/-2,scale/-2);

    QString nextline="";
    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine();
        if(line=="NODES")
        {
            nextline="Node";
        }
        else if(line=="EDGES")
        {
            nextline="Edge";
        }
        else if(nextline=="Node")
        {
            QStringList list = line.split(" ");
            QString name = list.at(0);
            int weight = list.at(1).toInt();
            QString desc = list.at(2);

            mngr->addItem(new Node(this, name, weight, desc));
            scene->addItem(mngr->getLastNode());
        }
        else if(nextline=="Edge")
        {
            QStringList list = line.split(" ");

            Node* sourc;
            Node* des;

            foreach(Node *n, mngr->getNodes())
            {
                if (n->getName() == list.at(0))
                    sourc = n;
                if (n->getName() == list.at(1))
                    des = n;
            }

            QString waga = list.at(2);
            Direction dir = (Direction)list.at(3).toInt();
            QStringList outW;
            int k = 0;
            for (int i = 4; i < list.length(); i++)
            {
                if (list.at(i) == " ")
                    continue;
                outW[k++] = list.at(i);                 // вероятно, тут проблема с открытием сохраненного файла
            }

            mngr->addItem(new Edge(sourc, des, waga, outW, dir, this));
            scene->addItem(mngr->getLastEdge());
        }
    }

    file.close();

    srand (time(NULL));
    foreach(Node* n, mngr->getNodes())
    {
        int pom=rand()%401+200;
        int pom2=rand()%401+200;
        n->setPos(-pom,-pom2); 
    }
}

/*scale view*/
void GraphicWindow::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

/*return graph manager*/
GraphManager* GraphicWindow::getMngr()
{
    return mngr;
}

/*return main window pointer*/
MainWindow* GraphicWindow::getWndw()
{
    return wndw;
}

/*graph manager -> unselect all nodes and edges*/
void GraphicWindow::unselectAll()
{
    mngr->unselectAllNodes();
    mngr->unselectAllEdges();
}

/*main window -> set active node from parameter and show editor*/
void GraphicWindow::showEditNode(Node *n)
{
    wndw->setActiveNode(n);
    wndw->hideEdgeTable();
    wndw->showEditNode();
}

/*main window -> set active edge from parameter and show editor */
void GraphicWindow::showEditEdge(Edge *e)
{
    wndw->setActiveEdge(e);
    wndw->hideEdgeTable();
    wndw->showEditEdge();
}

/*main window -> add new node to nodes list from parameter */
void GraphicWindow::newNode(Node *n)
{
    wndw->addNode(n);
}

/*main window -> add new edge to edges list from parameter */
void GraphicWindow::newEdge(Edge *e)
{
    wndw->addEdge(e);
}

/*add new node to graphic view and graph manager*/
void GraphicWindow::addNode(QString name, int weight, QString desc)
{

    Node *newNode = new Node(this, name, weight, desc);
    mngr->addItem(newNode);
    scene()->addItem(newNode);

    //Ustalanie pozycji
    srand (time(NULL));
    int pom=rand()%401+200;
    int pom2=rand()%401+200;
    newNode->setPos(-pom,-pom2);
    newNode->update();
}

/*add new edge to graphic view and graph manager*/
void GraphicWindow::addEdge(Node *source, Node *dest, QString weight, QStringList outWeight, Direction dir)
{
    Edge *newEdge = new Edge(source, dest, weight, outWeight, dir, this);
    mngr->addItem(newEdge);
    scene()->addItem(newEdge);

    newEdge->update();
}

/*wheel event*/
void GraphicWindow::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, event->delta() / 240.0));
}


