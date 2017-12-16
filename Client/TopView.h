#ifndef TOPVIEW_H
#define TOPVIEW_H

#include <QGraphicsView>
class QGraphicsScene;
class QLabel;
class QPushButton;

class TopView: public QGraphicsView
{
    Q_OBJECT

public:

    ~TopView();
     static TopView* getTopView();
     void setMoney(int money);
     void setHappiness(int happiness);
     void setPopulation(int population);

private:
      TopView(QWidget *parent=0);
    QGraphicsScene *scene;
    static TopView* topViewInstance;
    int screenWidth;
    int screenHeight;
    QLabel *money;
    QLabel *population;
    QLabel *happiness;
    QPushButton *quit;
};

#endif // TOPVIEW_H