#ifndef _ARDUINO_SOUND_H_INCLUDED
#define _ARDUINO_SOUND_H_INCLUDED
class AudioInI2SClass
{
public:
	AudioInI2SClass();
	virtual ~AudioInI2SClass();

	int begin(long sampleRate, int bitsPerSample);
	virtual void end();

	virtual long sampleRate();
	virtual int bitsPerSample();
	virtual int channels();

protected:
	virtual int begin();
	virtual int read(void* buffer, size_t size);
	virtual int reset();

private:
	void onReceive();

	static void onI2SReceive();

private:
	long _sampleRate;
	int _bitsPerSample;
	bool _callbackTriggered;
};
extern AudioInI2SClass AudioInI2S;

#endif