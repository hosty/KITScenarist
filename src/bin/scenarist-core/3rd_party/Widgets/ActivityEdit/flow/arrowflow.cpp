#include "arrowflow.h"
#include <QLineF>
#include <math.h>
#include <QPainter>
#include "../shape/card.h"
#include "../shape/note.h"
#include "../flow/pdlg_arrowflow.h"
#include "flowtext.h"
#include <QGraphicsScene>
#include <QDebug>

/**
 * Возвращает фигуру текста, связанную со связью.
 * @return Фигура текста.
 */
FlowText *ArrowFlow::textShape() const
{
	return _flowtext;
}

QPainterPath ArrowFlow::shape() const
{
	if (endShape()->parentItem() != startShape()) {
		return Flow::shape();
	}

	return QPainterPath();
}

bool ArrowFlow::canConnectStartTo (Shape *s)
{
	return dynamic_cast<CardShape *>(s)	&& s != endShape();
}


bool ArrowFlow::canConnectEndTo (Shape *s)
{
	return dynamic_cast<CardShape *>(s)	&& s != startShape();
}

/**
 * Задает текст над связью.
 * @param newtext Новое значение текста
 */
void ArrowFlow::setText (const QString &newtext)
{
	prepareGeometryChange();
	_text = newtext;
	emit contentsChanged();
}

/**
 * Возвращает текст над связью.
 * @return Текущее значение текста
 */
const QString &ArrowFlow::text() const
{
	return _text;
}

void ArrowFlow::editProperties()
{
	ArrowFlowPropertiesDialog(this).exec();
}

/**
 * Проверяет, можно ли соединить две фигуры этой связью.
 * Вообще-то, логичнее было бы сделать эту функцию виртуальной, но пока удобнее
 * ее использовать так - ведь на момент ее вызова экземпляр еще не существует,
 * и склонировать его неоткуда, а вот тип связи как раз хорошо известен.
 * Возможно, при последующей разработке можно дополнить Flow чисто виртуальной
 * функцией, реализующей точно такую же функциональность, и обобщить код порождающих
 * состояний сцены (ShapeSceneTool, FlowSceneTool) c помощью прототипов для всех фигур,
 * но пока что вот такого варианта вполне достаточно.
 *
 * @see NoteFlow::canConnect()
 * @param s Начальная фигура
 * @param e Конечная фигура
 * @return true, если можно соединить фигуры
 */
bool ArrowFlow::canConnect (Shape *s, Shape *e)
{
	if (s==e) return false;
	if (dynamic_cast<FlowText *>(s) || dynamic_cast<FlowText *>(e)) return false;
	if (dynamic_cast<NoteShape *>(s) || dynamic_cast<NoteShape *>(e)) return false;
	if (dynamic_cast<Flow *>(s) || dynamic_cast<Flow *>(e)) return false;
	return true;
}

ArrowFlow::ArrowFlow (Shape *start, Shape *end, QGraphicsItem *parent)
	: Flow(start, end, parent)
{
	_flowtext = new FlowText(this, this);
}

ArrowFlow::~ArrowFlow()
{
	if (scene())
		scene()->removeItem(_flowtext);
	delete _flowtext;
}

void ArrowFlow::paintCap (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPolygonF poly;
	poly << _start_pt;
	for(int i=0; i<knots.count(); ++i)
		poly << knots[i];
	poly << _end_pt;

	QLineF line(_end_pt, poly[poly.count()-2]);
	double arrowSize = 22;
	double angle = ::acos(line.dx()/line.length());
	if (line.dy() >= 0) angle = (M_PI * 2) - angle;
	QPointF arrowP1 = line.p1() + QPointF(
			sin(angle + M_PI / 2.35) * arrowSize,
			cos(angle + M_PI / 2.35) * arrowSize);
	QPointF arrowP2 = line.p1() + QPointF(
			sin(angle + M_PI - M_PI / 2.35) * arrowSize,
			cos(angle + M_PI - M_PI / 2.35) * arrowSize);
	painter->setBrush(painter->pen().color());
	painter->drawPolygon(QPolygonF() << arrowP1 << line.p1() << arrowP2);
}

void ArrowFlow::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	if (endShape()->parentItem() != startShape()) {
		Flow::paint(painter, option, widget);
		paintCap(painter, option, widget);
	}
}

QVariant ArrowFlow::itemChange (GraphicsItemChange change, const QVariant &value)
{
	switch(change)
	{
		case ItemSceneChange:
		{
			// Нужно помнить про фигуру текста и переносить ее за собой при изменениях сцены
			QGraphicsScene *scene = qvariant_cast<QGraphicsScene *>(value);
			if (scene)
			{
				scene->addItem(_flowtext), _flowtext->updateShape();
				connect(_flowtext, SIGNAL(stateIsAboutToBeChangedByUser()), this, SIGNAL(stateIsAboutToBeChangedByUser()));
			}
			else
			{
				this->scene()->removeItem(_flowtext);
				disconnect(_flowtext, SIGNAL(stateIsAboutToBeChangedByUser()), this, SIGNAL(stateIsAboutToBeChangedByUser()));
			}
		} break;
		default:
			break;
	}
	return Flow::itemChange(change, value);
}
