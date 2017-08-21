/* 
 * File:   ProgressMonitorBufferedReader.h
 * Author: mucho
 *
 * Created on 10 de abril de 2011, 20:28
 */

#ifndef PROGRESSMONITORBUFFEREDREADER_H
#define	PROGRESSMONITORBUFFEREDREADER_H

#include "ProgressMonitor.h"
#include "BufferedReader.h"

#ifdef	__cplusplus
extern "C" {
#endif


    typedef struct ProgressMonitorBufferedReader {
        BufferedReader reader;
        ProgressMonitor progressMonitor;
    } ProgressMonitorBufferedReader;

    int ProgressMonitorBufferedReader_open(ProgressMonitorBufferedReader* this, const char* filename, size_t bufferSize);

    inline int ProgressMonitorBufferedReader_isOpen(ProgressMonitorBufferedReader* this);

    /**
     * Cierra un AbstractBufferedReader liberando los recursos.
     * @param this El AbstractBufferedReader que se quiere cerrar
     */
    inline void ProgressMonitorBufferedReader_close(ProgressMonitorBufferedReader* this);

    inline int ProgressMonitorBufferedReader_readByte(ProgressMonitorBufferedReader* this, unsigned char* c);


#ifdef	__cplusplus
}
#endif

#endif	/* PROGRESSMONITORBUFFEREDREADER_H */

