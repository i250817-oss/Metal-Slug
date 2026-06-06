#pragma once
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;

// gen1 the guy who made that is one of the goated ta best question of the
// assignment you can see from the comment what i learned

/*
vvv imp point :
#pragma pack(push, 1)
tells the compiler:
add NO padding bytes between struct fields.
This guarantees the struct is exactly 44 bytes, matching the WAV file spec.
Without it, your reads are misaligned and produce garbage.
*/

#pragma pack(push, 1)
struct WavHeader {
  char chunkId[4];     //"RIFF"
  int chunkSize;       // total file size - 8
  char format[4];      //"WAVE"
  char subchunk1Id[4]; //"fmt "
  int subchunk1Size;   // 16 for PCM
  short audioFormat;   // 1 for PCM
  short numChannels;   // 1 for Mono, 2 for Stereo
  int sampleRate;      // e.g. 44100
  int byteRate;        // sampleRate * numChannels * 2
  short blockAlign;    // numChannels * 2
  short bitsPerSample; // 16
  char subchunk2Id[4]; //"data"
  int subchunk2Size;   // size of audio data in bytes
};
#pragma pack(pop) // no padding so  layout is exactly as above

/*
->provided statement (copy paste from assignment )
Phase 4: Filtering Features
In order to filter the unwoven Whispers, you must implement:
 Filter class: A standalone filter class holding an array of float weights (0.0
to 1.0). Frequency to Index mapping: Develop a formula to map indexes to certain
frequency bins. (Critical insight: The Nyquist Mirror)
 Filtering will be done via construction of these specific filter types under
three methods: Low Pass: Cut the frequencies above a specific cutoff. High Pass:
Cut the frequencies below a specific cutoff. Band Pass: Isolate the midrange
whispers (lowerbound - upperbound) while silencing all other frequencies.

This filter object will be passed onto a method from spectrum for application.
*/

class Filter {
private:
  float *InputValues;
  int paddedValues;

public:
  Filter(int size = 0) {
    paddedValues = size;
    if (size == 0) {
      InputValues = NULL;
      return;
    }
    InputValues = new float[paddedValues];
    for (int i = 0; i < paddedValues; i++)
      InputValues[i] = 0;
  }

  Filter(const Filter &other) {
    paddedValues = other.paddedValues;
    if (paddedValues == 0 || other.InputValues == NULL) {
      InputValues = NULL;
      return;
    }
    InputValues = new float[paddedValues];
    for (int i = 0; i < paddedValues; i++)
      InputValues[i] = other.InputValues[i];
  }

  ~Filter() {
    delete[] InputValues;
    InputValues = NULL;
  }

  float *getInputValues() { return InputValues; }
  int getPaddedValues() { return paddedValues; }

  double filterRes(int sampleRate, int paddedSize, float cutoffHz) {

    return cutoffHz * paddedSize / (sampleRate);
  }

  Filter lowPass(int sampleRate, int paddedSize, float cutoffHz) {
    if (InputValues == NULL || paddedValues != paddedSize) {
      return *this;
    }
    double result = filterRes(sampleRate, paddedSize, cutoffHz);

    for (int i = 0; i <= paddedValues / 2; i++) {
      InputValues[i] = (i <= result) ? 1 : 0;
    }

    for (int i = 1; i < paddedValues / 2; i++)
      InputValues[paddedValues - i] = InputValues[i];
    return *this;
  }

  Filter highPass(int sampleRate, int paddedSize, float cutoffHz) {
    if (InputValues == NULL || paddedValues != paddedSize) {
      return *this;
    }
    double result = filterRes(sampleRate, paddedSize, cutoffHz);
    for (int i = 0; i <= paddedValues / 2; i++) {
      InputValues[i] = (i >= result) ? 1 : 0;
    }
    for (int i = 1; i < paddedValues / 2; i++)
      InputValues[paddedValues - i] = InputValues[i];
    return *this;
  }

  Filter bandPass(int sampleRate, int paddedSize, float lowHz, float highHz) {
    if (InputValues == NULL || paddedValues != paddedSize) {
      return *this;
    }
    double lowIndex = filterRes(sampleRate, paddedSize, lowHz);
    double highIndex = filterRes(sampleRate, paddedSize, highHz);
    for (int i = 0; i <= paddedValues / 2; i++) {
      InputValues[i] = (i >= lowIndex && i <= highIndex) ? 1 : 0;
    }
    for (int i = 1; i < paddedValues / 2; i++)
      InputValues[paddedValues - i] = InputValues[i];
    return *this;
  }
};

class Complex {
private:
  double real;
  double imag;

public:
  Complex(double r = 0, double i = 0) : real(r), imag(i) {}

  double getReal() { return real; }
  double getImagnary() { // kept your comment/logic name
    return imag;
  }
  void setIt(double r, double i) {
    real = r;
    imag = i;
  }

  Complex operator+(const Complex &other) const {
    return Complex(real + other.real, imag + other.imag);
  }
  Complex operator-(const Complex &other) const {
    return Complex(real - other.real, imag - other.imag);
  }
  Complex operator*(const Complex &other) const {
    return Complex(real * other.real - imag * other.imag,
                   real * other.imag + imag * other.real);
  }
  Complex operator*(double val) const {
    return Complex(real * val, imag * val);
  }
  Complex operator/(double val) const {
    if (val == 0.0) {
      return *this; // if it be 0 we return that function (kept your logic)
    }
    return Complex(real / val, imag / val);
  }
  double magnitude() const { return sqrt(real * real + imag * imag); }
  Complex operator/(const Complex &input) {
    double bottom = (input.real * input.real) + (input.imag * input.imag);

    if (bottom == 0) {
      return Complex(0, 0); // we need to provide a null complex entity
    }

    Complex result;
    result.real =
        ((this->real * input.real) + (this->imag * input.imag)) / bottom;
    result.imag =
        ((this->imag * input.real) - (this->real * input.imag)) / bottom;
    return result;
  }
  Complex operator*(float scalar) const {
    return Complex(real * scalar, imag * scalar);
  }
};

