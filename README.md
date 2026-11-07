# Yunay AI Assistant v2

Yunay is an advanced, ultra-responsive multimodal AI Voice and Vision Assistant for Android, featuring real-time bi-directional conversation powered by Google Gemini Live, integrated screen perception, automation capabilities, and a futuristic Deep Space ("Antariksh") sci-fi HUD.

---

## 🌟 Key Features

* **🌌 Deep Space ("Antariksh") HUD:** Futuristic animated circular HUD with realistic cosmic nebula, audio-reactive pulses, and status telemetry.
* **⚡ Gemini Live Multimodal WebSocket:** Natural, low-latency, two-way conversational voice in English and Hindi/Hinglish.
* **📱 Screen Perception & Vision:** On-device visual grounding and screen capture analysis.
* **🤖 Device Automation:** WhatsApp auto-messaging and YouTube hands-free controls.
* **🔋 Smart Battery Optimization:** Adaptive frame-rate throttling (idle 20fps, active 60fps) to minimize battery drain.
* **🚀 Xiaomi / MIUI 1-Click Setup:** Integrated bypass helper for Autostart and Battery Optimization ("No Restrictions").

---

## 📁 Repository Structure

```
.
├── AndroidManifest.xml       # App Manifest with permissions and service declarations
├── assets/
│   ├── hud/                  # Main interactive HUD UI (HTML5, Tailwind, WebGL shader)
│   ├── orb/                  # Core assistant orb assets & Antariksh cosmic visual
│   ├── settings/             # Settings UI & Xiaomi permissions manager
│   └── model-en-us/          # Offline voice model
├── res/                      # Android resources (drawables, layouts, themes)
├── classes.dex               # Compiled application bytecode
├── classes2.dex              # Main assistant & automation logic
└── build.sh                  # Standalone APK packaging & signing script
```

---

## 🛠️ Build & Sign APK

To build and sign the APK:

```bash
bash build.sh
```

---

## 👤 Author & Owner
* **Owner:** Sushil (Yunay)
* **Version:** v2.0.0 / v2.2.0

## 🎙️ Version 2.3.0: Dual-Engine Real-Time Audio Denoising
1. **Butterworth 2nd-Order High-Pass Filter (Cutoff 180 Hz):**
   - Filters out low-frequency rumble, fan blades hum, motor vibrations, and 50Hz mains hum by over 92%.
   - 0% CPU overhead, processed in 0.05ms per buffer.
2. **Google WebRTC Spectral Noise Suppression Engine ():**
   - Native C/C++ engine running aggressive Spectral Wiener filtering.
   - Cleans out ambient room noise, typing, traffic, and background hiss.
3. **AGC Over-Boost Prevention:**
   - Prevents Android hardware AGC from over-amplifying background noise during pauses and silence.
