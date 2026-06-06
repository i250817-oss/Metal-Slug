#pragma once
/*
 * VoiceRecognitionEngine — "The Voice of the Bene Gesserit"
 *
 * Startup engine that listens for "Valar Morghulis".
 * Pipeline:
 *   1. Load reference WAV  -> FFT -> magnitude spectrum (template)
 *   2. Capture mic audio   -> FFT -> magnitude spectrum (query)
 *   3. Cosine similarity between template and query
 *   4. similarity >= 0.40  -> "Command Detected Successfully" popup
 *
 * Uses Whisper/Spectrum/Complex from assignement q4.h for all FFT work.
 * Uses sf::SoundBufferRecorder for microphone capture.
 */

#include "../Character System/assignement q4.h"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <cstdio>
#include <iostream>

using namespace sf;
using namespace std;

class VoiceRecognitionEngine {
public:
    static const int   CAPTURE_SECONDS = 4;
    static const int   SAMPLE_RATE     = 44100;

    VoiceRecognitionEngine() : refMag(nullptr), refSize(0), ready(false) {}

    ~VoiceRecognitionEngine() {
        delete[] refMag;
    }

    // Load + FFT the reference WAV. Call once at startup.
    bool loadReference(const char* wavPath) {
        cout << "[VRE] Loading reference: " << wavPath << "\n";

        Whisper ref(wavPath);
        if (ref.getData() == nullptr || ref.getDataBytes() == 0) {
            cout << "[VRE] ERROR: Could not read WAV.\n";
            return false;
        }

        delete[] refMag;
        refMag = magnitudeSpectrum(ref, refSize);

        cout << "[VRE] Reference decomposed: " << refSize << " bins.\n";
        ready = true;
        return true;
    }

    // Record CAPTURE_SECONDS from mic, compare to reference, show popup.
    // Returns true if the phrase was detected (similarity >= 0.40).
    bool listenAndMatch(RenderWindow& window) {
        if (!ready) {
            cout << "[VRE] No reference loaded.\n";
            return false;
        }
        if (!SoundBufferRecorder::isAvailable()) {
            cout << "[VRE] ERROR: No microphone available.\n";
            return false;
        }

        Font font;
        bool fontOK = font.loadFromFile("Sprites/font.ttf") ||
                      font.loadFromFile(
                          "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf") ||
                      font.loadFromFile(
                          "/usr/share/fonts/truetype/liberation/"
                          "LiberationSans-Regular.ttf");

        drawListeningOverlay(window, font, fontOK);

        // Record
        SoundBufferRecorder recorder;
        recorder.start(SAMPLE_RATE);
        cout << "[VRE] Recording " << CAPTURE_SECONDS << "s...\n";

        Clock capClock;
        while (capClock.getElapsedTime().asSeconds() < (float)CAPTURE_SECONDS
               && window.isOpen()) {
            Event ev;
            while (window.pollEvent(ev))
                if (ev.type == Event::Closed) window.close();

            float progress = capClock.getElapsedTime().asSeconds()
                             / (float)CAPTURE_SECONDS;
            drawProgress(window, font, fontOK, progress);
        }
        recorder.stop();

        const SoundBuffer& buf = recorder.getBuffer();
        int sampleCount = (int)buf.getSampleCount();
        int sampleRate  = (int)buf.getSampleRate();
        int channels    = (int)buf.getChannelCount();

        cout << "[VRE] Captured " << sampleCount << " samples.\n";

        if (sampleCount == 0) {
            cout << "[VRE] Empty capture.\n";
            return false;
        }

        // Build Whisper from captured raw samples
        short* raw = const_cast<short*>(buf.getSamples());
        Whisper captured(raw, sampleCount, sampleRate, channels);

        int capSize = 0;
        float* capMag = magnitudeSpectrum(captured, capSize);

        float similarity = cosineSimilarity(refMag, refSize, capMag, capSize);
        delete[] capMag;

        cout << "[VRE] Similarity: " << similarity
             << " (threshold 0.40)\n";

        bool matched = (similarity >= 0.40f);
        if (matched)
            showSuccessPopup(window, font, fontOK);
        else
            showFailPopup(window, font, fontOK, similarity);

        return matched;
    }

