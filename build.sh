#!/bin/bash
set -e

echo "=== Building Yunay AI Assistant APK ==="
python3 -c "
import zipfile, os

out_apk = 'build_unaligned.apk'
if os.path.exists(out_apk): os.remove(out_apk)

with zipfile.ZipFile(out_apk, 'w', compression=zipfile.ZIP_DEFLATED) as z:
    for root, dirs, files in os.walk('.'):
        for f in files:
            if f.endswith('.apk') or f.endswith('.sh') or f.startswith('.'):
                continue
            full = os.path.join(root, f)
            arc = os.path.relpath(full, '.')
            z.write(full, arc)
"

zipalign -v -p 4 build_unaligned.apk Yunay_AI_Assistant_Release.apk | tail -n 3
rm -f build_unaligned.apk

if [ -f /sdcard/Download/yunay.jks ]; then
  apksigner sign --ks /sdcard/Download/yunay.jks --ks-pass pass:123456 --key-pass pass:123456 --v1-signing-enabled true --v2-signing-enabled true --v3-signing-enabled true Yunay_AI_Assistant_Release.apk
  echo "=== Successfully built & signed Yunay_AI_Assistant_Release.apk! ==="
fi
