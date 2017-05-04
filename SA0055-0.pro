#-------------------------------------------------
#
# Project created by QtCreator 2016-12-06T09:38:57
#
#-------------------------------------------------

QT += core gui widgets sql

TARGET = SA0055-0
TEMPLATE = app
INCLUDEPATH += table-manager column-delegate other-delegate form validator tool common-header \
                table-model expression log ext-widget csv proxy-model tree-model

SOURCES += main.cpp\
    validator/validatormanager.cpp \
    \
    tool/dbtools.cpp \
    \
    form/aboutdialog.cpp \
    form/datadumpform.cpp \
    form/datatraceform.cpp \
    form/deviceparaform.cpp \
    form/deviceparaform-infotable.cpp \
    form/deviceparaform-iec104para.cpp \
    form/deviceparaform-devicepara.cpp \
    form/formulaform.cpp \
    form/globalparaform.cpp \
    form/integralformulaform.cpp \
    form/mainwindow.cpp \
    form/mainwindow-actions.cpp \
    form/newdevicedialog.cpp \
    form/newprojectdialog.cpp \
    form/plancurveform.cpp \
    form/selectpointdialog.cpp \
    form/serverparaform.cpp \
    \
    table-manager/aidumptablemanager.cpp \
    table-manager/aitracetablemanager.cpp \
    table-manager/deviceparatablemanager.cpp \
    table-manager/enumtablemanager.cpp \
    table-manager/enumtablemanager-objects.cpp \
    table-manager/iec104infotablemanager.cpp \
    table-manager/iec104paratablemanager.cpp \
    table-manager/maptablemanager.cpp \
    table-manager/maptablemanager-objects.cpp \
    table-manager/plancurvetablemanager.cpp \
    table-manager/serverparatablemanager.cpp \
    \
    column-delegate/descriptioncolumndelegate.cpp \
    column-delegate/doublecolumndelegate.cpp \
    column-delegate/foreignkeycolumndelegate.cpp \
    column-delegate/integercolumndelegate.cpp \
    \
    other-delegate/globalparadelegate.cpp \
    table-manager/integraldefinitiontablemanager.cpp \
    form/newformuladialog.cpp \
    form/formulaform-calcvar.cpp \
    table-manager/calcvartablemanager.cpp \
    table-manager/calcvarbindtablemanager.cpp \
    column-delegate/selectpointcolumndelegate.cpp \
    form/formulaform-combobox.cpp \
    column-delegate/selectdevicecolumndelegate.cpp \
    tool/util.cpp \
    form/calcvarform.cpp \
    table-model/calcvarbindtablemodel.cpp \
    table-model/tempcalcvartablemodel.cpp \
    form/formulaform-expr.cpp \
    expression/expression.cpp \
    expression/operand.cpp \
    expression/operator.cpp \
    table-manager/alarmruletablemanager.cpp \
    table-manager/alarminfotablemanager.cpp \
    log/logutil.cpp \
    form/newalarmruledialog.cpp \
    ext-widget/exlistwidget.cpp \
    column-delegate/selectcolorcolumndelegate.cpp \
    column-delegate/selectfilepathcolumndelegate.cpp \
    table-manager/soundfiletablemanager.cpp \
    table-manager/picturefiletablemanager.cpp \
    other-delegate/comboboxmapperdelegate.cpp \
    column-delegate/comboboxitemdelegate.cpp \
    csv/infotableimportexport.cpp \
    form/deviceparaform-import.cpp \
    table-manager/abstracttablemanager.cpp \
    form/selectdevicedialog.cpp \
    table-manager/alarmcausetablemanager.cpp \
    proxy-model/selectpointfilterproxymodel.cpp \
    form/alarmform-batchsetvalue.cpp \
    form/alarmform.cpp \
    form/mainwindow-tree.cpp \
    tree-model/treenode.cpp \
    tree-model/selectpointtreenode.cpp \
    tree-model/selectpointtreenode-init.cpp \
    tree-model/treemodel.cpp \
    tree-model/projecttreenode.cpp \
    tree-model/projecttreenode-init.cpp \
    tree-model/projecttreenode-modify.cpp \
    table-manager/formuladeftablemanager.cpp \
    form/selectpoint.cpp

