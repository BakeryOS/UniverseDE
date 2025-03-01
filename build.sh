#!/bin/bash

# -------------------------- Before build ----------------------- #

cd "$(dirname "$0")" || exit
# set -e
source scripts/utils.sh

chmod +x scripts/deepinstall.sh

# ---------------------- Default Variables ---------------------- #

CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:-"-Wall -Wextra -std=c++17 -O2 -fPIC"}"

SuccessExit=0
CompilationErrorExit=1
LinkingErrorExit=2
ErrorExit=3

SRC="src"
BUILD="build"

OUT="Universe.elf"
OUTDIR="bin"

# ---------------------- Configuration Variables ---------------------- #

QtIncludePath="${QtIncludePath:-$(find /usr/include /usr/local/include -type d -name "qt5" 2>/dev/null | head -n 1)}"
KF5IncludePath="${KF5IncludePath:-$(find /usr/include /usr/local/include -type d -name "KF6" 2>/dev/null | head -n 1)}"

QtLibPath="${QtLibPath:-$(find /usr/lib* /usr/local/lib* -type f -name "libQt5Core.so*" 2>/dev/null | xargs -r dirname | head -n 1)}"
[ -z "$QtLibPath" ] && QtLibPath="$(pkg-config --variable=libdir Qt5 2>/dev/null)"
[ -z "$QtLibPath" ] && QtLibPath="/usr/lib64/qt5"

KF5LibPath="${KF5LibPath:-$(find /usr/lib* /usr/local/lib* -type f -name "libkconfig.so*" 2>/dev/null | xargs -r dirname | head -n 1)}"
[ -z "$KF5LibPath" ] && KF5LibPath="$(pkg-config --variable=libdir KF6Config 2>/dev/null)"
[ -z "$KF5LibPath" ] && KF5LibPath="/usr/lib64/KConfig"

# ---------------------- Parsing Arguments ---------------------- #

for arg in "$@"; do
	case $arg in
	--CXX=*)
		CXX="${arg#*=}"
		;;
	--CXXFLAGS=*)
		CXXFLAGS="${arg#*=}"
		;;
	--QtIncludePath=*)
		QtIncludePath="${arg#*=}"
		;;
	--KF5IncludePath=*)
		KF5IncludePath="${arg#*=}"
		;;
	--QtLibPath=*)
		QtLibPath="${arg#*=}"
		;;
	--KF5LibPath=*)
		KF5LibPath="${arg#*=}"
		;;
	--OUT-FILE=*)
		OUT="${arg#*=}"
		;;
	--OUT-DIR=*)
		OUTDIR="${arg#*=}"
		;;
	--BUILD-DIR=*)
		BUILD="${arg#*=}"
		;;
	clean | --clean)
		[[ ! (-z "$BUILD" || -z "$OUTDIR") ]] && rm -rf "$BUILD" "$OUTDIR"
		exit $SuccessExit
		;;
	*)
		ShowError $ErrorExit "Unknown argument: $arg"
		;;
	esac
done

scripts/deepinstall.sh || ShowWarn "Failed to install dependencies"

# ---------------------- Libs and includepath ---------------------- #

INCLUDEPATH=(
	"/usr/include"
	"$QtIncludePath"
	"$QtIncludePath/QtCore"
	"$QtIncludePath/QtWidgets"
	"$QtIncludePath/QtGui"
	"$QtIncludePath/QtX11Extras"
	"$KF5IncludePath/KConfig"
	"$KF5IncludePath/KConfigCore"
	"$PWD/src/Include"
	"$PWD/src"
)

LIBS=(
	"-L$QtLibPath"
	"-L$KF5LibPath"
	"-lQt5Core"
	"-lQt5Widgets"
	"-lQt5Gui"
	"-lQt5X11Extras"
	"-lQt5Concurrent"
	"-lQt5Network"
	"-lQt5Xml"
	"-lQt5DBus"
	"-lQt5PrintSupport"
	"-lKF5ConfigCore"
	"-lKF5ConfigGui"
	"-lKF5CoreAddons"
	"-lX11"
)

# ---------------------- Building Project ---------------------- #

mkdir -p "$BUILD" || ShowError $ErrorExit "Failed to create build directory"
mkdir -p "$OUTDIR" || ShowError $ErrorExit "Failed to create output directory"

SOURCES=($(find "$SRC" -type f -name "*.cpp"))
OBJECTS=()
total=0
compiled=0

MOCFILES=($(find "$SRC" -type f -name "*.hpp" -exec grep -l "Q_OBJECT" {} \;))

