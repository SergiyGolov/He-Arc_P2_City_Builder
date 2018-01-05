#ifndef TOPVIEW_H
#define TOPVIEW_H

#include <QGraphicsView>
class QLabel;
class QSlider;
class QPushButton;
class QDoubleSpinBox;
class QGraphicsScene;

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
    QPushButton *save;
    QPushButton *load;
    QPushButton *quit;

    QLabel *labelMaster;
    QLabel *labelMusic;
    QLabel *labelSfx;
    QSlider *master;
    QSlider *music;
    QSlider *sfx;

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
    void volumeMasterChanged();
    void volumeSfxChanged();
    void volumeMusicChanged();
};

#endif // TOPVIEW_H
