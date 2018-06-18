#include "ashurawind.h"
#include "ui_ashurawind.h"


AshuraWind::AshuraWind(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::AshuraWind), mSkala(1.0)
{
    ui->setupUi(this);
}

AshuraWind::~AshuraWind()
{
    delete ui;
}

cv::Mat offsetImageWithPadding(const cv::Mat& originalImage, int offsetX, int offsetY, cv::Scalar backgroundColour){
        cv::Mat padded = cv::Mat(originalImage.rows + 2 * abs(offsetY), originalImage.cols + 2 * abs(offsetX), CV_8UC3, backgroundColour);
        originalImage.copyTo(padded(cv::Rect(abs(offsetX), abs(offsetY), originalImage.cols, originalImage.rows)));
        return cv::Mat(padded,cv::Rect(abs(offsetX) + offsetX, abs(offsetY) + offsetY, originalImage.cols, originalImage.rows));
}

void AshuraWind::updateScale()
{
    if (!mPixmapFirst.isNull()) {
        QSize sizeFirst = mPixmapFirst.size();
        sizeFirst*=mSkala;
        ui->qwej->setPixmap(mPixmapFirst.scaled(sizeFirst));
    }
    if (!mPixmapSecond.isNull()) {
        QSize sizeSecond = mPixmapSecond.size();
        sizeSecond*=mSkala;
        ui->qwej_2->setPixmap(mPixmapSecond.scaled(sizeSecond));
    }
    if (!mPixmapOutput.isNull()) {
        QSize sizeOutput = mPixmapOutput.size();
        sizeOutput*=mSkala;
        ui->qwyj->setPixmap(mPixmapOutput.scaled(sizeOutput));
    }
}

bool AshuraWind::execute(cv::Mat &image, AshuraWind::Stages toStage)
{
    cv::Mat tmp;
    cv::cvtColor(image, tmp, CV_BGR2GRAY); // Konwersja do odcieni szarości

    int threshold = ui->spinBox->value(); // Pobranie wartości progowania
    cv::Mat mask = tmp > threshold; // Progowanie obrazu

    if (toStage == Stages::Threshold_Stage) {
        cvtColor(mask, image, CV_GRAY2BGR);
        return true;
    }

    // Wyodrębnienie konturów
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(mask, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    if (contours.size()<1) return false;

    int index=0;
    for(int i=1; i<contours.size(); ++i) { // Tylko największy obszar
        if (contours[index].size() < contours[i].size())
            index = i;
    }

    cv::RotatedRect rotatedRect = cv::minAreaRect(contours[index]); // Dopasowanie prostokąta do konturu

    cv::Point2f rect_points[4];
    rotatedRect.points( rect_points ); // Wczytanie 4 wierzchołków

    float  angle = rotatedRect.angle; // Przepisanie kąta nachylenia

    cv::Point2f center = rotatedRect.center; // Przepisanie środka prostokąta

    if (toStage == Stages::AreaRectWithAngle_Stage) {
        for(unsigned int j=0; j<4; ++j) { // Rysowanie prostokąta
            cv::line(image, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,255,0), 9);
        }

        std::stringstream ss;   ss << angle; // Konwersja float do string
        cv::circle(image, center, 5, cv::Scalar(0,255,0)); // Rysowanie punktu centralnego
        // Umieszczenie kąta nachylenia na obrazie
        cv::putText(image, ss.str(), center + cv::Point2f(-25,25), cv::FONT_HERSHEY_SIMPLEX, 10, cv::Scalar(255,0,0), 10, cv::LINE_AA);

        return true;
    }

    int longestSide=0, shortestSide=10000;
    for(unsigned int j=0; j<4; ++j) { // Wyznaczenie długości najdłuższego i najkrótszego boku prostokąta
        double res = cv::norm(cv::Mat(rect_points[j]),cv::Mat(rect_points[(j+1)%4])); // Długość odcinka wyznaczonego przez punkty
        if (res < shortestSide)
            shortestSide = res;
        if (res > longestSide)
            longestSide = res;
    }

    cv::Mat r = cv::getRotationMatrix2D(center, angle, 1.0);
    cv::warpAffine(image, tmp, r, image.size()); // what size I should use?

    image = offsetImageWithPadding (tmp, center.x-longestSide/2, center.y-shortestSide/2, cv::Scalar(0,0,0));

    cv::Rect roi = cv::Rect(0, 0, longestSide, shortestSide);
    image = cv::Mat(image, roi);

    if (toStage == Stages::RotationTranslation_Stage) {
        return true;
    }

    return false;
}