// forward declaration of Whisper because Spectrum returned it in ifft (old
// design) and Whisper needs Spectrum in fft() that's why at the end you will
// see i initialize it
class Whisper;
class Filter;

// Spectrum class
class Spectrum {
private:
  Complex *outputFreq;
  int paddedSize;
  int original;
  int sampleRate;
  int numChannels;

public:
  Spectrum(Complex *input = NULL, int paddedValue = 0, int orig = 0, int sr = 0,
           int ch = 1) {
    paddedSize = paddedValue;
    original = orig;
    sampleRate = sr;
    numChannels = ch;

    if (input == NULL || paddedSize == 0) {
      outputFreq = NULL;
      return;
    }

    outputFreq = new Complex[paddedSize];
    for (int i = 0; i < paddedSize; i++)
      outputFreq[i] = input[i];
  }

  // copy constructor
  Spectrum(const Spectrum &other) {
    paddedSize = other.paddedSize;
    original = other.original;
    sampleRate = other.sampleRate;
    numChannels = other.numChannels;

    if (other.outputFreq == NULL || paddedSize == 0) {
      outputFreq = NULL;
      return;
    }

    outputFreq = new Complex[paddedSize];
    for (int i = 0; i < paddedSize; i++)
      outputFreq[i] = other.outputFreq[i];
  }

  // destructor
  ~Spectrum() {
    delete[] outputFreq;
    outputFreq = NULL;
  }

  // getters

  Complex *getOutputFreq() { return outputFreq; }
  int getPaddedSize() { return paddedSize; }
  int getOriginal() { return original; }
  int getSampleRate() { return sampleRate; }
  int getNumChannels() { return numChannels; }

  void applyFilter(Filter &filter) {
    if (outputFreq == NULL)
      return;

    int filterSize = filter.getPaddedValues();
    int limit = paddedSize < filterSize ? paddedSize : filterSize;

    for (int i = 0; i < limit; i++)
      outputFreq[i] = outputFreq[i] * filter.getInputValues()[i];

    for (int i = limit; i < paddedSize; i++)
      outputFreq[i] = Complex(0.0, 0.0);
  }

  Whisper ifft();
};

class Whisper {
private:
  WavHeader Input;
  short *data; // dynamically allocated array of 16-bit audio samples. This is
               // the raw audio.
  int dataBytes; // number of samples (16-bit shorts)

public:
  // default constructor
  Whisper() {
    data = NULL;
    dataBytes = 0;
    // zero the header
    for (int i = 0; i < 4; i++)
      Input.chunkId[i] = 0;
    Input.chunkSize = 0;
    for (int i = 0; i < 4; i++)
      Input.format[i] = 0;
    for (int i = 0; i < 4; i++)
      Input.subchunk1Id[i] = 0;
    Input.subchunk1Size = 0;
    Input.audioFormat = 0;
    Input.numChannels = 0;
    Input.sampleRate = 0;
    Input.byteRate = 0;
    Input.blockAlign = 0;
    Input.bitsPerSample = 0;
    for (int i = 0; i < 4; i++)
      Input.subchunk2Id[i] = 0;
    Input.subchunk2Size = 0;
  }

