/*
 * SM.cpp: Main file for reading Assetto Corsa Competizione shared memory and formatting data for pybind11.
 *         Pybind11 is used to convert c++ code to python module.
 *
 * ACC shared memory docs: https://www.assettocorsa.net/forum/index.php?threads/acc-shared-memory-documentation.59965/
*/

#include "pybind11/pybind11.h"
#include "stdafx.h"
#include "SharedFileOut.h"
#include <windows.h>
#include <tchar.h>
#include <string>
#include <map>
#include <any>
#include <pybind11/numpy.h>
#include <iostream>

#pragma optimize("", off)
using namespace std;
namespace py = pybind11;

template<typename T, unsigned S>
inline unsigned arraysize(const T(&v)[S]) {
    return S;
}

struct SMElement {
    HANDLE hMapFile;
    unsigned char *mapFileBuffer;

};

SMElement m_physics;
SMElement m_graphics;
SMElement m_static;

void initPhysics() {
    /***
    * Function for initializing retrieving in-game physics data from shared memory
    */

    TCHAR szName[] = TEXT("Local\\acpmf_physics");
    m_physics.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFilePhysics),szName);
    if (!m_physics.hMapFile)
    {
        std::cout << "Creating filemap for storing physics data failed" << endl;
    }

    m_physics.mapFileBuffer = (unsigned char *) MapViewOfFile(m_physics.hMapFile, FILE_MAP_READ, 0, 0,sizeof(SPageFilePhysics));
    if (!m_physics.mapFileBuffer)
    {
        std::cout << "Retrieving physics data from shared memory access failed" << endl;
    }
}

void initGraphics() {
    /***
    * Function for initializing retrieving in-game graphics data from shared memory
    */

    TCHAR szName[] = TEXT("Local\\acpmf_graphics");
    m_graphics.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFileGraphic),szName);
    if (!m_graphics.hMapFile)
    {
        std::cout << "Creating filemap for storing graphics data failed" << endl;
    }

    m_graphics.mapFileBuffer = (unsigned char *) MapViewOfFile(m_graphics.hMapFile, FILE_MAP_READ, 0, 0,sizeof(SPageFileGraphic));
    if (!m_graphics.mapFileBuffer)
    {
        std::cout << "Retrieving graphics data from shared memory access failed" << endl;
    }
}

void initStatic() {
    /***
     * Function for initializing retrieving in-game static data from shared memory
     */

    TCHAR szName[] = TEXT("Local\\acpmf_static");
    m_static.hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SPageFileStatic),szName);
    if (!m_static.hMapFile)
    {
        std::cout << "Creating filemap for storing static data failed" << endl;
    }

    m_static.mapFileBuffer = (unsigned char *) MapViewOfFile(m_static.hMapFile, FILE_MAP_READ, 0, 0,sizeof(SPageFileStatic));
    if (!m_static.mapFileBuffer)
    {
        std::cout << "Retrieving static data from shared memory access failed" << endl;
    }
}

