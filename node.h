#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

class Edge;
class GraphicWindow;

class Node : public QGraphicsItem{
public:
    Node(GraphicWindow *graphWidget, QString n, int w, QString d);
    ~Node();
    void removeThis();
    void addEdge(Edge *edge);
    void removeEdge(Edge *edge);
    void calculateForces();
    bool advance();
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    QPainterPath shape() const Q_DECL_OVERRIDE;
    void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) Q_DECL_OVERRIDE;

    //getery
    QList<Edge *> edges() const;
    QString getName();
    int getWeight();
    QString getDesc();
    QColor getColor();

    //setery
    void setName(QString n);
    void setWeight(int w);
    void setDesc(QString d);
    void setColor(QColor c);

    void selectThis();
    void unselect();

protected:
    QVariant itemChange(
            GraphicsItemChange change,
            const QVariant &value) Q_DECL_OVERRIDE;

private:
    QPointF newPos;
    GraphicWindow *graphic;

    QList<Edge *> edgeList;
    QString name;
    int weight;
    QString desc;
    QColor color;
    bool selected=false;
};

#endif // NODE_H
