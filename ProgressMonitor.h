/* 
 * File:   ProgressMonitor.h
 * Author: Carlos Illobre
 *
 * Created on 27 de marzo de 2011, 12:35
 */

#ifndef PROGRESSMONITOR_H
#define	PROGRESSMONITOR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <time.h>
#include <stdlib.h>


    typedef struct ProgressMonitor {
        size_t current;
        size_t total;
        char printProgress;
        clock_t endwait;
        time_t startTime;
        size_t printCount;
    } ProgressMonitor;

    /**
     * Inicializa los valores del monitor
     * @param this
     * @param total
     */
    inline void ProgressMonitor_init(ProgressMonitor* this, size_t total, char printProgress);

    /**
     * Incrementa en 1 el valor del progreso
     */
    inline void ProgressMonitor_incProgress(ProgressMonitor* this);

    inline void ProgressMonitor_addProgress(ProgressMonitor* this, size_t progress);


#ifdef	__cplusplus
}
#endif

#endif	/* PROGRESSMONITOR_H */