py::dict getPhysicsData() {
    /***
     * Function for retrieving physics telemetry data from memory buffer and
     * converting data to python dictionary for pybind
     *
     * return: pybind dictionary (c++ map for python)
     */

    //Fill struct with physics data from buffer
    SPageFilePhysics *pfPhysics = (SPageFilePhysics *) m_physics.mapFileBuffer;

    //Fill python dictionary with telemetry data from physics struct
    py::dict physicsDict;
    physicsDict[py::str("packetID")] = pfPhysics->packetId;
    physicsDict[py::str("gas")] = pfPhysics->gas;
    physicsDict[py::str("brake")] = pfPhysics->brake;
    physicsDict[py::str("camber rad")] = pfPhysics->camberRAD;
    physicsDict[py::str("damage")] = pfPhysics->carDamage;
    physicsDict[py::str("car height")] = pfPhysics->cgHeight;
    physicsDict[py::str("drs")] = pfPhysics->drs;
    physicsDict[py::str("tc")] = pfPhysics->tc;
    physicsDict[py::str("fuel")] = pfPhysics->fuel;
    physicsDict[py::str("gas")] = pfPhysics->gas;
    physicsDict[py::str("gear")] = pfPhysics->gear;
    physicsDict[py::str("number of tyres out")] = pfPhysics->numberOfTyresOut;
    physicsDict[py::str("packet id")] = pfPhysics->packetId;
    physicsDict[py::str("heading")] = pfPhysics->heading;
    physicsDict[py::str("pitch")] = pfPhysics->pitch;
    physicsDict[py::str("roll")] = pfPhysics->roll;
    physicsDict[py::str("rpms")] = pfPhysics->rpms;
    physicsDict[py::str("speed kmh")] = pfPhysics->speedKmh;

    //2d float array to python numpy 2d array
    physicsDict[py::str("contactPoint")] = py::array_t<float>(std::vector<ptrdiff_t>{4, 3},&pfPhysics->tyreContactPoint[0][0]);
    physicsDict[py::str("contactNormal")] = py::array_t<float>(std::vector<ptrdiff_t>{4, 3},&pfPhysics->tyreContactNormal[0][0]);
    physicsDict[py::str("contactHeading")] = py::array_t<float>(std::vector<ptrdiff_t>{4, 3},&pfPhysics->tyreContactHeading[0][0]);
    physicsDict[py::str("brakeBias")] = pfPhysics->brakeBias;
    physicsDict[py::str("localVelocity")] = pfPhysics->localVelocity;
    physicsDict[py::str("slipRatio")] = pfPhysics->slipRatio;
    physicsDict[py::str("slipAngle")] = pfPhysics->slipAngle;
    physicsDict[py::str("steer")] = pfPhysics->steerAngle;
    physicsDict[py::str("suspensionTravel")] = pfPhysics->suspensionTravel;
    physicsDict[py::str("tyreCoreTemp")] = pfPhysics->tyreCoreTemperature;
    physicsDict[py::str("tyreDirtyLevel")] = pfPhysics->tyreDirtyLevel;
    physicsDict[py::str("tyreWear")] = pfPhysics->tyreWear;
    physicsDict[py::str("velocity")] = pfPhysics->velocity;
    physicsDict[py::str("accG")] = pfPhysics->accG;
    physicsDict[py::str("wheelAngularSpeed")] = pfPhysics->wheelAngularSpeed;
    physicsDict[py::str("wheelLoad")] = pfPhysics->wheelLoad;
    physicsDict[py::str("wheelSlip")] = pfPhysics->wheelSlip;
    physicsDict[py::str("wheelPressure")] = pfPhysics->wheelsPressure;
    physicsDict[py::str("waterTemp")] = pfPhysics->waterTemp;
    physicsDict[py::str("brakePressure")] = pfPhysics->brakePressure;
    physicsDict[py::str("frontBrakeCompound")] = pfPhysics->frontBrakeCompound;
    physicsDict[py::str("rearBrakeCompound")] = pfPhysics->rearBrakeCompound;
    physicsDict[py::str("padLife")] = pfPhysics->padLife;
    physicsDict[py::str("discLife")] = pfPhysics->discLife;
    physicsDict[py::str("ignitionOn")] = pfPhysics->ignitionOn;
    physicsDict[py::str("starterEngineOn")] = pfPhysics->starterEngineOn;
    physicsDict[py::str("isEngineRunning")] = pfPhysics->isEngineRunning;
    physicsDict[py::str("kerbVibration")] = pfPhysics->kerbVibration;
    physicsDict[py::str("slipVibrations")] = pfPhysics->slipVibrations;
    physicsDict[py::str("gVibrations")] = pfPhysics->gVibrations;
    physicsDict[py::str("absVibrations")] = pfPhysics->absVibrations;
    physicsDict[py::str("isAIControlled")] = pfPhysics->isAIControlled;
    physicsDict[py::str("brakeTemp")] = pfPhysics->brakeTemp;
    physicsDict[py::str("clutch")] = pfPhysics->clutch;
    physicsDict[py::str("pitLimiterOn")] = pfPhysics->pitLimiterOn;
    physicsDict[py::str("abs")] = pfPhysics->abs;
    physicsDict[py::str("autoShifterOn")] = pfPhysics->autoShifterOn;
    physicsDict[py::str("turboBoost")] = pfPhysics->turboBoost;
    physicsDict[py::str("airTemp")] = pfPhysics->airTemp;
    physicsDict[py::str("roadTemp")] = pfPhysics->roadTemp;
    physicsDict[py::str("localAngularVel")] = pfPhysics->localAngularVel;
    physicsDict[py::str("finalFF")] = pfPhysics->finalFF;

    return physicsDict;
}

