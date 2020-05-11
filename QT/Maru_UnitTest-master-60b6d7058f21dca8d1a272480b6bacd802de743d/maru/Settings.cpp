#include "Settings.h"

quint8 Settings::_orgNo = 0;
quint16 Settings::_meansCode = 0;

QString Settings::_stationName;
QString Settings::_equipmentId;

QTime Settings::_startTime(4, 30);
QTime Settings::_endTime(2, 30);

bool Settings::_dockingMode = false;
quint8 Settings::_operationStatus = 0xFF;

quint32 Settings::_psgCount = 1;

//quint8 Settings::_userMode = Settings::NORMAL_MODE;  // Normal User

bool Settings::_usbMemExist = false;

bool Settings::_printerOpened = false;

bool Settings::_printerNoPaper = false;
