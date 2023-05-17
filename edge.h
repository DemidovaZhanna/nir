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
         QString w,
         QStringList outW,
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
    QString getWeight();
    QStringList getOutWeight();
    Direction getDirection();
    void setSource(Node *s);
    void setDest(Node *d);
    void setWeight(QString w);
    void setOutWeight(QStringList outW);
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
    QString weight;
    QStringList outWeight;
    Direction direction;
    bool selected;
    GraphicWindow *graphic;

    std::vector<QString> _descrOut = {"<none>", "N_CONNECT.REQ", "N_DISCONNECT.REQ", "T_DISCONNECT.IND",
                                      "T_CONNECT.CONF", "N_DATA.REQ", "T_CONNECT.IND",
                                      "N_CONNECT.RESP", "T_DATA.IND", "N_DATA.REQ (disconnect)"};
};

#endif // EDGE_H