py::dict getGraphicsData() {
    /***
    * Function for retrieving graphics telemetry data from memory buffer and
    * converting data to python dictionary for pybind
    *
    * return: pybind dictionary (c++ map for python)
    */

    //Fill struct with graphics data from buffer
    SPageFileGraphic *pfGraphics = (SPageFileGraphic *) m_graphics.mapFileBuffer;

    //Fill python dictionairy with telemetry data from physics struct
    py::dict graphicsDict;
    graphicsDict[py::str("packetID")] = pfGraphics->packetId;
    graphicsDict[py::str("STATUS ")] = pfGraphics->status;
    graphicsDict[py::str("session")] = pfGraphics->session;
    graphicsDict[py::str("completed laps")] = pfGraphics->completedLaps;
    graphicsDict[py::str("position")] = pfGraphics->position;
    graphicsDict[py::str("currentTime")] = pfGraphics->currentTime;
    graphicsDict[py::str("iCurrentTime")] = pfGraphics->iCurrentTime;
    graphicsDict[py::str("lastTime")] = pfGraphics->lastTime;
    graphicsDict[py::str("iLastTime")] = pfGraphics->iLastTime;
    graphicsDict[py::str("bestTime")] = pfGraphics->bestTime;
    graphicsDict[py::str("iBestTime")] = pfGraphics->iBestTime;
    graphicsDict[py::str("split")] = pfGraphics->split;
    graphicsDict[py::str("sessionTimeLeft")] = pfGraphics->sessionTimeLeft;
    graphicsDict[py::str("distanceTraveled")] = pfGraphics->distanceTraveled;
    graphicsDict[py::str("isInPit")] = pfGraphics->isInPit;
    graphicsDict[py::str("currentSectorIndex")] = pfGraphics->currentSectorIndex;
    graphicsDict[py::str("lastSectorTime")] = pfGraphics->lastSectorTime;
    graphicsDict[py::str("numberOfLaps")] = pfGraphics->numberOfLaps;
    graphicsDict[py::str("tyreCompound")] = pfGraphics->tyreCompound;
    graphicsDict[py::str("normalizedCarPosition")] = pfGraphics->normalizedCarPosition;
    graphicsDict[py::str("carCoordinates")] = py::array_t<float>(std::vector<ptrdiff_t>{60, 3},&pfGraphics->carCoordinates[0][0]);
    graphicsDict[py::str("activeCars")] = pfGraphics->activeCars;
    graphicsDict[py::str("isInPitLane")] = pfGraphics->isInPitLane;
    graphicsDict[py::str("penaltyTime")] = pfGraphics->penaltyTime;
    graphicsDict[py::str("idealLineOn")] = pfGraphics->idealLineOn;
    graphicsDict[py::str("carID")] = pfGraphics->carID;
    graphicsDict[py::str("playerCarID")] = pfGraphics->playerCarID;
    graphicsDict[py::str("surfaceGrip")] = pfGraphics->surfaceGrip;
    graphicsDict[py::str("mandatoryPitDone")] = pfGraphics->mandatoryPitDone;
    graphicsDict[py::str("windSpeed")] = pfGraphics->windSpeed;
    graphicsDict[py::str("windDirection")] = pfGraphics->windDirection;
    graphicsDict[py::str("isSetupMenuVisible")] = pfGraphics->isSetupMenuVisible;
    graphicsDict[py::str("mainDisplayIndex")] = pfGraphics->mainDisplayIndex;
    graphicsDict[py::str("secondaryDisplayIndex")] = pfGraphics->secondaryDisplayIndex;
    graphicsDict[py::str("TC")] = pfGraphics->TC;
    graphicsDict[py::str("TCCut")] = pfGraphics->TCCut;
    graphicsDict[py::str("EngineMap")] = pfGraphics->EngineMap;
    graphicsDict[py::str("ABS")] = pfGraphics->ABS;
    graphicsDict[py::str("fuelXLap")] = pfGraphics->fuelXLap;
    graphicsDict[py::str("rainLights ")] = pfGraphics->rainLights ;
    graphicsDict[py::str("flashingLights")] = pfGraphics->flashingLights;
    graphicsDict[py::str("lightsStage")] = pfGraphics->lightsStage;
    graphicsDict[py::str("exhaustTemperature")] = pfGraphics->exhaustTemperature;
    graphicsDict[py::str("wiperLV ")] = pfGraphics->wiperLV ;
    graphicsDict[py::str("DriverStintTotalTimeLeft")] = pfGraphics->DriverStintTotalTimeLeft;
    graphicsDict[py::str("DriverStintTimeLeft")] = pfGraphics->DriverStintTimeLeft;
    graphicsDict[py::str("rainTyres")] = pfGraphics->rainTyres;
    graphicsDict[py::str("sessionIndex")] = pfGraphics->sessionIndex;
    graphicsDict[py::str("usedFuel")] = pfGraphics->usedFuel;
    graphicsDict[py::str("deltaLapTime")] = pfGraphics->deltaLapTime;
    graphicsDict[py::str("iDeltaLapTime")] = pfGraphics->iDeltaLapTime;
    graphicsDict[py::str("estimatedLapTime")] = pfGraphics->estimatedLapTime;
    graphicsDict[py::str("iEstimatedLapTime")] = pfGraphics->iEstimatedLapTime;
    graphicsDict[py::str("isDeltaPositive")] = pfGraphics->isDeltaPositive;
    graphicsDict[py::str("iSplit")] = pfGraphics->iSplit;
    graphicsDict[py::str("isValidLap")] = pfGraphics->isValidLap;
    graphicsDict[py::str("fuelEstimatedLaps")] = pfGraphics->fuelEstimatedLaps;
    graphicsDict[py::str("trackStatus")] = pfGraphics->trackStatus;
    graphicsDict[py::str("missingMandatoryPits")] = pfGraphics->missingMandatoryPits;
    graphicsDict[py::str("Clock")] = pfGraphics->Clock;
    graphicsDict[py::str("directionLightsLeft")] = pfGraphics->directionLightsLeft;
    graphicsDict[py::str("directionLightsRight")] = pfGraphics->directionLightsRight;
    graphicsDict[py::str("GlobalYellow")] = pfGraphics->GlobalYellow;
    graphicsDict[py::str("GlobalYellow1")] = pfGraphics->GlobalYellow1;
    graphicsDict[py::str("GlobalYellow2")] = pfGraphics->GlobalYellow2;
    graphicsDict[py::str("GlobalYellow3")] = pfGraphics->GlobalYellow3;
    graphicsDict[py::str("GlobalWhite")] = pfGraphics->GlobalWhite;
    graphicsDict[py::str("GlobalGreen")] = pfGraphics->GlobalGreen;
    graphicsDict[py::str("GlobalChequered")] = pfGraphics->GlobalChequered;
    graphicsDict[py::str("GlobalRed")] = pfGraphics->GlobalRed;
    graphicsDict[py::str("mfdTyreSet")] = pfGraphics->mfdTyreSet;
    graphicsDict[py::str("mfdFuelToAdd")] = pfGraphics->mfdFuelToAdd;
    graphicsDict[py::str("mfdTyrePressureLF")] = pfGraphics->mfdTyrePressureLF;
    graphicsDict[py::str("mfdTyrePressureRF")] = pfGraphics->mfdTyrePressureRF;
    graphicsDict[py::str("mfdTyrePressureLR")] = pfGraphics->mfdTyrePressureLR;
    graphicsDict[py::str("mfdTyrePressureRR")] = pfGraphics->mfdTyrePressureRR;
    graphicsDict[py::str("currentTyreSet")] = pfGraphics->currentTyreSet;
    graphicsDict[py::str("strategyTyreSet")] = pfGraphics->strategyTyreSet;
    graphicsDict[py::str("gapAhead")] = pfGraphics->gapAhead;
    graphicsDict[py::str("gapBehind")] = pfGraphics->gapBehind;

    // currently not working
    graphicsDict[py::str("flag")] = pfGraphics->flag; //maybe change directly to string penalty
    graphicsDict[py::str("penalty")] = pfGraphics->penalty;

    return graphicsDict;
}

