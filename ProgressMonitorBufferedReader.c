#include "ProgressMonitorBufferedReader.h"


int ProgressMonitorBufferedReader_open(ProgressMonitorBufferedReader* this, const char* filename, size_t bufferSize) {
    FILE* aux = fopen(filename, "rb");
    if (aux) {
        fseek(aux, 0, SEEK_END);
        ProgressMonitor_init(&(this->progressMonitor), ftell(aux), 1);
        fclose(aux);
    }
    return BufferedReader_open(&(this->reader), filename, bufferSize, 0);
}


inline int ProgressMonitorBufferedReader_isOpen(ProgressMonitorBufferedReader* this) {
    return BufferedReader_isOpen(&(this->reader));
}


inline void ProgressMonitorBufferedReader_close(ProgressMonitorBufferedReader* this) {
    BufferedReader_close(&(this->reader));
}


inline int ProgressMonitorBufferedReader_readByte(ProgressMonitorBufferedReader* this, unsigned char* c) {
    if (!BufferedReader_readNextByte(&(this->reader), c)) return 0;
    ProgressMonitor_incProgress(&(this->progressMonitor));
    return 1;
}

