#include "renderarea.h"
#include <QPaintEvent>
#include <QPainter>

RenderArea::RenderArea(QWidget *parent) :
    QWidget{parent},
    mBackgroundColor (0,0,255),
    mPen (Qt::white),
    mShape (Astroid)
{
    mPen.setWidth(2);
    on_shape_changed();
}


QSize RenderArea::minimumSizeHint() const
{
    return QSize(400, 400);
}

QSize RenderArea::sizeHint() const
{
    return QSize(400, 400);
}

void RenderArea::on_shape_changed()
{
    switch(mShape)
    {
    case Astroid:
        mScale = 90;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;
    case Cycloid:
        mScale = 10;
        mIntervalLength = 4 * M_PI;
        mStepCount = 128;
        break;
    case HuygensCycloid:
        mScale = 4;
        mIntervalLength = 4 * M_PI;
        mStepCount = 256;
        break;
    case HypoCycloid:
        mScale = 40;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;
    case Line:
        mScale = 100;
        mIntervalLength = 2; // Arbitrary number
        mStepCount = 128;
        break;
    case Circle:
        mScale = 100;
        mIntervalLength = 2 * M_PI;
        mStepCount = 128;
        break;
    case Ellipse:
        mScale = 75;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;
    case Flower:
        mScale = 10;
        mIntervalLength = 12 * M_PI;
        mStepCount = 512;
        break;
    case Star:
        mScale = 25;
        mIntervalLength = 6 * M_PI;
        mStepCount = 256;
        break;
    case Cloud:
        mScale = 10;
        mIntervalLength = 28 * M_PI;
        mStepCount = 128;
        break;
    case InvertedCloud:
        mScale = 10;
        mIntervalLength = 28 * M_PI;
        mStepCount = 128;
        break;
    case Cardioid:
        mScale = 10;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;
    default: // Default to astroid for now
        mScale = 40;
        mIntervalLength = 2 * M_PI;
        mStepCount = 256;
        break;
    }
}

QPointF RenderArea::compute(float t)
{
    switch(mShape)
    {
    case Astroid:
        return compute_astroid(t);
        break;
    case Cycloid:
        return compute_cycloid(t);
        break;
    case HuygensCycloid:
        return compute_huygens(t);
        break;
    case HypoCycloid:
        return compute_hypo(t);
        break;
    case Line:
        return compute_line(t);
        break;
    case Circle:
        return compute_circle(t);
        break;
    case Ellipse:
        return compute_ellipse(t);
        break;
    case Flower:
        return compute_flower(t);
        break;
    case Star:
        return compute_star(t);
        break;
    case Cloud:
        return compute_cloud(t,-1);
        break;
    case InvertedCloud:
        return compute_cloud(t,1);
        break;
    case Cardioid:
        return compute_cardioid(t);
        break;
    default:
        break;
    }
    return QPointF(0,0);
}

// Parametric equations
QPointF RenderArea::compute_astroid(float t)
{
    float cos_t = cos(t);
    float sin_t = sin(t);
    float X = 2 * cos_t * cos_t * cos_t;
    float Y = 2 * sin_t * sin_t * sin_t;

    return QPointF(X,Y);
}

QPointF RenderArea::compute_cycloid(float t)
{
    return QPointF(
                1.5 * (1 - cos(t)),
                1.5 * (t - sin(t))
    );
}

QPointF RenderArea::compute_huygens(float t)
{
    return QPointF(
                4 * (3 * cos(t) - cos(3 * t)),
                4 * (3 * sin(t) - sin(3 * t))
    );
}

QPointF RenderArea::compute_hypo(float t)
{
    return QPointF(
                1.5 * (2 * cos(t) + cos(2 * t)),
                1.5 * (2 * sin(t) - sin(2 * t))
    );
}

/*
 * Shape: Line
 * Equation: x = 1 - t, y = 1 - t
 */
QPointF RenderArea::compute_line(float t)
{
    return QPointF(1 - t, 1 - t);
}

/*
 * Shape: Circle
 * Equation: x = cos(t), y = sin(t)
 */
QPointF RenderArea::compute_circle(float t)
{
    return QPointF(cos(t), sin(t));
}

/*
 * Shape: Ellipse
 * Equation: x = (a)*cos(t), y = (b)*sin(t)
 */
QPointF RenderArea::compute_ellipse(float t)
{
    // Arbitrary values hardcoded for a,b
    float a = 2;
    float b = 1.1;
    return QPointF(a * cos(t), b * sin(t));
}

/*
 * Shape: Flower
 * Equation:
 *   x = a * cos(t) - b * cos(c * t)
 *   y = a * sin(t) - b * sin(c * t)
 */
QPointF RenderArea::compute_flower(float t)
{
    float a = 11.0;
    float b = 6.0;
    float c = (a/b);
    return QPointF(
       a * cos(t) - b * cos(c * t),
       a * sin(t) - b * sin(c * t)
    );
}

/*
 * Shape: Star
 * Equation:
 *   x = (R - r) * cos(t) + d * cos( ((R - r) / r ) * t )
 *   y = (R - r) * sin(t) - d * sin( ((R - r) / r ) * t )
 */
QPointF RenderArea::compute_star(float t)
{
    float R = 5.0;
    float r = 3.0;
    float d = 5.0;
    return QPointF(
       (R - r) * cos(t) + d * cos( ((R - r) / r ) * t ),
       (R - r) * sin(t) - d * sin( ((R - r) / r ) * t )
    );
}

/*
 * Shape: Cloud
 * Equation:
 *   x = (a + b) * cos(t * (b/a)) + b * cos( ((a + b) / a ) * t )
 *   y = (a + b) * sin(t * (b/a)) - b * sin( ((a + b) / a ) * t )
 */
QPointF RenderArea::compute_cloud(float t, int sign)
{
    float a = 14.0;
    float b = 1.0;

    return QPointF(
       (a + b) * cos(t * (b/a)) + sign * b * cos( ((a + b) / a ) * t ),
       (a + b) * sin(t * (b/a)) - b * sin( ((a + b) / a ) * t )
    );
}

/*
 * Shape: Cardioid
 * Equation:
 *   x = -16sin^3(t)
 *   y = -13cos(t) + 5cos(2t) + 2cos(3t) + cos(4t)
 */
QPointF RenderArea::compute_cardioid(float t)
{
    float x = -16 * pow(sin(t),3);
    float y = -13 * cos(t) + 5 * cos(2 * t) + 2 * cos(3 * t) + cos(4 * t);

    return QPointF(x,y);
}

void RenderArea::paintEvent(QPaintEvent *event)
{
    // ! TODO: For future use...
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.setBrush(mBackgroundColor);
    painter.setPen(mPen);

    // drawing area
    painter.drawRect(this->rect());

    QPoint center = this->rect().center();

    QPointF prevPoint = compute(0);
    QPointF prevPixel;
    prevPixel.setX(prevPoint.x() * mScale + center.x());
    prevPixel.setY(prevPoint.y() * mScale + center.y());

    float step = mIntervalLength / mStepCount;
    for(float t = 0; t < mIntervalLength; t += step)
    {
        QPointF point = compute(t);

        QPoint pixel;
        pixel.setX(point.x() * mScale + center.x());
        pixel.setY(point.y() * mScale + center.y());

        painter.drawLine(pixel, prevPixel);
        prevPixel = pixel;
    }
    QPointF point = compute(mIntervalLength);
    QPoint pixel;
    pixel.setX(point.x() * mScale + center.x());
    pixel.setY(point.y() * mScale + center.y());
    painter.drawLine(pixel, prevPixel);
}
