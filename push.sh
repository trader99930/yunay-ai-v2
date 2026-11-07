#!/bin/bash
set -e
echo "=== Pushing to https://github.com/trader99930/yunay-ai-v2 ==="
git add .
git commit -m "feat: Yunay AI Assistant v2 with Antariksh HUD and optimizations" || true
git push origin main
echo "=== Push Complete! ==="