void AshuraWind::on_actionOtw_rz_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(NULL, QStringLiteral("Wybierz plik"), QDir::currentPath(), "Obraz (*.jpg)", 0, QFileDialog::DontUseNativeDialog);

    if (!files.isEmpty()) {
        int SOmeOFFsets[2] {0,51}; int itmp=0;
        std::vector<cv::Mat> imgs;
        int max_height=0, img_width=0;

        for (auto &s : files) {
            imgs.push_back(cv::imread(s.toStdString().c_str()));
        }

        for (auto &s : imgs) {
            cv::Size s1 = s.size();
            img_width += s1.width;
            if (max_height<s1.height+SOmeOFFsets[itmp])
                max_height = s1.height+SOmeOFFsets[itmp]; itmp++;
        }

        mImageFirst = cv::Mat(max_height, img_width, imgs[0].type()); // put in the type of your mat

        int offset = 0; itmp=0;
        for (auto &s : imgs) {
            cv::Size s1 = s.size();
            cv::Mat helper(mImageFirst, cv::Rect(offset, SOmeOFFsets[itmp++], s1.width, s1.height));
            offset += s1.width;

            s.copyTo(helper);
        }

        cv::Mat out;
        cvtColor(mImageFirst, out, CV_BGR2RGB);
        mPixmapFirst = QPixmap::fromImage(QImage(out.data, out.cols, out.rows, out.step, QImage::Format_RGB888));
        updateScale();
    }
}

void AshuraWind::on_actionOtw_rz_obraz_2_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(NULL, QStringLiteral("Wybierz plik"), QDir::currentPath(), "Obraz (*.jpg)", 0, QFileDialog::DontUseNativeDialog);

    if (!files.isEmpty()) {
        int SOmeOFFsets[2] {0,51}; int itmp=0;
        std::vector<cv::Mat> imgs;
        int max_height=0, img_width=0;

        for (auto &s : files) {
            imgs.push_back(cv::imread(s.toStdString().c_str()));
        }

        for (auto &s : imgs) {
            cv::Size s1 = s.size();
            img_width += s1.width;
            if (max_height<s1.height+SOmeOFFsets[itmp])
                max_height = s1.height+SOmeOFFsets[itmp]; itmp++;
        }

        mImageSecond = cv::Mat(max_height, img_width, imgs[0].type()); // put in the type of your mat

        int offset = 0; itmp=0;
        for (auto &s : imgs) {
            cv::Size s1 = s.size();
            cv::Mat helper(mImageSecond, cv::Rect(offset, SOmeOFFsets[itmp++], s1.width, s1.height));
            offset += s1.width;
            s.copyTo(helper);
        }

        cv::Mat out;
        cvtColor(mImageSecond, out, CV_BGR2RGB);
        mPixmapSecond = QPixmap::fromImage(QImage(out.data, out.cols, out.rows, out.step, QImage::Format_RGB888));
        updateScale();
    }
}

void AshuraWind::on_horizontalSlider_sliderMoved(int value)
{
    ui->doubleSpinBox->setValue((qreal)value/100);
}

void AshuraWind::on_doubleSpinBox_valueChanged(double arg1)
{
    ui->horizontalSlider->setValue(arg1*100);
    mSkala = arg1;

    updateScale();
}