  // use ifstream in binary mode.If fails, set data = NULL, dataBytes = 0,
  // return.
  Whisper(const char *path) {
    data = NULL;
    dataBytes = 0;

    fstream file;
    // one more thing i added the ios::in as it tells binary to read some
    // compiler may show flag honestly my one did not so i did that so
    file.open(path,
              ios::binary |
                  ios::in); // since it directly write on the file if eg i want
                            // to write hello \n in ios::in it be hello \r\n it
                            // is translated as \r but in binary it is
                            // translated as same like hello \n
    if (!file.is_open()) {
      data = NULL;
      dataBytes = 0;
      return;
    }
    if (file.fail()) {
      return;
    } // before reading we need to checking weither it has error in the error or
      // not
    // read the fixed 44-byte WAV header
    file.read((char *)&Input, sizeof(Input));
    // why char* since it read one byte
    // so 44 byte assign byte by byte to the member which i want he he he
    // without padding in the waveheader .h so it is for that

    // i was doing the resurch on the mp4 and i have found that fascinating
    // change please see it normally we assume that after 44 bytes  comes "data"
    // i personaly did before doing that question but Real WAV structure can be:
    /*RIFF
     fmt
     LIST include an extra chunk (ir relavent like inlcude the basic information
     about the auother and other things ) ID3  include an extra chunk data
     include an actual audio so in that case data is shift left minor case so we
     need to figure it out
     */

    bool dataFound =
        (Input.subchunk2Id[0] == 'd' && Input.subchunk2Id[1] == 'a' &&
         Input.subchunk2Id[2] == 't' && Input.subchunk2Id[3] == 'a');
    // IN THE WAVE HEADER WE HAVE .WAV FILE THAT ALWAYS HAVE IN THE FOLLOWING
    // ORDER 1)RIFF ALL FILE MUST START WITH THAT IT HOLD 4 BYTES 2)CHUNK ID IE
    // THE Size of entire file after this field WE NEGATE IT WITH 8 AS THE CHUNK
    // ID PLUS THE CHUNK SIZE HOLD 8 SO HOW MUCH DATA THAT IT HOLDS 3)WAVE TELLS
    // THAT IN THAT RIFF FILE HERE IS THE WAVE AUDIO IN SHORT IT IS FOR THE
    // FORMAT IF IT IS WAV THEN ITS FOR SOUND ELSE FOR OTHER LIKE AUDIO 4 BYTES
    // 4)FMT (FORMAT CHUNK) his chunk tells your program how to interpret the
    // audio data in the data chunK 4 BYTES 5) SIZE OF FMT CHUNK 4 BYTES 16 IS
    // THE VALUE OF PCMM 6)AUDIO FOMAT  2 BYTES 1= COMPRESSED AND 2 =
    // UNCOMPRESSED 7)NUM CHANNELS SHORT  1 = MONO 2 = STERIO 8) sampleRate IE
    // Samples per second example 44100 Hz determines pitch and speed (4 bytes)
    // 9) byteRate  Bytes per second = sampleRate * numChannels *
    // bitsPerSample/8; used for streaming (4 bytes) 10) blockAlign Bytes per
    // frame (all channels together) = numChannels * bitsPerSample/8 (2 bytes)
    // 11) bitsPerSample  Bits per sample per channel: 8, 16, etc.; determines
    // audio precision (2 bytes) 12) subchunk2Id  "data" marks the start of
    // actual audio samples (4 bytes) 13) subchunk2Size  Size of audio data in
    // bytes = numSamples * numChannels * bitsPerSample/8 (4 bytes)

    if (!dataFound) {
      // so here what we are doing is first we are jumping the pointer to that
      // point like after 12+8 and let say 16 so 36 so it jump to the end chunk
      int endChunk = 12 + 8 + Input.subchunk1Size;
      file.seekg(endChunk, ios::beg);
      // here that function seekg(final position , initial pos)
      // here it will move the reading pointer from begining to that end
      // temp chunk
      char chunkId[4];
      int chunkSz = 0;
      while (file.read(chunkId, 4)) {
        file.read((char *)&chunkSz, 4);
        if (file.fail())
          break;

        if (chunkId[0] == 'd' && chunkId[1] == 'a' && chunkId[2] == 't' &&
            chunkId[3] == 'a') {
          // found the real data chunk and update the header fields
          Input.subchunk2Id[0] = 'd';
          Input.subchunk2Id[1] = 'a';
          Input.subchunk2Id[2] = 't';
          Input.subchunk2Id[3] = 'a';
          Input.subchunk2Size = chunkSz;
          dataFound = true; // looop break
          break;
        }
        // skip this chunk's payload and move to the next one from that
        file.seekg(
            chunkSz,
            ios::cur); // where it was previously current means the inial point
      }

      if (!dataFound) {
        file.close();
        return;
      }
    }

    int audioBytes = Input.subchunk2Size;

    if (Input.bitsPerSample == 16) {
      // 16bit
      dataBytes = audioBytes / 2;
      data = new short[dataBytes];
      file.read((char *)data,
                audioBytes); // it will read 2 bytes per sample from here

    } else if (Input.bitsPerSample == 8) {
      // 8 bit
      dataBytes = audioBytes;
      data = new short[dataBytes];

      unsigned char *temp = new unsigned char[audioBytes];
      file.read((char *)temp, audioBytes);
      for (int i = 0; i < dataBytes; i++) {
        data[i] = ((temp[i] - 128) * 256);
      }
      delete[] temp;

      // update header
      Input.bitsPerSample = 16;
      Input.byteRate = Input.sampleRate * Input.numChannels * 2;
      Input.blockAlign = Input.numChannels * 2; // provided formuleas
      Input.subchunk2Size = dataBytes * 2;
      Input.chunkSize = 36 + Input.subchunk2Size;

    } else {
    }

    file.close();
  }

  // construct from a raw short array (used by ifft, splice, mix, etc.)
  // builds a minimal valid mono/stereo WAV header from sampleRate

  Whisper(short *arr, int size, int sampleRate, int channels = 1) {
    dataBytes = size;
    data = new short[dataBytes];
    for (int i = 0; i < dataBytes; i++)
      data[i] = arr[i];

    // build header
    Input.chunkId[0] = 'R';
    Input.chunkId[1] = 'I';
    Input.chunkId[2] = 'F';
    Input.chunkId[3] = 'F';
    Input.format[0] = 'W';
    Input.format[1] = 'A';
    Input.format[2] = 'V';
    Input.format[3] = 'E';
    Input.subchunk1Id[0] = 'f';
    Input.subchunk1Id[1] = 'm';
    Input.subchunk1Id[2] = 't';
    Input.subchunk1Id[3] = ' ';
    Input.subchunk2Id[0] = 'd';
    Input.subchunk2Id[1] = 'a';
    Input.subchunk2Id[2] = 't';
    Input.subchunk2Id[3] = 'a';

    Input.subchunk1Size = 16;
    Input.audioFormat = 1; // PCM
    Input.numChannels = channels;
    Input.sampleRate = sampleRate;
    Input.bitsPerSample = 16;
    Input.byteRate = sampleRate * channels * 2;
    Input.blockAlign = channels * 2;
    Input.subchunk2Size = dataBytes * 2;
    Input.chunkSize = 36 + Input.subchunk2Size;
  }

  // copy constructor (deep copy)
  Whisper(const Whisper &other) {
    Input = other.Input;
    dataBytes = other.dataBytes;
    if (dataBytes > 0 && other.data != NULL) {
      data = new short[dataBytes];
      for (int i = 0; i < dataBytes; i++)
        data[i] = other.data[i];
    } else {
      data = NULL;
    }
  }

