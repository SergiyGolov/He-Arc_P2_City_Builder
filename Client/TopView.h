#ifndef TOPVIEW_H
#define TOPVIEW_H

#include <QGraphicsView>
class QGraphicsScene;
class QLabel;
class QPushButton;
class QDoubleSpinBox;

class TopView: public QGraphicsView
{
    Q_OBJECT

public:
    ~TopView();
     static TopView* getTopView();
     void update();
private:
    TopView(QWidget *parent = 0);
    static TopView* topViewInstance;

    QGraphicsScene *scene;

    int screenWidth;
    int screenHeight;

    QLabel *money;
    QLabel *moneyDelta;
    QLabel *population;
    QLabel *populationDelta;
    QLabel *happiness;
    QLabel *happinessDelta;
    QLabel *labelTaxes;
    QDoubleSpinBox *taxes;
    QPushButton *quit;

    QString deltaFormat(double delta);

    void setMoney(int money);
    void setHappiness(int happiness);
    void setPopulation(int population);

    void setMoneyDelta(double delta);
    void setHappinessDelta(double delta);
    void setPopulationDelta(int delta);
protected:
    void keyPressEvent(QKeyEvent *event);
private slots:
    void taxesChanged();
};

#endif // TOPVIEW_H
