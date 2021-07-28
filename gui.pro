TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = qt3dwidget cfdqt

cfdqt.depends = qt3dwidget