  // destructor

  ~Whisper() {
    delete[] data;
    data = NULL;
  }

  // getters
  short *getData() { return data; }
  int getDataBytes() { return dataBytes; }
  int getSampleRate() { return Input.sampleRate; }
  WavHeader getHeader() { return Input; }

  // provide the updated form

  void updateHeader() {
    Input.subchunk2Size = dataBytes * 2;
    Input.chunkSize = 36 + Input.subchunk2Size;
    Input.bitsPerSample = 16;
    Input.byteRate = Input.sampleRate * Input.numChannels * 2;
    Input.blockAlign = Input.numChannels * 2;
  }

  // save to a .wav file

  void save(const char *path) {
    updateHeader(); // first it will update the path
    fstream file;
    file.open(path, ios::binary | ios::out | ios::trunc);
    if (!file.is_open()) {
      return;
    }
    file.write((char *)&Input, 44);
    file.write((char *)data, dataBytes * 2);
    file.close();
  }

  // phase 2: Time domain operations

  // peak Normalization scale so the loudest sample hits 32767
  /*as said
                   Peak Normalization: Scan the buffer for the maximum value and
     scale the whole buffer such that the scanned peak reached the max value of
     32767. You must make sure no sample exceeds this max and to ensure that you
     must perform hard clipping.

*/
  void normalize() {
    if (this->data == NULL || this->dataBytes == 0) // guard
      return;
    int peak = 0;
    for (int i = 0; i < this->dataBytes; i++) {
      int val = this->data[i];
      if (val < 0)
        val = -val; // negative to positive
      if (val > peak)
        peak = val;
    }
    if (peak == 0)
      return;
    float scale = 32767 / peak;
    for (int i = 0; i < this->dataBytes; i++) {
      float val = this->data[i] * scale;
      if (val > 32767)
        val = 32767;
      if (val < -32768)
        val = -32768;
      this->data[i] = val;
    }
  }

  // gain
  Whisper &operator*=(float factor) {
    for (int i = 0; i < this->dataBytes; i++) {
      float val = this->data[i] * factor;
      if (val > 32767)
        val = 32767;
      if (val < -32768)
        val = -32768;
      this->data[i] = val;
    }
    return *this;
  }

  // reversal — reverse time order per frame (mono: one short per frame;
  // stereo+: keep L/R together)
  void reverse() {
    if (this->data == NULL || this->dataBytes < 2)
      return;

    int ch = Input.numChannels;
    if (ch < 1)
      ch = 1;
    if (this->dataBytes % ch != 0)
      ch = 1;

    int frames = this->dataBytes / ch;
    for (int i = 0; i < frames / 2; i++) {
      int j = frames - 1 - i;
      for (int c = 0; c < ch; c++) {
        int a = i * ch + c;
        int b = j * ch + c;
        short t = this->data[a];
        this->data[a] = this->data[b];
        this->data[b] = t;
      }
    }
  }

  Whisper splice(float startSec, float endSec) {
    if (this->data == NULL || this->dataBytes == 0) {
      short empty = 0;
      return Whisper(&empty, 0, this->Input.sampleRate,
                     this->Input.numChannels);
    }

    int ch = Input.numChannels;
    if (ch < 1)
      ch = 1;
    if (this->dataBytes % ch != 0)
      ch = 1;

    int totalFrames = this->dataBytes / ch;
    int startFrame = (startSec * this->Input.sampleRate);
    int endFrame = (endSec * this->Input.sampleRate);

    if (startFrame < 0)
      startFrame = 0;
    if (startFrame >= totalFrames)
      startFrame = totalFrames - 1;
    if (endFrame > totalFrames)
      endFrame = totalFrames;

    int startSample = startFrame * ch;
    int endSample = endFrame * ch;
    int newSize = endSample - startSample;
    if (newSize <= 0) {
      // return an empty Whisper
      short empty = 0;
      return Whisper(&empty, 0, this->Input.sampleRate,
                     this->Input.numChannels);
    }
    short *arr = new short[newSize];
    for (int i = 0; i < newSize; i++)
      arr[i] = this->data[startSample + i];
    Whisper result(arr, newSize, this->Input.sampleRate,
                   this->Input.numChannels);
    delete[] arr;
    return result;
  }

  Whisper operator+(const Whisper &other) const {
    int newSize = this->dataBytes > other.dataBytes
                      ? this->dataBytes
                      : other.dataBytes; // max size is taken
    short *arr = new short[newSize];
    for (int i = 0; i < newSize; i++) {
      int a = i < this->dataBytes ? this->data[i] : 0;
      int b = i < other.dataBytes ? other.data[i] : 0;
      int mixed = a + b;
      if (mixed > 32767)
        mixed = 32767;
      if (mixed < -32768)
        mixed = -32768;
      arr[i] = mixed;
    }
    Whisper result(
        arr, newSize, this->Input.sampleRate,
        this->Input.numChannels); // constructor make sorley for that assignment
    delete[] arr;
    return result;
  }

  Whisper &operator+=(const Whisper &other) {
    int newSize = this->dataBytes + other.dataBytes;
    short *arr = new short[newSize];
    for (int i = 0; i < this->dataBytes; i++)
      arr[i] = this->data[i];
    for (int i = 0; i < other.dataBytes; i++)
      arr[this->dataBytes + i] = other.data[i];
    delete[] this->data;
    this->data = arr;
    this->dataBytes = newSize;
    this->updateHeader(); // increment + equal
    return *this;
  }

