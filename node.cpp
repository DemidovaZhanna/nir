#include "edge.h"
#include "node.h"
#include "graphicwindow.h"
#include "graphmanager.h"
#include "mainwindow.h"

/*set new node*/
Node::Node(GraphicWindow *GraphicWindow, QString n, int w, QString d)
    : graphic(GraphicWindow), name(n), weight(w), desc(d)
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);

    graphic->newNode(this);
}

/**/
Node::~Node(){}

/*remove this node from view, from nodes list and graph manager*/
void Node::removeThis()
{
    graphic->scene()->removeItem(this);
    graphic->getMngr()->removeItem(this);
    foreach(Edge *e,edgeList)
    {
        graphic->getWndw()->removeEdge(e);
        graphic->getMngr()->removeItem(e);
        graphic->scene()->removeItem(e);
        removeEdge(e);
    }
}

/*add new edge to list of edges for this node*/
void Node::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

/*remove edge from list of edges for this node*/
void Node::removeEdge(Edge *edge)
{
    edgeList.removeOne(edge);
    edge->adjust();
}

/*calculate forces for drag & drop*/
void Node::calculateForces()
{
    QPointF newPos;

    if (!scene() || scene()->mouseGrabberItem() == this)
    {
        newPos = pos();
        return;
    }

    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items())
    {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0)
        {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    double weight = (edgeList.size() + 1) * 10;
    foreach (Edge *edge, edgeList)
    {
        QPointF vec;
        if (edge->getSourceNode() == this)
            vec = mapToItem(edge->getDestNode(), 0, 0);
        else
            vec = mapToItem(edge->getSourceNode(), 0, 0);
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
        xvel = yvel = 0;

    QRectF sceneRect = scene()->sceneRect();
    newPos = pos() + QPointF(xvel, yvel);
    newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));
    newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));
}

/*return true if node changed position*/
bool Node::advance()
{
    if (newPos == pos())
        return false;

    setPos(newPos);
    return true;
}

/*return bounding rectangle*/
QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -20 - adjust, -20 - adjust, 130 + adjust, 35 + adjust);
//    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

/*return painter path*/
QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

/*paint node*/
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QRadialGradient gradient(-3, -3, 10);
    gradient.setColorAt(0,QColor(200,200,200,200));
    gradient.setColorAt(0,QColor(200,200,200,0));
    painter->setPen(Qt::NoPen);
    painter->setBrush(gradient);
    painter->drawEllipse(-7, -7, 20, 20);

    if(selected)
        gradient.setColorAt(0, Qt::black);

    painter->setBrush(gradient);

    painter->setPen(QPen(QColor(200,200,200,255), 0));
    painter->drawEllipse(-10, -10, 20, 20);

    QRectF textRect(-10, -10, 20, 20);
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(6);
    painter->setFont(font);
    if(!selected)
        painter->setPen(Qt::black);
    else
        painter->setPen(Qt::white);
    painter->drawText(textRect,Qt::AlignCenter, name + ":" + QString::number(weight));

    update();
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change)
    {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

/*return list of edges for this node*/
QList<Edge *> Node::edges() const
{
    return edgeList;
}

/*return node name*/
QString Node::getName()
{
    return name;
}

/*return node weight*/
int Node::getWeight()
{
    return weight;
}

/*return node description*/
QString Node::getDesc()
{
    return desc;
}

/*return node color*/
QColor Node::getColor()
{
    return color;
}

/*set new name for node*/
void Node::setName(QString n)
{
    name=n;
    this->update();
}

/*set new weight for node*/
void Node::setWeight(int w)
{
    weight=w;
    this->update();
}

/*set new decription for node*/
void Node::setDesc(QString d)
{
    desc = d;
    this->update();
}

/*set new color for node*/
void Node::setColor(QColor c)
{
    color=c;
    this->update();
}

/*select node*/
void Node::selectThis()
{
    graphic->unselectAll();
    graphic->showEditNode(this);
    selected=true;
    update();
}

/*unselect node */
void Node::unselect()
{
    selected=false;
    update();
}