    bool isReady() const { return ready; }

private:
    float* refMag;
    int    refSize;
    bool   ready;

    // ── FFT helpers ──────────────────────────────────────────────────────────

    // Returns heap-allocated magnitude array; caller deletes[].
    float* magnitudeSpectrum(Whisper& w, int& outSize) {
        Spectrum sp   = w.fft();
        outSize       = sp.getPaddedSize();
        Complex* bins = sp.getOutputFreq();

        float* mag = new float[outSize];
        for (int i = 0; i < outSize; i++) {
            double r  = bins[i].getReal();
            double im = bins[i].getImagnary();
            mag[i]    = (float)sqrt(r * r + im * im);
        }
        return mag;
    }

    float cosineSimilarity(float* a, int na, float* b, int nb) {
        int n = (na < nb) ? na : nb;
        if (n == 0) return 0.0f;

        // Focus comparison strictly on the speech frequency band (150 Hz to 3500 Hz)
        // This eliminates high-frequency silence/white noise and low-frequency hum/DC offset.
        int minBin = (int)(150.0 * n / 44100.0);
        int maxBin = (int)(3500.0 * n / 44100.0);
        if (maxBin > n) maxBin = n;
        if (minBin < 0) minBin = 0;

        double dot   = 0;
        double normA = 0;
        double normB = 0;
        for (int i = minBin; i < maxBin; i++) {
            dot   += (double)a[i] * b[i];
            normA += (double)a[i] * a[i];
            normB += (double)b[i] * b[i];
        }
        if (normA == 0.0 || normB == 0.0) return 0.0f;
        return (float)(dot / (sqrt(normA) * sqrt(normB)));
    }





    // ── Drawing helpers ──────────────────────────────────────────────────────

    void drawListeningOverlay(RenderWindow& window, Font& font, bool fontOK) {
        RectangleShape bg(Vector2f((float)window.getSize().x,
                                  (float)window.getSize().y));
        bg.setFillColor(Color(0, 0, 0, 200));

        window.draw(bg);

        if (fontOK) {
            Text t;
            t.setFont(font);
            t.setString("Say: \"Valar Morghulis\"");
            t.setCharacterSize(34);
            t.setFillColor(Color(100, 220, 255));
            t.setStyle(Text::Bold);
            FloatRect lb = t.getLocalBounds();
            t.setOrigin(lb.width / 2.f, lb.height / 2.f);
            t.setPosition((float)window.getSize().x / 2.f,
                          (float)window.getSize().y / 2.f);
            window.draw(t);
        }
        window.display();
    }

    void drawProgress(RenderWindow& window, Font& font, bool fontOK,
                      float progress) {
        RectangleShape bg(Vector2f((float)window.getSize().x,
                                  (float)window.getSize().y));
        bg.setFillColor(Color(0, 0, 0, 200));
        window.draw(bg);

        if (fontOK) {
            Text t;
            t.setFont(font);
            t.setString("Say: \"Valar Morghulis\"");
            t.setCharacterSize(34);
            t.setFillColor(Color(100, 220, 255));
            t.setStyle(Text::Bold);
            FloatRect lb = t.getLocalBounds();
            t.setOrigin(lb.width / 2.f, lb.height / 2.f);
            t.setPosition((float)window.getSize().x / 2.f,
                          (float)window.getSize().y / 2.f);
            window.draw(t);
        }

        // Progress bar
        float barW = (float)window.getSize().x * progress;
        RectangleShape bar(Vector2f(barW, 6.f));
        bar.setFillColor(Color(100, 220, 255, 200));
        bar.setPosition(0.f, (float)window.getSize().y - 8.f);
        window.draw(bar);
        window.display();
    }

