#ifndef DATA_RECORDER_HH
#define DATA_RECORDER_HH

#include <fstream>
#include <mutex>
#include <string>

class DataRecorder {
public:
    static DataRecorder& Instance();
    
    void Open(const std::string& filename);
    void Close();
    void WriteRow(int eventID, int trackID, int pdg, double pxIn, double pyIn, double pzIn, double pIn, double tIn, double pxOut, double pyOut, double pzOut, double pOut, double tOut);

private:
    DataRecorder() = default;
    ~DataRecorder() { Close(); }
    DataRecorder(const DataRecorder&) = delete;
    DataRecorder& operator=(const DataRecorder&) = delete;
    
    std::ofstream fOut;
    std::mutex fMutex;
    bool fHeaderWritten = false;
};

#endif
