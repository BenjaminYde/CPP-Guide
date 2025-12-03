#!/bin/bash

EXE_PATH=$1
OUTPUT_DIR=$2
QT_VERSION=$3

# This path matches the 'aqtinstall' location in your Dockerfile
QT_ROOT="/opt/qt/${QT_VERSION}/mingw_64"

if [ -z $EXE_PATH ] || [ -z $OUTPUT_DIR ]; then 
    echo "Usage: create_windows_build_artifact.sh <exe> <out-dir>"
    exit 1
fi

# 1. Create Output Directory
mkdir -p $OUTPUT_DIR

# 2. Copy the Main Executable
echo "Copying Executable..."
cp $EXE_PATH $OUTPUT_DIR

# 3. Copy Essential Qt DLLs
echo "Copying Qt DLLs..."
cp "$QT_ROOT/bin/Qt6Core.dll" $OUTPUT_DIR
cp "$QT_ROOT/bin/Qt6Gui.dll" $OUTPUT_DIR
cp "$QT_ROOT/bin/Qt6Widgets.dll" $OUTPUT_DIR
cp "$QT_ROOT/bin/Qt6Svg.dll" $OUTPUT_DIR

# 4. Copy MinGW Compiler Runtime DLLs
# We search /usr because the exact location varies by distro (e.g. /usr/lib/gcc/...)
echo "Copying Compiler Runtime DLLs..."
find /usr -name "libgcc_s_seh-1.dll" -exec cp {} $OUTPUT_DIR \; -quit 2>/dev/null
find /usr -name "libstdc++-6.dll" -exec cp {} $OUTPUT_DIR \; -quit 2>/dev/null
find /usr -name "libwinpthread-1.dll" -exec cp {} $OUTPUT_DIR \; -quit 2>/dev/null

# 5. Copy Qt Plugins
echo "Copying Plugins..."

# Platforms (REQUIRED for the app to launch)
mkdir -p "$OUTPUT_DIR/platforms" 
cp "$QT_ROOT/plugins/platforms/qwindows.dll" "$OUTPUT_DIR/platforms/"

# Styles (Optional but recommended for native look)
mkdir -p "$OUTPUT_DIR/styles" 
cp "$QT_ROOT/plugins/styles/qwindowsvistastyle.dll" "$OUTPUT_DIR/styles/" 2>/dev/null || true

# Image Formats (Jpeg, Gif, Ico support)
mkdir -p "$OUTPUT_DIR/imageformats" 
cp -r "$QT_ROOT/plugins/imageformats/"* "$OUTPUT_DIR/imageformats/"

echo "Deployment Complete! Check the '$OUTPUT_DIR' folder."