    void showSuccessPopup(RenderWindow& window, Font& font, bool fontOK) {
        const float SHOW = 2.5f;
        Clock c;

        while (c.getElapsedTime().asSeconds() < SHOW && window.isOpen()) {
            Event ev;
            while (window.pollEvent(ev))
                if (ev.type == Event::Closed) window.close();

            float t     = c.getElapsedTime().asSeconds();
            float alpha = 1.f;
            if (t > SHOW - 0.5f) alpha = (SHOW - t) / 0.5f;
            Uint8 a = (Uint8)(255 * alpha);

            RectangleShape panel(Vector2f(720.f, 130.f));
            panel.setFillColor(Color(8, 28, 8, (Uint8)(220 * alpha)));
            panel.setOutlineColor(Color(80, 240, 80, a));
            panel.setOutlineThickness(2.f);
            panel.setOrigin(360.f, 65.f);
            panel.setPosition((float)window.getSize().x / 2.f,
                              (float)window.getSize().y / 2.f);
            window.draw(panel);

            if (fontOK) {
                Text title;
                title.setFont(font);
                title.setString("Command Detected Successfully");
                title.setCharacterSize(30);
                title.setFillColor(Color(100, 255, 100, a));
                title.setStyle(Text::Bold);
                FloatRect tb = title.getLocalBounds();
                title.setOrigin(tb.width / 2.f, tb.height / 2.f);
                title.setPosition((float)window.getSize().x / 2.f,
                                  (float)window.getSize().y / 2.f - 18.f);
                window.draw(title);

                Text sub;
                sub.setFont(font);
                sub.setString("\"Valar Morghulis\" - All men must die.");
                sub.setCharacterSize(18);
                sub.setFillColor(Color(160, 255, 160, a));
                FloatRect sb = sub.getLocalBounds();
                sub.setOrigin(sb.width / 2.f, sb.height / 2.f);
                sub.setPosition((float)window.getSize().x / 2.f,
                                (float)window.getSize().y / 2.f + 22.f);
                window.draw(sub);
            }
            window.display();
        }
    }

    void showFailPopup(RenderWindow& window, Font& font, bool fontOK,
                       float similarity) {
        const float SHOW = 2.0f;
        Clock c;

        while (c.getElapsedTime().asSeconds() < SHOW && window.isOpen()) {
            Event ev;
            while (window.pollEvent(ev))
                if (ev.type == Event::Closed) window.close();

            RectangleShape panel(Vector2f(600.f, 100.f));
            panel.setFillColor(Color(28, 8, 8, 220));
            panel.setOutlineColor(Color(200, 60, 60));
            panel.setOutlineThickness(2.f);
            panel.setOrigin(300.f, 50.f);
            panel.setPosition((float)window.getSize().x / 2.f,
                              (float)window.getSize().y / 2.f);
            window.draw(panel);

            if (fontOK) {
                Text title;
                title.setFont(font);
                title.setString("Phrase Not Recognised");
                title.setCharacterSize(28);
                title.setFillColor(Color(255, 100, 100));
                FloatRect tb = title.getLocalBounds();
                title.setOrigin(tb.width / 2.f, tb.height / 2.f);
                title.setPosition((float)window.getSize().x / 2.f,
                                  (float)window.getSize().y / 2.f - 14.f);
                window.draw(title);

                char buf[64];
                snprintf(buf, sizeof(buf), "Match: %.0f%%  (need 40%%)",
                         similarity * 100.f);
                Text sub;
                sub.setFont(font);
                sub.setString(buf);
                sub.setCharacterSize(18);
                sub.setFillColor(Color(200, 140, 140));
                FloatRect sb = sub.getLocalBounds();
                sub.setOrigin(sb.width / 2.f, sb.height / 2.f);
                sub.setPosition((float)window.getSize().x / 2.f,
                                (float)window.getSize().y / 2.f + 20.f);
                window.draw(sub);
            }
            window.display();
        }
    }
};
