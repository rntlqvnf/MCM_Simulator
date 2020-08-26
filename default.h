#ifndef DEFAULT_H
#define DEFAULT_H

#include <QString>

#define SLIDER_TIME_RATE 100
#define GRAPH_X_LIMITATION 120
#define WHEEL_ZOOM_RATE 0.1

const QString logDir = "Data/";
const QString firstTabTable = "ALLDATA";
const QString degreeGraphTable = "DEGREE";
const QString rsGraphTable = "RS";
const QString oxGraphTable = "OX";
const QString contGraphTable = "CONT";

enum GRAPH_TYPE {VT, WT};
enum SOUND_TYPE{LOW, HIGH};
enum DATA_TYPE {FINE, COARSE};
enum INSERT_TYPE {FRONT, BACK};
enum TABLE_TYPE {DEGREE, RS, OX, CONT};

#endif // DEFAULT_H
