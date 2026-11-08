package com.jarvis.assistant.audio;

public final class AudioDenoiseFilter {
    public static final AudioDenoiseFilter INSTANCE = new AudioDenoiseFilter();

    // 2nd-order Butterworth High-Pass Filter (Cutoff 180 Hz @ 16 kHz)
    private static final float B0 = 0.95124542f;
    private static final float B1 = -1.90249084f;
    private static final float B2 = 0.95124542f;
    private static final float A1 = -1.90011242f;
    private static final float A2 = 0.90486927f;

    // Filter memory state
    private static float x1 = 0.0f;
    private static float x2 = 0.0f;
    private static float y1 = 0.0f;
    private static float y2 = 0.0f;

    // Soft-knee noise gate threshold
    private static final float GATE_THRESHOLD = 250.0f;

    private AudioDenoiseFilter() {}

    public static final void processBuffer(byte[] audioData, int length) {
        if (audioData == null || length <= 1) {
            return;
        }

        try {
            int numSamples = length / 2;
            float lx1 = x1;
            float lx2 = x2;
            float ly1 = y1;
            float ly2 = y2;

            for (int i = 0; i < numSamples; i++) {
                int byteIdx = i * 2;
                // Read 16-bit PCM little endian
                int low = audioData[byteIdx] & 0xFF;
                int high = audioData[byteIdx + 1];
                short sample = (short) ((high << 8) | low);

                // Step 1: 180 Hz High-Pass Filter
                float x = (float) sample;
                float y = B0 * x + B1 * lx1 + B2 * lx2 - A1 * ly1 - A2 * ly2;
                lx2 = lx1;
                lx1 = x;
                ly2 = ly1;
                ly1 = y;

                // Step 2: Soft Noise Gate Expander
                float absY = (y >= 0.0f) ? y : -y;
                if (absY < GATE_THRESHOLD) {
                    float ratio = absY / GATE_THRESHOLD;
                    y = y * (ratio * ratio);
                }

                // Step 3: Hard Clamping
                if (y > 32767.0f) y = 32767.0f;
                else if (y < -32768.0f) y = -32768.0f;

                short out = (short) y;
                // Write back 16-bit PCM little endian
                audioData[byteIdx] = (byte) (out & 0xFF);
                audioData[byteIdx + 1] = (byte) ((out >> 8) & 0xFF);
            }

            x1 = lx1;
            x2 = lx2;
            y1 = ly1;
            y2 = ly2;
        } catch (Throwable ignored) {
        }
    }
}
