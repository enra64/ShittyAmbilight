#ifndef FPSMETER_H
#define FPSMETER_H

#include <QCheckBox>
#include <QLabel>
#include <QWidget>

#ifdef QT_CHARTS_FOUND
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
QT_CHARTS_USE_NAMESPACE // using namespace qtcharts
#endif // QT_CHARTS_FOUND

class FpsMeter : public QWidget
{
    Q_OBJECT

public:
    explicit FpsMeter(QWidget *parent = 0);
    ~FpsMeter();

public:// interface
    /**
     * @brief Pushes the fps value into our stack to be displayed as the newest value
     * @param fpsValue the new value
     */
    void update(float fpsValue);

    /**
     * @brief Enables or disables the chart view embedded in this widget
     * @param enable The widget shows the chart if enable is true
     */
    void setEnabled(bool enable = true);

private slots:
    void toggled(bool checked);

private: // member functions
    void setupChart();
    float getHistoryAverage();

private: // normal member variables
    QCheckBox* mEnableCheckBox;///< checkbox used to dis/enable widget display
    unsigned int mFpsTickCount = 0;///< how many fps ticks have been displayed
    const static int CONCURRENT_FPS_VALUES = 200; ///< how many fps values may be in the graph at a time
    float mFpsHistory[CONCURRENT_FPS_VALUES];///< a history of the values to calc. a moving average

#ifdef QT_CHARTS_FOUND
private: // qt charts members
    QLineSeries* mFpsLineSeries = new QLineSeries();///< the fps line series (fps data)
    QLineSeries* mAverageFpsLineSeries = new QLineSeries();///< the average fps line series
    QChart* mFpsChart = new QChart();///< the fps chart displayed by the chart view
    QChartView* mFpsChartView = nullptr;///< our fps chart view
#else
private:  // alternative members
    QLabel* mFpsLabel;
#endif //QT_CHARTS_FOUND

};
#endif // FPSMETER_H