  // phase 3: fft()
  // DECLARATION ONLY here.
  // implementation is below (after this class definition closes)
  // because the body needs the full Spectrum definition.

  Spectrum fft();
};

// Spectrum::ifft() — implementation deferred to here because it
// must construct and return a Whisper object, and Whisper was only
// forward-declared when complex.h was parsed.

Whisper Spectrum::ifft() {
  // since through the gmail of the sir sheryar i learn that part from the
  // google what is ifft and what is colly tucky algoritham since you might get
  // a similar thing as copying the exect words of sir "the intent is logic
  // building , pointers and dynamic memory which i did here you can see while
  // regarding to the algortiham you are encourged to resurch them freely asit
  // comes in learning " so i see it through two parts one
  // https://en.wikipedia.org/wiki/Fast_Fourier_transform yes wiki pidia then i
  // learn the detailed concept through multiple videos i btw didnot use ai
  // which i think if i use so it will decrease time if so i would recomend
  // peplix as a good learner ai as it break but i use a different idea for
  // algoritham i use abdul bari lectures and for these i use online videos as
  // shown that inspire me to get delve into it then i code here you may see
  // similarity and i will say it is because of learning the same algoritham i
  // would btw recoment you https://youtu.be/h7apO7q16V0?si=jo4PDzlRZgaFVwHU
  // that video as it was very useful for me

  double pi = acos(-1.0);
  int n = paddedSize;
  Complex *tempBins = new Complex[n];

  // Conjugate
  for (int i = 0; i < n; i++) {
    tempBins[i].setIt(outputFreq[i].getReal(), -outputFreq[i].getImagnary());
  }

  // bit reversal permutation
  int bits = 0;
  int tmpSize = n;
  while (tmpSize > 1) {
    bits++;
    tmpSize /= 2;
  }

  for (int i = 0; i < n; i++) {
    int rev = 0, x = i;
    for (int j = 0; j < bits; j++) {
      rev = (rev << 1) | (x & 1);
      x >>= 1;
    }
    if (rev > i) {
      Complex tmpC = tempBins[i];
      tempBins[i] = tempBins[rev];
      tempBins[rev] = tmpC;
    }
  }

  // butterfly  negative angle which conjugate trick that  reuses forward FFT
  for (int len = 2; len <= n; len *= 2) {
    double angle = -2.0 * pi / len;
    Complex wlen(cos(angle), sin(angle));
    for (int i = 0; i < n; i += len) {
      Complex w(1.0, 0.0);
      for (int j = 0; j < len / 2; j++) {
        Complex u = tempBins[i + j];
        Complex v = tempBins[i + j + len / 2] * w;
        tempBins[i + j] = u + v;
        tempBins[i + j + len / 2] = u - v;
        w = w * wlen;
      }
    }
  }

  // trim to original size, divide by n, hard clip
  short *resultSamples = new short[original];
  for (int i = 0; i < original; i++) {
    double val = tempBins[i].getReal() / n;
    if (val > 32767.0)
      val = 32767.0;
    if (val < -32768.0)
      val = -32768.0;
    resultSamples[i] = val;
  }

  Whisper w(resultSamples, original, sampleRate, numChannels);
  delete[] resultSamples;
  delete[] tempBins;
  return w;
}
// same work for here so
Spectrum Whisper::fft() {
  double pi = acos(-1.0);
  int n = dataBytes;

  int paddedSize = 1;
  while (paddedSize < n)
    paddedSize *= 2;

  Complex *bins = new Complex[paddedSize];
  for (int i = 0; i < n; i++)
    bins[i] = Complex(data[i], 0.0);
  for (int i = n; i < paddedSize; i++)
    bins[i] = Complex(0.0, 0.0); // zero pad

  // bit reversal permutation
  int bits = 0, tmp = paddedSize;
  while (tmp > 1) {
    bits++;
    tmp /= 2;
  }

  for (int i = 0; i < paddedSize; i++) {
    int rev = 0, x = i;
    for (int j = 0; j < bits; j++) {
      rev = (rev << 1) | (x & 1);
      x >>= 1;
    }
    if (rev > i) {
      Complex t = bins[i];
      bins[i] = bins[rev];
      bins[rev] = t;
    }
  }

  // cooley Tukey butterfly for that i follow that goated lecture
  // https://youtu.be/mPVtovydY1k?si=1uJuWAPAUioTpDke that explain me about it
  // so the main learning go through there
  for (int len = 2; len <= paddedSize; len *= 2) {
    double angle = -2.0 * pi / len;
    Complex wlen(cos(angle), sin(angle));
    for (int i = 0; i < paddedSize; i += len) {
      Complex w(1.0, 0.0);
      for (int j = 0; j < len / 2; j++) {
        Complex u = bins[i + j];
        Complex v = bins[i + j + len / 2] * w;
        bins[i + j] = u + v;
        bins[i + j + len / 2] = u - v;
        w = w * wlen;
      }
    }
  }

  // orignal = dataBytes so IFFT can restore exactly
  Spectrum result(bins, paddedSize, n, Input.sampleRate, Input.numChannels);
  delete[] bins;
  return result;
}

class Orchestrator {
private:
  Whisper *whispers[4];
  Spectrum *spectra[4];
  int activeW;
  int activeS;

  int readInt(const char *input) {
    cout << input;
    int val;
    cin >> val;
    if (cin.fail()) { // it is use to handle the cases when the user enter abc
                      // but we demand 123 so in that case in order to get red
                      // of that we do that
      cin.clear(); // it happens with me when i input cricket12a now a creates a
                   // flag and stream get distubed in all that buffer be
                   // assigned which can be true through cin.clear
      char character;
      while (cin.get(character) && character != '\n')
        ; // clearing through buffer
      return -9999;
    }
    return val;
  }

