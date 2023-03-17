#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QMetaEnum>

class Node;
class GraphicWindow;

enum Direction{
    SOURCE_TO_DEST = 0,
    DEST_TO_SOURCE = 1,
    TWO_WAY = 2
};

class Edge : public QGraphicsItem
{
public:
    Edge(Node *getSourceNode,
         Node *getDestNode,
         int w,
         int outW,
         Direction d,
         GraphicWindow *graphicWindow);
    ~Edge();

    void removeThis();           
    void select();
    void unselect();
    void adjust();

    /*getters & setters*/
    Node *getSourceNode() const;
    Node *getDestNode() const;
    QString getSource();
    QString getDest();
    int getWeight();
    int getOutWeight();
    Direction getDirection();
    void setSource(Node *s);
    void setDest(Node *d);
    void setWeight(int w);
    void setOutWeight(int outW);
    void setDirection(Direction d);

protected:
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) Q_DECL_OVERRIDE;

private:
    Node *source;
    Node *dest;
    QString sourceName;
    QString destName;
    QPointF sourcePoint;
    QPointF destPoint;
    int weight;
    int outWeight;
    Direction direction;
    bool selected;
    GraphicWindow *graphic;
};

#endif // EDGE_H