ShowInfo "Building $OUT..."

# ------------------- QMetaObject Compiler ------------------- #
totalMoc=${#MOCFILES[@]}
compiledMoc=0
mocFilesToCompile=()

for header in "${MOCFILES[@]}"; do
	mocFile="$BUILD/moc_$(basename "${header%.hpp}.cpp")"
  SOURCES+=("$mocFile")
	if [[ ! -f "$mocFile" || "$(stat -c %Y "$header")" -gt "$(stat -c %Y "$mocFile")" ]]; then
		mocFilesToCompile+=("$header")
	fi
done

if [[ ${#mocFilesToCompile[@]} -gt 0 ]]; then
	ShowInfo "Running qmeta object compiler..."
	for header in "${mocFilesToCompile[@]}"; do
		mocFile="$BUILD/moc_$(basename "${header%.hpp}.cpp")"
		moc "$header" -o "$mocFile" || ShowError $CompilationErrorExit "MOC failed for $header"
		((compiledMoc++))
		printf "\033[1;33m[%3d%% ]\033[93m %s\033[0m -> \033[93;1m%s\n" "$((compiledMoc * 100 / totalMoc))" "$header" "$mocFile"
	done
fi

# ----------------- Attachment Resources (qrc) ----------------- #
QRCFILES=("assets/QSS/qss.qrc" "assets/Images/images.qrc")
totalRcc=${#QRCFILES[@]}
compiledRcc=0
rccFilesToCompile=()

for qrc in "${QRCFILES[@]}"; do
	baseName=$(basename "${qrc%.qrc}")
	rccFile="$BUILD/qrc_${baseName}.cpp"
  obj="$BUILD/$(basename "${rccFile%.cpp}.o")"
  OBJECTS+=("$obj")
	if [[ ! -f "$rccFile" || "$(stat -c %Y "$qrc")" -gt "$(stat -c %Y "$rccFile")" ]]; then
		rccFilesToCompile+=("$qrc")
	fi
done

if [[ ${#rccFilesToCompile[@]} -gt 0 ]]; then
	ShowInfo "Attachment of resources..."
	for qrc in "${rccFilesToCompile[@]}"; do
		baseName=$(basename "${qrc%.qrc}")
		rccFile="$BUILD/qrc_${baseName}.cpp"
		rcc -name "$baseName" "$qrc" -o "$rccFile" || ShowError $CompilationErrorExit "RCC failed for $qrc"
		((compiledRcc++))
		printf "\033[1;33m[%3d%% ]\033[93m %s \033[0m-> \033[93;1m%s" "$((compiledRcc * 100 / totalRcc))" "$qrc" "$rccFile"
    obj="$BUILD/$(basename "${rccFile%.cpp}.o")"
    $CXX $CXXFLAGS "${INCLUDEPATH[@]/#/-I}" -c "$rccFile" -o "$obj" || ShowError $CompilationErrorExit "Compilation failed for $rccFile"
    echo -e "\033[0m -> \033[93;1m$obj"
	done
fi

# ---------------------- Compiling Sources ---------------------- #
for src in "${SOURCES[@]}"; do
	obj="$BUILD/$(basename "${src%.cpp}.o")"
	if [[ ! -f "$obj" || "$(stat -c %Y "$src")" -gt "$(stat -c %Y "$obj")" ]]; then

		((total++))
	fi
	OBJECTS+=("$obj")
done

ShowInfo "Compiling sources..."
for src in "${SOURCES[@]}"; do
	obj="$BUILD/$(basename "${src%.cpp}.o")"

	if [[ ! -f "$obj" || "$(stat -c %Y "$src")" -gt "$(stat -c %Y "$obj")" ]]; then
		$CXX $CXXFLAGS "${INCLUDEPATH[@]/#/-I}" -c "$src" -o "$obj" || ShowError $CompilationErrorExit "Compilation failed for $src"
		((compiled++))
		printf "\033[1;33m[%3d%% ]\033[93m %s \033[0m-> \033[93;1m%s\n" "$((compiled * 100 / total))" "$src" "$obj"
	fi
done

# ------------------------ Linking Objects ------------------------ #
ShowInfo "Linking objects..."

$CXX $CXXFLAGS "${OBJECTS[@]}" "${LIBS[@]}" -o "$OUTDIR/$OUT" || ShowError $LinkingErrorExit "Linking failed."

# ------------------------ Finishing Build ------------------------ #
chmod +x $OUTDIR/$OUT

ShowSuccess "Build completed: $OUT"
exit $SuccessExit