  float readFloat(const char *input) {
    cout << input;
    float val;
    cin >> val;
    if (cin.fail()) {
      cin.clear(); // same motivation as above
      char character;
      while (cin.get(character) && character != '\n')
        ;
      return -9999;
    }
    return val;
  }

  void readPath(const char *input, char *buf, int size) {
    cout << input; // here that is specially designed for the path one i have
                   // use some of the iostream functions for that
    char character;
    while (cin.peek() == '\n' ||
           cin.peek() == '\r') // use to look at the next character of buffer
      cin.get(character); // if it is \n or \r (\r comes if you read it without
                          // binary through file )
    int i = 0;
    while (i < size - 1) {
      character = cin.get(); // as cin.get returns an integer therfore casting
                             // it
      if (character == '\n' || character == '\r')
        break;
      buf[i] = character;
      i++;
    }
    buf[i] = '\0';
    /*string readPath(const string& prompt) {
cout << prompt;
string input;
getline(cin, input);
return input;
an alternatvie way but since string were not allowed
}*/
  }

  void line() { cout << "--------------------------------------------\n"; }

  void printSlots() {
    line();
    cout << "Whisper slots:\n";
    for (int i = 0; i < 4; i++) {
      cout << "  [" << i << "] ";
      if (whispers[i] == NULL)
        cout << "EMPTY\n";
      else {
        WavHeader h = whispers[i]->getHeader();
        cout << "LOADED  samples=" << whispers[i]->getDataBytes()
             << "  sampleRate=" << h.sampleRate << "  ch=" << h.numChannels;
        if (activeW == i)
          cout << "  <-- ACTIVE";
        cout << "\n";
      }
    }
    cout << "Spectrum slots:\n";
    for (int i = 0; i < 4; i++) {
      cout << "  [" << i << "] ";
      if (spectra[i] == NULL)
        cout << "EMPTY\n";
      else {
        cout << "LOADED  paddedSize=" << spectra[i]->getPaddedSize()
             << "  origSize=" << spectra[i]->getOriginal()
             << "  sampleRate=" << spectra[i]->getSampleRate();
        if (activeS == i)
          cout << "  <-- ACTIVE";
        cout << "\n";
      }
    }
    line();
  }

  void loadTrack() {
    printSlots();
    int slot = readInt("Enter Whisper slot to load into (0-3): ");
    if (slot < 0 || slot > 3) {
      cout << "Error: invalid slot. Must be 0-3.\n";
      return;
    }
    char path[512];
    readPath("Enter path to .wav file: ", path, 512);

    Whisper *w = new Whisper(path);
    if (w->getData() == NULL) {
      cout << "Error: failed to load audio from \"" << path << "\".\n";
      delete w;
      return;
    }

    if (whispers[slot] != NULL) {
      delete whispers[slot];
      whispers[slot] = NULL;
      cout << "Old Whisper in slot " << slot << " replaced.\n";
    }
    whispers[slot] = w;
    cout << "Loaded into Whisper slot " << slot << "  (" << w->getDataBytes()
         << " samples, " << w->getSampleRate() << " Hz)\n";
  }

  void selectActiveWhisper() {
    printSlots();
    int slot = readInt("Select active Whisper slot (0-3): ");
    if (slot < 0 || slot > 3) {
      cout << "Error: invalid slot.\n";
      return;
    }
    if (whispers[slot] == NULL) {
      cout << "Error: slot " << slot << " is empty. Load a track first.\n";
      return;
    }
    activeW = slot;
    cout << "Active Whisper is now slot " << slot << ".\n";
  }

  void selectActiveSpectrum() {
    printSlots();
    int slot = readInt("Select active Spectrum slot (0-3): ");
    if (slot < 0 || slot > 3) {
      cout << "Error: invalid slot.\n";
      return;
    }
    if (spectra[slot] == NULL) {
      cout << "Error: slot " << slot
           << " is empty. Run FFT on a Whisper first.\n";
      return;
    }
    activeS = slot;
    cout << "Active Spectrum is now slot " << slot << ".\n";
  }

  void saveActiveWhisper() {
    if (activeW == -1 || whispers[activeW] == NULL) {
      cout << "Error: no active Whisper.\n";
      return;
    }
    char path[512];
    readPath("Enter output path (e.g. out.wav): ", path, 512);
    whispers[activeW]->save(path);
  }

