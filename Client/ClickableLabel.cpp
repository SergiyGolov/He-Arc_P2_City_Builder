#include "Client/ClickableLabel.h"

ClickableLabel::ClickableLabel(QString s, QWidget* parent)
    : QLabel(s, parent)
{

}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}
