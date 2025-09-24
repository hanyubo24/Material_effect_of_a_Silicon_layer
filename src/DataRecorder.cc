#include "DataRecorder.hh"
#include <iostream>

DataRecorder& DataRecorder::Instance() {
    static DataRecorder inst; return inst;
}

void DataRecorder::Open(const std::string& filename) {
    std::scoped_lock<std::mutex> lock(fMutex);
    if (fOut.is_open()) fOut.close();
    fOut.open(filename, std::ios::out | std::ios::trunc);
    if (!fOut) {
    std::cerr << "[DataRecorder] Failed to open file: " << filename << std::endl;
    return;
    }
    fHeaderWritten = false;
}

void DataRecorder::Close() {
    std::scoped_lock<std::mutex> lock(fMutex);
    if (fOut.is_open()) fOut.close();
}

void DataRecorder::WriteRow(int eventID, int trackID, int pdg, double pxIn, double pyIn, double pzIn, double pIn, double tIn, double pxOut, double pyOut, double pzOut, double pOut, double tOut) {
    std::scoped_lock<std::mutex> lock(fMutex);
    if (!fOut.is_open()) return;
    if (!fHeaderWritten) {
        fOut << "event,track,pdg,"
        "pxIn(MeV/c),pyIn(MeV/c),pzIn(MeV/c),pIn(MeV/c),tIn(ns),"
        "pxOut(MeV/c),pyOut(MeV/c),pzOut(MeV/c),pOut(MeV/c),tOut(ns)\n";
        fHeaderWritten = true;
    }
    fOut << eventID << ',' << trackID << ',' << pdg << ','
    << pxIn << ',' << pyIn << ',' << pzIn << ',' << pIn << ',' << tIn << ','
    << pxOut << ',' << pyOut << ',' << pzOut << ',' << pOut << ',' << tOut << '\n';
}