  void timeDomainMenu() {
    if (activeW == -1 || whispers[activeW] == NULL) {
      cout << "Error: no active Whisper. Please select one first.\n";
      return;
    }

    bool running = true;
    while (running) {
      line();
      cout << "TIME DOMAIN MENU  (active Whisper slot " << activeW << ")\n";
      line();
      cout << "  1. Normalize (Peak Normalization)\n";
      cout << "  2. Gain (scale volume)\n";
      cout << "  3. Reverse\n";
      cout << "  4. Splice (extract region into new Whisper slot)\n";
      cout << "  5. Mix (sum with another Whisper into new slot)\n";
      cout << "  6. Concatenate (append another Whisper to active)\n";
      cout << "  7. Save active Whisper to file\n";
      cout << "  8. Run FFT on active Whisper -> Spectrum slot\n";
      cout << "  0. Back to main menu\n";
      line();

      int choice = readInt("Choice: ");

      if (choice == 0) {
        running = false;
      } else if (choice == 1) {
        whispers[activeW]->normalize();
        cout << "Normalized Whisper in slot " << activeW << ".\n";
      } else if (choice == 2) {
        float g =
            readFloat("Enter gain factor (e.g. 2.0 = double, 0.5 = halve): ");
        if (g == -9999) {
          cout << "Error: invalid gain value.\n";
        } else {
          *whispers[activeW] *= g;
          cout << "Applied gain " << g << " to slot " << activeW << ".\n";
        }
      } else if (choice == 3) {
        whispers[activeW]->reverse();
        cout << "Reversed Whisper in slot " << activeW << ".\n";
      } else if (choice == 4) {
        float startSec = readFloat("Start time (seconds): ");
        if (startSec == -9999) {
          cout << "Error: invalid start.\n";
          continue;
        }
        float endSec = readFloat("End time (seconds): ");
        if (endSec == -9999) {
          cout << "Error: invalid end.\n";
          continue;
        }
        if (endSec <= startSec) {
          cout << "Error: end must be > start.\n";
          continue;
        }

        int destSlot = readInt("Destination Whisper slot (0-3): ");
        if (destSlot < 0 || destSlot > 3) {
          cout << "Error: invalid slot.\n";
          continue;
        }

        Whisper spliced = whispers[activeW]->splice(startSec, endSec);

        if (whispers[destSlot] != NULL) {
          delete whispers[destSlot];
          cout << "Old Whisper in slot " << destSlot << " replaced.\n";
        }
        whispers[destSlot] = new Whisper(spliced);
        cout << "Spliced [" << startSec << "s - " << endSec << "s] into slot "
             << destSlot << " (" << whispers[destSlot]->getDataBytes()
             << " samples).\n";
      } else if (choice == 5) {
        printSlots();
        int otherSlot = readInt("Slot of second Whisper to mix with: ");
        if (otherSlot < 0 || otherSlot > 3) {
          cout << "Error: invalid slot.\n";
          continue;
        }
        if (whispers[otherSlot] == NULL) {
          cout << "Error: slot " << otherSlot << " is empty.\n";
          continue;
        }

        int destSlot = readInt("Destination slot for mixed result (0-3): ");
        if (destSlot < 0 || destSlot > 3) {
          cout << "Error: invalid slot.\n";
          continue;
        }

        Whisper mixed = *whispers[activeW] + *whispers[otherSlot];
        if (whispers[destSlot] != NULL) {
          delete whispers[destSlot];
          cout << "Old Whisper in slot " << destSlot << " replaced.\n";
        }
        whispers[destSlot] = new Whisper(mixed);
        cout << "Mixed slots " << activeW << " + " << otherSlot << " into slot "
             << destSlot << ".\n";
      } else if (choice == 6) {
        printSlots();
        int otherSlot = readInt("Slot of Whisper to append to active: ");
        if (otherSlot < 0 || otherSlot > 3) {
          cout << "Error: invalid slot.\n";
          continue;
        }
        if (whispers[otherSlot] == NULL) {
          cout << "Error: slot " << otherSlot << " is empty.\n";
          continue;
        }

        *whispers[activeW] += *whispers[otherSlot];
        cout << "Appended slot " << otherSlot << " onto slot " << activeW
             << "  (now " << whispers[activeW]->getDataBytes()
             << " samples).\n";
      } else if (choice == 7) {
        saveActiveWhisper();
      } else if (choice == 8) {
        int destSlot =
            readInt("Destination Spectrum slot for FFT output (0-3): ");
        if (destSlot < 0 || destSlot > 3) {
          cout << "Error: invalid slot.\n";
          continue;
        }

        if (spectra[destSlot] != NULL) {
          delete spectra[destSlot];
          cout << "Old Spectrum in slot " << destSlot << " replaced.\n";
        }

        cout << "Running FFT on Whisper slot " << activeW << "...\n";
        Spectrum s = whispers[activeW]->fft();
        spectra[destSlot] = new Spectrum(s);
        cout << "FFT done. Spectrum stored in slot " << destSlot
             << "  (paddedSize=" << spectra[destSlot]->getPaddedSize()
             << ").\n";
      } else {
        cout << "Error: invalid choice. Enter 0-8.\n";
      }
    }
  }