py::dict getStaticData() {
    /***
    * Function for retrieving graphics static data from memory buffer and
    * converting data to python dictionary for pybind
    *
    * return: pybind dictionary (c++ map for python)
    */

    //Fill struct with graphics data from buffer
    SPageFileStatic *pfStatic = (SPageFileStatic *) m_static.mapFileBuffer;

    //Fill python dictionary with telemetry data from physics struct
    py::dict staticDict;
    staticDict[py::str("smVersion")] = pfStatic->smVersion;
    staticDict[py::str("smVersion")] = pfStatic->acVersion;
    staticDict[py::str("numberOfSessions")] = pfStatic->numberOfSessions;
    staticDict[py::str("numCars")] = pfStatic->numCars;
    staticDict[py::str("carModel")] = pfStatic->carModel;
    staticDict[py::str("track")] = pfStatic->track;
    staticDict[py::str("playerName")] = pfStatic->playerName;
    staticDict[py::str("playerSurname")] = pfStatic->playerSurname;
    staticDict[py::str("playerNick")] = pfStatic->playerNick;
    staticDict[py::str("sectorCount")] = pfStatic->sectorCount;
    staticDict[py::str("maxRpm")] = pfStatic->maxRpm;
    staticDict[py::str("maxFuel")] = pfStatic->maxFuel;
    staticDict[py::str("penaltiesEnabled")] = pfStatic->penaltiesEnabled;
    staticDict[py::str("aidFuelRate")] = pfStatic->aidFuelRate;
    staticDict[py::str("aidTireRate")] = pfStatic->aidTireRate;
    staticDict[py::str("aidMechanicalDamage")] = pfStatic->aidMechanicalDamage;
    staticDict[py::str("aidAllowTyreBlankets")] = pfStatic->aidAllowTyreBlankets;
    staticDict[py::str("aidStability")] = pfStatic->aidStability;
    staticDict[py::str("aidAutoClutch")] = pfStatic->aidAutoClutch;
    staticDict[py::str("aidAutoBlip")] = pfStatic->aidAutoBlip; // always true
    staticDict[py::str("PitWindowStart")] = pfStatic->PitWindowStart;
    staticDict[py::str("PitWindowEnd")] = pfStatic->PitWindowEnd;
    staticDict[py::str("isOnline")] = pfStatic->isOnline;
    staticDict[py::str("dryTyresName")] = pfStatic->dryTyresName;
    staticDict[py::str("wetTyresName")] = pfStatic->wetTyresName;

    return staticDict;
}