int diff (cv::Mat &img_1, cv::Mat &img_2, cv::Mat &debug_img) {
    cv::Mat result_mat;
    cv::Mat src_img;
    cv::Mat template_img;

    cv::cvtColor(img_1, src_img, CV_BGR2GRAY); // Konwersja do odcieni szarości
    cv::cvtColor(img_2, template_img, CV_BGR2GRAY); // Konwersja do odcieni szarości

    cv::cvtColor(template_img, debug_img, CV_GRAY2BGR);

    int match_method = CV_TM_CCORR_NORMED;
    cv::matchTemplate(src_img, template_img, result_mat, match_method);
    cv::normalize(result_mat, result_mat, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());

    double minVal; double maxVal;
    cv::Point minLoc, maxLoc, matchLoc;
    cv::minMaxLoc(result_mat, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )  matchLoc = minLoc;
    else matchLoc = maxLoc;

    cv::rectangle(
        debug_img,
        matchLoc,
        cv::Point(matchLoc.x + template_img.cols , matchLoc.y + template_img.rows),
        CV_RGB(255,0,0),
        3);

    return 0;
}

void AshuraWind::on_qexec_clicked()
{
    cv::Mat gray;
    if (!ui->checkBox_4->isChecked()) {
        if (ui->comboBox->currentIndex()==0) {
            mImageFirst.copyTo(mImageOutput);
        } else {
            mImageSecond.copyTo(mImageOutput);
        }
    }

    if (ui->checkBox->isChecked() && !ui->checkBox_2->isChecked()) {
        execute(mImageOutput, Threshold_Stage);
        cv::Mat out;
        cvtColor(mImageOutput, out, CV_BGR2RGB);
        mPixmapOutput = QPixmap::fromImage(QImage(out.data, out.cols, out.rows, out.step, QImage::Format_RGB888));
    } else if (ui->checkBox_2->isChecked() && !ui->checkBox_3->isChecked()) {
        execute(mImageOutput, AreaRectWithAngle_Stage);
        cv::Mat out;
        cvtColor(mImageOutput, out, CV_BGR2RGB);
        mPixmapOutput = QPixmap::fromImage(QImage(out.data, out.cols, out.rows, out.step, QImage::Format_RGB888));
    } else if (ui->checkBox_3->isChecked() && !ui->checkBox_4->isChecked()) {
        execute(mImageOutput, RotationTranslation_Stage);
        cv::Mat out;
        cvtColor(mImageOutput, out, CV_BGR2RGB);
        mPixmapOutput = QPixmap::fromImage(QImage(out.data, out.cols, out.rows, out.step, QImage::Format_RGB888));
    } else if (ui->checkBox_4->isChecked()) {
        cv::Mat out1;
        mImageFirst.copyTo(out1);
        execute(out1, RotationTranslation_Stage);
        cv::Mat out2;
        mImageSecond.copyTo(out2);
        execute(out2, RotationTranslation_Stage);
        cv::Mat out;

        cv::resize(out1, out1, cv::Size(out2.size[1], out2.size[0]), 0, 0, CV_INTER_LINEAR);

        cv::absdiff(out1, out2, mImageOutput);
        cv::threshold(mImageOutput, mImageOutput, 80, 255, cv::THRESH_BINARY);
        cv::erode(mImageOutput, mImageOutput, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(6,6)));

        cvtColor(mImageOutput, out, CV_BGR2RGB);
        mPixmapOutput = QPixmap::fromImage(QImage(out.data, out.cols, out.rows, out.step, QImage::Format_RGB888));
    }

    updateScale();
}

void AshuraWind::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider_2->setValue(arg1);
    on_qexec_clicked();
}

void AshuraWind::on_horizontalSlider_2_sliderMoved(int position)
{
    ui->spinBox->setValue(position);
}
