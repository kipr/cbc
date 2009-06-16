CONFIG -= debug release_and_debug
CONFIG += release

QMAKE_DEL_FILE=rm -rf

OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc

QMAKE_DISTCLEAN += build/obj build/moc build/ui build/rcc build