HEADERS  += common-header/datatype.h \
    \
    validator/validatormanager.h \
    tool/dbtools.h \
    \
    form/aboutdialog.h \
    form/datadumpform.h \
    form/datatraceform.h \
    form/deviceparaform.h \
    form/eventalarmform.h \
    form/formulaform.h \
    form/globalparaform.h \
    form/integralformulaform.h \
    form/mainwindow.h \
    form/newdevicedialog.h \
    form/newprojectdialog.h \
    form/plancurveform.h \
    form/selectpoint.h \
    form/selectpointdialog.h \
    form/serverparaform.h \
    \
    table-manager/abstractmaptablemanager.h \
    table-manager/aidumptablemanager.h \
    table-manager/aitracetablemanager.h \
    table-manager/deviceparatablemanager.h \
    table-manager/enumtablemanager.h \
    table-manager/iec104infotablemanager.h \
    table-manager/iec104paratablemanager.h \
    table-manager/maptablemanager.h \
    table-manager/plancurvetablemanager.h \
    table-manager/serverparatablemanager.h \
    \
    column-delegate/descriptioncolumndelegate.h \
    column-delegate/doublecolumndelegate.h \
    column-delegate/foreignkeycolumndelegate.h \
    column-delegate/integercolumndelegate.h \
    \
    other-delegate/globalparadelegate.h \
    common-header/constvalue.h \
    table-manager/integraldefinitiontablemanager.h \
    form/newformuladialog.h \
    table-manager/calcvartablemanager.h \
    table-manager/calcvarbindtablemanager.h \
    column-delegate/selectpointcolumndelegate.h \
    column-delegate/selectdevicecolumndelegate.h \
    tool/util.h \
    form/calcvarform.h \
    table-model/calcvarbindtablemodel.h \
    table-model/tempcalcvartablemodel.h \
    table-model/calcvarbindpoint.h \
    table-manager/formuladeftablemanager.h \
    expression/expression.h \
    expression/operand.h \
    expression/operator.h \
    table-manager/alarmruletablemanager.h \
    table-manager/alarminfotablemanager.h \
    log/logutil.h \
    form/newalarmruledialog.h \
    ext-widget/exlistwidget.h \
    column-delegate/selectcolorcolumndelegate.h \
    column-delegate/selectfilepathcolumndelegate.h \
    table-manager/soundfiletablemanager.h \
    table-manager/picturefiletablemanager.h \
    other-delegate/comboboxmapperdelegate.h \
    column-delegate/comboboxitemdelegate.h \
    csv/infotableimportexport.h \
    table-manager/abstracttablemanager.h \
    form/selectdevicedialog.h \
    table-manager/alarmcausetablemanager.h \
    proxy-model/selectpointfilterproxymodel.h \
    tree-model/treenode.h \
    tree-model/selectpointtreenode.h \
    tree-model/treemodel.h \
    tree-model/projecttreenode.h

FORMS += ui/mainwindow.ui \
    ui/formulaform.ui \
    ui/selectpointdialog.ui \
    ui/globalparaform.ui \
    ui/serverparaform.ui \
    ui/newprojectdialog.ui \
    ui/deviceparaform.ui \
    ui/plancurveform.ui \
    ui/aboutdialog.ui \
    ui/datadumpform.ui \
    ui/datatraceform.ui \
    ui/integralformulaform.ui \
    ui/newdevicedialog.ui \
    ui/newformuladialog.ui \
    ui/calcvarform.ui \
    ui/newalarmruledialog.ui \
    ui/selectdevicedialog.ui \
    ui/alarmform.ui

RESOURCES += \
    resource/resource.qrc
