#ifndef ASHURAWIND_H
#define ASHURAWIND_H

#include <QtWidgets>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace Ui {
class AshuraWind;
}

class AshuraWind : public QMainWindow
{
    Q_OBJECT

public:
    explicit AshuraWind(QWidget *parent = 0);
    ~AshuraWind();

    enum Stages {
        Threshold_Stage,
        AreaRectWithAngle_Stage,
        RotationTranslation_Stage,
        Difference_Stage
    };

    void updateScale();
    bool execute(cv::Mat &image, Stages toStage);

private slots:
    void on_actionOtw_rz_triggered();
    void on_actionOtw_rz_obraz_2_triggered();
    void on_horizontalSlider_sliderMoved(int value);
    void on_doubleSpinBox_valueChanged(double arg1);
    void on_qexec_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_horizontalSlider_2_sliderMoved(int position);

private:
    Ui::AshuraWind *ui;
    qreal mSkala;
    cv::Mat mImageFirst;
    QPixmap mPixmapFirst;
    cv::Mat mImageSecond;
    QPixmap mPixmapSecond;
    cv::Mat mImageOutput;
    QPixmap mPixmapOutput;
};

#endif // ASHURAWIND_H
