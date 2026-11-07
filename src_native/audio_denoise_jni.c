#include <jni.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "/tmp/ns_repo/webrtc_ns/src/main/include/noise_suppression.h"

typedef struct {
    NsHandle *ns;
    float x1, x2;
    float y1, y2;
} FilterState;

static const float b0 = 0.95124542f;
static const float b1 = -1.90249084f;
static const float b2 = 0.95124542f;
static const float a1 = -1.90011242f;
static const float a2 = 0.90486927f;

JNIEXPORT jlong JNICALL
Java_com_jarvis_assistant_audio_AudioDenoiseFilter_create(JNIEnv *env, jclass clazz, jint sampleRate, jint mode) {
    FilterState *st = (FilterState *)calloc(1, sizeof(FilterState));
    if (!st) return 0;
    
    st->ns = WebRtcNs_Create();
    if (st->ns) {
        if (WebRtcNs_Init(st->ns, (uint32_t)sampleRate) != 0 ||
            WebRtcNs_set_policy(st->ns, mode) != 0) {
            WebRtcNs_Free(st->ns);
            st->ns = NULL;
        }
    }
    return (jlong)(intptr_t)st;
}

JNIEXPORT void JNICALL
Java_com_jarvis_assistant_audio_AudioDenoiseFilter_process(JNIEnv *env, jclass clazz, jlong handle, jbyteArray audioData, jint length) {
    FilterState *st = (FilterState *)(intptr_t)handle;
    if (!st || !audioData || length <= 0) return;
    
    jbyte *bytes = (*env)->GetByteArrayElements(env, audioData, NULL);
    if (!bytes) return;
    
    int numSamples = length / 2;
    int16_t *samples = (int16_t *)bytes;
    
    // Step 1: 2nd-order Butterworth High-Pass Filter (Cutoff 180 Hz)
    float x1 = st->x1, x2 = st->x2;
    float y1 = st->y1, y2 = st->y2;
    
    for (int i = 0; i < numSamples; i++) {
        float x = (float)samples[i];
        float y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
        x2 = x1; x1 = x;
        y2 = y1; y1 = y;
        
        if (y > 32767.0f) y = 32767.0f;
        else if (y < -32768.0f) y = -32768.0f;
        samples[i] = (int16_t)y;
    }
    st->x1 = x1; st->x2 = x2;
    st->y1 = y1; st->y2 = y2;
    
    // Step 2: WebRTC Noise Suppression (160 samples per 10ms chunk)
    if (st->ns) {
        int pos = 0;
        int16_t outFrame[160];
        while (pos + 160 <= numSamples) {
            const int16_t *spframe = &samples[pos];
            int16_t *outPtr = outFrame;
            
            WebRtcNs_Process(st->ns, &spframe, 1, &outPtr);
            memcpy(&samples[pos], outFrame, 160 * sizeof(int16_t));
            pos += 160;
        }
    }
    
    (*env)->ReleaseByteArrayElements(env, audioData, bytes, 0);
}

JNIEXPORT void JNICALL
Java_com_jarvis_assistant_audio_AudioDenoiseFilter_free(JNIEnv *env, jclass clazz, jlong handle) {
    FilterState *st = (FilterState *)(intptr_t)handle;
    if (st) {
        if (st->ns) {
            WebRtcNs_Free(st->ns);
        }
        free(st);
    }
}