  void freqDomainMenu() {
    if (activeS == -1 || spectra[activeS] == NULL) {
      cout << "Error: no active Spectrum. Run FFT on a Whisper first.\n";
      return;
    }

    bool running = true;
    while (running) {
      line();
      cout << "FREQUENCY DOMAIN MENU  (active Spectrum slot " << activeS
           << ")\n";
      line();
      cout << "  1. Apply Low-Pass filter\n";
      cout << "  2. Apply High-Pass filter\n";
      cout << "  3. Apply Band-Pass filter\n";
      cout << "  4. Run IFFT (Spectrum -> new Whisper slot)\n";
      cout << "  0. Back to main menu\n";
      line();

      int choice = readInt("Choice: ");

      if (choice == 0) {
        running = false;
      } else if (choice == 1 || choice == 2 || choice == 3) {
        int ps = spectra[activeS]->getPaddedSize();
        int sr = spectra[activeS]->getSampleRate();

        Filter filt(ps);

        if (choice == 1) {
          float cutoff = readFloat("Enter cutoff frequency (Hz): ");
          if (cutoff == -9999 || cutoff <= 0) {
            cout << "Error: invalid cutoff.\n";
            continue;
          }
          if (cutoff > sr / 2) {
            cout << "Warning: cutoff (" << cutoff << " Hz) exceeds Nyquist ("
                 << sr / 2 << " Hz). Clamping.\n";
            cutoff = (sr / 2);
          }
          // FIX 2: removed extra chan argument
          filt.lowPass(sr, ps, cutoff);
          spectra[activeS]->applyFilter(filt);
          cout << "Low-pass filter at " << cutoff
               << " Hz applied to Spectrum slot " << activeS << ".\n";
        } else if (choice == 2) {
          float cutoff = readFloat("Enter cutoff frequency (Hz): ");
          if (cutoff == -9999 || cutoff < 0) {
            cout << "Error: invalid cutoff.\n";
            continue;
          }
          if (cutoff > sr / 2) {
            cout << "Warning: cutoff exceeds Nyquist. Clamping.\n";
            cutoff = (sr / 2);
          }
          // FIX 2: removed extra chan argument
          filt.highPass(sr, ps, cutoff);
          spectra[activeS]->applyFilter(filt);
          cout << "High-pass filter at " << cutoff
               << " Hz applied to Spectrum slot " << activeS << ".\n";
        } else {
          float lowHz = readFloat("Enter lower bound frequency (Hz): ");
          if (lowHz == -9999 || lowHz < 0) {
            cout << "Error: invalid lower bound.\n";
            continue;
          }
          float highHz = readFloat("Enter upper bound frequency (Hz): ");
          if (highHz == -9999 || highHz <= lowHz) {
            cout << "Error: upper bound must be > lower bound.\n";
            continue;
          }
          if (highHz > sr / 2) {
            cout << "Warning: upper bound exceeds Nyquist. Clamping.\n";
            highHz = (sr / 2);
          }

          filt.bandPass(sr, ps, lowHz, highHz);
          spectra[activeS]->applyFilter(filt);
          cout << "Band-pass filter [" << lowHz << " Hz - " << highHz
               << " Hz] applied to Spectrum slot " << activeS << ".\n";
        }
      } else if (choice == 4) {
        int destSlot =
            readInt("Destination Whisper slot for IFFT output (0-3): ");
        if (destSlot < 0 || destSlot > 3) {
          cout << "Error: invalid slot.\n";
          continue;
        }

        cout << "Running IFFT on Spectrum slot " << activeS << "...\n";
        Whisper w = spectra[activeS]->ifft();

        if (whispers[destSlot] != NULL) {
          delete whispers[destSlot];
          cout << "Old Whisper in slot " << destSlot << " replaced.\n";
        }
        whispers[destSlot] = new Whisper(w);
        cout << "IFFT done. Whisper stored in slot " << destSlot << "  ("
             << whispers[destSlot]->getDataBytes() << " samples).\n";
      } else {
        cout << "Error: invalid choice. Enter 0-4.\n";
      }
    }
  }

public:
  Orchestrator() {
    for (int i = 0; i < 4; i++) {
      whispers[i] = NULL;
      spectra[i] = NULL;
    }
    activeW = -1;
    activeS = -1;
  }

  Orchestrator(const Orchestrator &other) {
    activeW = other.activeW;
    activeS = other.activeS;
    for (int i = 0; i < 4; i++) {
      whispers[i] =
          (other.whispers[i] != NULL) ? new Whisper(*other.whispers[i]) : NULL;
      spectra[i] =
          (other.spectra[i] != NULL) ? new Spectrum(*other.spectra[i]) : NULL;
    }
  }

  ~Orchestrator() {
    for (int i = 0; i < 4; i++) {
      delete whispers[i];
      whispers[i] = NULL;
      delete spectra[i];
      spectra[i] = NULL;
    }
  }

  void run() {
    cout << "\n";
    cout << "  THE MASTER OF WHISPERERS — DSP ENGINE\n";
    cout << "\n";

    bool running = true;
    while (running) {
      printSlots();
      cout << "MAIN MENU\n";
      line();
      cout << "  1. Load .wav file into Whisper slot\n";
      cout << "  2. Select active Whisper slot\n";
      cout << "  3. Select active Spectrum slot\n";
      cout << "  4. Time Domain operations\n";
      cout << "  5. Frequency Domain operations\n";
      cout << "  6. Free a Whisper slot\n";
      cout << "  7. Free a Spectrum slot\n";
      cout << "  0. Exit\n";
      line();

      int choice = readInt("Choice: ");

      if (choice == 0) {
        running = false;
        cout << "The End\n";
      } else if (choice == 1) {
        loadTrack();
      } else if (choice == 2) {
        selectActiveWhisper();
      } else if (choice == 3) {
        selectActiveSpectrum();
      } else if (choice == 4) {
        timeDomainMenu();
      } else if (choice == 5) {
        freqDomainMenu();
      } else if (choice == 6) {
        int slot = readInt("Whisper slot to free (0-3): ");
        if (slot < 0 || slot > 3) {
          cout << "Error: invalid slot.\n";
          continue;
        }
        if (whispers[slot] == NULL) {
          cout << "Slot " << slot << " is already empty.\n";
          continue;
        }
        delete whispers[slot];
        whispers[slot] = NULL;
        if (activeW == slot)
          activeW = -1;
        cout << "Whisper slot " << slot << " freed.\n";
      } else if (choice == 7) {
        int slot = readInt("Spectrum slot to free (0-3): ");
        if (slot < 0 || slot > 3) {
          cout << "Error: invalid slot.\n";
          continue;
        }
        if (spectra[slot] == NULL) {
          cout << "Slot " << slot << " is already empty.\n";
          continue;
        }
        delete spectra[slot];
        spectra[slot] = NULL;
        if (activeS == slot)
          activeS = -1;
        cout << "Spectrum slot " << slot << " freed.\n";
      } else {
        cout << "Error: invalid choice. Enter 0-7.\n";
      }
    }
  }
};