PYBIND11_MAKE_OPAQUE(std::map<std::string, std::any>);
PYBIND11_MODULE(ACCSharedMemory, m) {
    m.doc() = "C++ ACCSharedMemory telemetry module";

    m.def("initPhysics", &initPhysics, "Function for initializing physics telemetry");
    m.def("initGraphics", &initGraphics, "Function for initializing Graphics telemetry");
    m.def("initStatic", &initStatic, "Function for initializing Static telemetry");

    m.def("getPhysicsData", &getPhysicsData, "Function for retrieving physics telemetry data");
    m.def("getGraphicsData", &getGraphicsData, "Function for retrieving Graphics telemetry data");
    m.def("getStaticData", &getStaticData, "Function for retrieving Static telemetry data");


}
/* ---- local testing ---- */
/*
void dismiss(SMElement element) {
    UnmapViewOfFile(element.mapFileBuffer);
    CloseHandle(element.hMapFile);
}

void printData(string name, float value) {
    wcout << name.c_str() << " : " << value << endl;
}

template<typename T, unsigned S>
inline void printData(const string name, const T(&v)[S]) {
    wcout << name.c_str() << " : ";

    for (int i = 0; i < S; i++) {
        wcout << v[i];
        if (i < S - 1) {
            wcout << " , ";
        }

    }
    wcout << endl;
}

template<typename T, unsigned S, unsigned S2>
inline void printData2(const string name, const T(&v)[S][S2]) {
    wcout << name.c_str() << " : ";

    for (int i = 0; i < S; i++) {
        wcout << i << " : ";
        for (int j = 0; j < S2; j++) {
            wcout << v[i][j];
            if (j < S2 - 1) {
                wcout << " , ";
            }
        }

        wcout << ";" << endl;

    }

}

int _tmain(int argc, _TCHAR* argv[])
{
    initPhysics();
    initGraphics();
    initStatic();

    //SPageFilePhysics* pf1 = getPhysicsData();
    //SPageFilePhysics* pf2 = getGraphicsData();
    //SPageFilePhysics* pf3 = getStaticData();


    //send it through udp or just directly import this as module
    while (FALSE)
    {

        wcout << "---------------PHYSICS INFO---------------" << endl;
        SPageFilePhysics* pf1 = (SPageFilePhysics*)m_physics.mapFileBuffer;
        printData("acc G", pf1->accG);
        printData("brake", pf1->brake);
        printData("camber rad", pf1->camberRAD);
        printData("damage", pf1->carDamage);
        printData("car height", pf1->cgHeight);
        printData("drs", pf1->drs);
        printData("tc", pf1->tc);
        printData("fuel", pf1->fuel);
        printData("gas", pf1->gas);
        printData("gear", pf1->gear);
        printData("number of tyres out", pf1->numberOfTyresOut);
        printData("packet id", pf1->packetId);
        printData("heading", pf1->heading);
        printData("pitch", pf1->pitch);
        printData("roll", pf1->roll);
        printData("rpms", pf1->rpms);
        printData("speed kmh", pf1->speedKmh);
        printData2("contact point", pf1->tyreContactPoint);
        printData2("contact normal", pf1->tyreContactNormal);
        printData2("contact heading", pf1->tyreContactHeading);
        printData("steer ", pf1->steerAngle);
        printData("suspension travel", pf1->suspensionTravel);
        printData("tyre core temp", pf1->tyreCoreTemperature);
        printData("tyre dirty level", pf1->tyreDirtyLevel);
        printData("tyre wear", pf1->tyreWear);
        printData("velocity", pf1->velocity);
        printData("wheel angular speed", pf1->wheelAngularSpeed);
        printData("wheel load", pf1->wheelLoad);
        printData("wheel slip", pf1->wheelSlip);
        printData("wheel pressure", pf1->wheelsPressure);



        wcout << "---------------GRAPHICS INFO---------------" << endl;
        SPageFileGraphic* pf2 = (SPageFileGraphic*)m_graphics.mapFileBuffer;
        printData("packetID ", pf2->packetId);
        printData("STATUS ", pf2->status);
        printData("session", pf2->session);
        printData("completed laps", pf2->completedLaps);
        printData("position", pf2->position);
        printData("current time s", pf2->currentTime);
        printData("current time", pf2->iCurrentTime);
        printData("last time s", pf2->lastTime);
        printData("last time ", pf2->iLastTime);
        printData("best time s", pf2->bestTime);
        printData("best time", pf2->iBestTime);
        printData("sessionTimeLeft", pf2->sessionTimeLeft);
        printData("distanceTraveled", pf2->distanceTraveled);
        printData("isInPit", pf2->isInPit);
        printData("currentSectorIndex", pf2->currentSectorIndex);
        printData("lastSectorTime", pf2->lastSectorTime);
        printData("numberOfLaps", pf2->numberOfLaps);
        wcout << "TYRE COMPOUND : " << pf2->tyreCompound << endl;
        printData("replayMult", pf2->replayTimeMultiplier);
        printData("normalizedCarPosition", pf2->normalizedCarPosition);
        printData2("carCoordinates", pf2->carCoordinates);


        //Just grab this every start of session
        wcout << "---------------STATIC INFO---------------" << endl;
        SPageFileStatic* pf3 = (SPageFileStatic*)m_static.mapFileBuffer;
        wcout << "SM VERSION " << pf3->smVersion << endl;
        wcout << "AC VERSION " << pf3->acVersion << endl;

        printData("number of sessions ", pf3->numberOfSessions);
        printData("numCars", pf3->numCars);
        wcout << "Car model " << pf3->carModel << endl;
        wcout << "Car track " << pf3->track << endl;
        wcout << "Player Name " << pf3->playerName << endl;
        printData("sectorCount", pf3->sectorCount);

        printData("maxTorque", pf3->maxTorque);
        printData("maxPower", pf3->maxPower);
        printData("maxRpm", pf3->maxRpm);
        printData("maxFuel", pf3->maxFuel);
        printData("suspensionMaxTravel", pf3->suspensionMaxTravel);
        printData("tyreRadius", pf3->tyreRadius);


        Sleep(3000);

    }

    dismiss(m_graphics);
    dismiss(m_physics);
    dismiss(m_static);

    return 0;
}*/


