#include "ProgressMonitorBlockBufferedReader.h"


int ProgressMonitorBlockBufferedReader_open(ProgressMonitorBlockBufferedReader* this, const char* filename, size_t bufferSize, size_t windowSize) {
    FILE* aux = fopen(filename, "rb");
    if (aux) {
        fseek(aux, 0, SEEK_END);
        ProgressMonitor_init(&(this->progressMonitor), ftell(aux), 1);
        fclose(aux);
    }
    return BlockBufferedReader_open(&(this->reader), filename, bufferSize, windowSize);
}


inline int ProgressMonitorBlockBufferedReader_isOpen(ProgressMonitorBlockBufferedReader* this) {
    return BlockBufferedReader_isOpen(&(this->reader));
}


inline void ProgressMonitorBlockBufferedReader_close(ProgressMonitorBlockBufferedReader* this) {
    BlockBufferedReader_close(&(this->reader));
}


inline int ProgressMonitorBlockBufferedReader_readNextBlock(ProgressMonitorBlockBufferedReader* this) {
    return BlockBufferedReader_readNextBlock(&(this->reader));
}


inline int ProgressMonitorBlockBufferedReader_readNextSize_t(ProgressMonitorBlockBufferedReader* this, size_t* symbol) {
    ProgressMonitor_addProgress(&(this->progressMonitor), sizeof(size_t));
    return BlockBufferedReader_readNextSize_t(&(this->reader), symbol);
}


inline int ProgressMonitorBlockBufferedReader_readNextByte(ProgressMonitorBlockBufferedReader* this, unsigned char* c) {
    ProgressMonitor_incProgress(&(this->progressMonitor));
    return BlockBufferedReader_readNextByte(&(this->reader), c);
}
