
#include "esp32_i2s.h"

AudioInI2SClass::AudioInI2SClass() :
    _sampleRate(-1),
    _bitsPerSample(-1),
    _callbackTriggered(true)
{
}

AudioInI2SClass::~AudioInI2SClass()
{
}




int AudioInI2SClass::begin(long sampleRate, int bitsPerSample)
{
    if (!I2S.begin(I2S_PHILIPS_MODE, sampleRate, bitsPerSample)) {
        return 0;
    }

    _sampleRate = sampleRate;
    _bitsPerSample = bitsPerSample;

    // add the receiver callback
    I2S.onReceive(AudioInI2SClass::onI2SReceive);

    // trigger a read to kick things off
    I2S.read();

    return 1;
}

void AudioInI2SClass::end()
{
    _sampleRate = -1;
    _bitsPerSample = -1;
    _callbackTriggered = true;

    I2S.end();
}

long AudioInI2SClass::sampleRate()
{
    return _sampleRate;
}

int AudioInI2SClass::bitsPerSample()
{
    return _bitsPerSample;
}

int AudioInI2SClass::channels()
{
    return 2;
}

int AudioInI2SClass::begin()
{
    _callbackTriggered = false;

    return 0;
}

int AudioInI2SClass::read(void* buffer, size_t size)
{
    int read = I2S.read(buffer, size);

    if (read) {
        samplesRead(buffer, read);
    }

    return read;
}

int AudioInI2SClass::reset()
{
    return 0;
}

void AudioInI2SClass::onReceive()
{
    if (_callbackTriggered) {
        uint8_t data[512];

        read(data, sizeof(data));
    }
}

void AudioInI2SClass::onI2SReceive()
{
    AudioInI2S.onReceive();
}

AudioInI2SClass AudioInI2S;
