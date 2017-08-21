/* 
 * File:   ProgressMonitorBlockBufferedReader.h
 * Author: Carlos Illobre
 *
 * Created on 10 de abril de 2011, 18:09
 */

#ifndef PROGRESSMONITORBLOCKBUFFEREDREADER_H
#define	PROGRESSMONITORBLOCKBUFFEREDREADER_H

#include "BlockBufferedReader.h"
#include "ProgressMonitor.h"


#ifdef	__cplusplus
extern "C" {
#endif


    typedef struct ProgressMonitorBlockBufferedReader {
        BlockBufferedReader reader;
        ProgressMonitor progressMonitor;
    } ProgressMonitorBlockBufferedReader;


    int ProgressMonitorBlockBufferedReader_open(ProgressMonitorBlockBufferedReader* this, const char* filename, size_t bufferSize, size_t windowSize);

    inline int ProgressMonitorBlockBufferedReader_isOpen(ProgressMonitorBlockBufferedReader* this);

    /**
     * Cierra un AbstractBufferedReader liberando los recursos.
     * @param this El AbstractBufferedReader que se quiere cerrar
     */
    inline void ProgressMonitorBlockBufferedReader_close(ProgressMonitorBlockBufferedReader* this);

    inline int ProgressMonitorBlockBufferedReader_readNextBlock(ProgressMonitorBlockBufferedReader* this);

    inline int ProgressMonitorBlockBufferedReader_readNextSize_t(ProgressMonitorBlockBufferedReader* this, size_t* symbol);


#ifdef	__cplusplus
}
#endif

#endif	/* PROGRESSMONITORBLOCKBUFFEREDREADER_H */

