#/bin/env sh

if ./scripts/build.sh
then
    ./scripts/download.sh
else
    echo "❌ Build Failed."
fi