#!/bin/bash
set -e
echo "Building libwebrtc_ns.so for ARM64..."
JNI_DIR=$(dirname $(find /usr/lib/jvm -name "jni.h" | head -n 1))
gcc -O3 -shared -fPIC -Wl,-soname,libwebrtc_ns.so \
  -I"$JNI_DIR" -I"$JNI_DIR/linux" \
  -I. \
  audio_denoise_jni.c noise_suppression.c \
  -o ../lib/arm64-v8a/libwebrtc_ns.so -lm
strip --strip-unneeded ../lib/arm64-v8a/libwebrtc_ns.so
echo "Done! Output: lib/arm64-v8a/libwebrtc_ns.so"
