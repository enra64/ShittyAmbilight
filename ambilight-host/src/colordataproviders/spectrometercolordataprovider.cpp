#include "spectrometercolordataprovider.h"

#include <string>

using namespace std;


// hanning window. // i have no idea
float windowFunction(int n, int N)
{
    return 0.5f * (1.0f - cosf(2.0f * M_PI * n / (N - 1.0f)));
}

void SpectrometerRgbLineProvider::calculateAmplitude(
        fftw_complex *fft,
        int fftSize,
        double *amplitudes,
        int numLeds)
{
    // for more explanation see https://groups.google.com/d/msg/comp.dsp/cZsS1ftN5oI/rEjHXKTxgv8J

    // calculate bar width using black magic
    double ledWidth = UPPER_FREQUENCY / (mFramesPerSecond*numLeds);

    // assert minimum bar width
    assert(ledWidth > 0);

    // index of the fft values
    int fftIndex = 0;

    // interpolate led values by calculating an average over the bin magnitudes
    for(int ledIndex = 0; ledIndex < numLeds; ledIndex++)
    {
        double magnitudeSum = 0.0;
        // sum all bin magnitudes for this led
        for(int i = 0; i < ledWidth && fftIndex < fftSize; fftIndex++, i++) {
            // calculate squares of real and imaginary fft result parts
            double real = pow(fft[fftIndex][0], 2);
            double imaginary = pow(fft[fftIndex][1], 2);
            // normalized bin magnitude
            magnitudeSum += 2.0 * sqrt(real + imaginary) / fftSize;
        }

        // calculate average from the sum of normalized bin magnitudes
        double avg = magnitudeSum / ledWidth;

        // compute decibels.
        amplitudes[ledIndex] = (int)(20.0 * log10(avg));
    }
}

SpectrometerRgbLineProvider::SpectrometerRgbLineProvider(size_t xLeds, size_t yLeds, int fps, float gain) : ColorDataProvider(xLeds, yLeds), mFramesPerSecond(fps), mGain(gain)
{
    // set sample specs
    mSampleSpecifications.channels = 2;
    mSampleSpecifications.format = PA_SAMPLE_FLOAT32LE;
    mSampleSpecifications.rate = 44100;

    // open record device
    int error;
    mPulseAudioDevice = pa_simple_new(
                NULL,                                   // default server
                "AmbiSpectrometer",                     // application name
                PA_STREAM_RECORD,
                NULL,                                   // default device
                "Data source for the AmbiSpectrometer", // description
                &mSampleSpecifications,                 // sample specs
                NULL,                                   // default channel map
                NULL,                                   // default buffering attributes
                &error);

    // check for errors
    if(!mPulseAudioDevice)
        throw runtime_error("pa_simple_new() failed:" + string(pa_strerror(error)));

    mSize = mSampleSpecifications.rate / fps;

    mWindow = new float[mSize];
    mPulseAudioBuffer = new float[mSampleSpecifications.channels * mSize];

    // compute window // ???
    for(int n = 0; n < mSize; n++)
        mWindow[n] = windowFunction(n, mSize);

    // fftw setup
    mFftwIn = (double*) fftw_malloc(sizeof(double) * mSize);
    mFftwOut = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * mSize);
    mFftwPlan = fftw_plan_dft_r2c_1d(mSize, mFftwIn, mFftwOut, FFTW_MEASURE);
}

float SpectrometerRgbLineProvider::getData(uint8_t *data) {
    // benchmarking
    clock_t start = clock();

    int err = 0;
    if(pa_simple_read(mPulseAudioDevice, mPulseAudioBuffer, sizeof(mPulseAudioBuffer), nullptr) < 0) {
        pa_simple_free(mPulseAudioDevice);
        throw std::runtime_error("pa_simple_read() failed: " + string(pa_strerror(err)));
    }

    double amplitudes[LED_COUNT];
    double* leftAmplitudes = amplitudes;
    double* rightAmplitudes = amplitudes + LED_COUNT / 2;

    // left input.
    for(int i = 0; i < mSize; i++)
        mFftwIn[i] = (double) (mWindow[i] * mPulseAudioBuffer[i * 2]);
    fftw_execute(mFftwPlan);
    calculateAmplitude(mFftwOut, mSize, leftAmplitudes, LED_COUNT / 2);

    // right input.
    for(int i = 0; i < mSize; i++)
        mFftwIn[i] = (double) (mWindow[i] * mPulseAudioBuffer[i * 2 + 1]);
    fftw_execute(mFftwPlan);
    calculateAmplitude(mFftwOut, mSize, rightAmplitudes, LED_COUNT / 2);

    // make more red the higher the amplitude
    for(size_t i = 0; i < LED_COUNT; i++){
        data[i * 3] = 255.0 / amplitudes[i];
    }

    // return benchmarking value
    return static_cast<float>(clock() - start) / CLOCKS_PER_SEC;
}

SpectrometerRgbLineProvider::~SpectrometerRgbLineProvider() {
    // free buffers
    delete[] mPulseAudioBuffer;
    delete[] mWindow;

    // clean up fftw
    fftw_destroy_plan(mFftwPlan);
    fftw_free(mFftwIn);
    fftw_free(mFftwOut);

    // clean up pulseaudio
    pa_simple_free(mPulseAudioDevice);
}