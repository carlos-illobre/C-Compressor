#include <stdio.h>

#include "ProgressMonitor.h"
#include <string.h>


#define PROGRESS_MONITOR_PRINT_PERIOD 1.5

#define BACKSPACES "\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b\b \b"

static const char MOTION[] = {'|', '/', '-', '\\'};


void ProgressMonitor_printProgress(ProgressMonitor* this) {
    // Si hay que imprimir
    if (this->printProgress) {
        // Imprimo una vez cada PROGRESS_MONITOR_PRINT_PERIOD segundos
        if (clock() > this->endwait) {
            this->printCount++;
            char perc = (char)(100*this->current/this->total);
            // Borro el porcentaje anterior
            printf(BACKSPACES);
            // Imprimo el porcentaje
            time_t end;
            time(&end);
            double diff = ((double)this->total/(double)this->current -1) * difftime(end, this->startTime);
            printf("%c Completado %d%c. Faltan %dhs %dmin %dseg", MOTION[this->printCount % 4], perc, '%', (int)(diff/3600), (int)(((long)diff%3600)/60), (int)(((long)diff%3600)%60));
            this->endwait = clock() + PROGRESS_MONITOR_PRINT_PERIOD * CLOCKS_PER_SEC;
            // Fuerzo a que lo impreso sin \n aparezca por consola
            fflush(stdout);
        }
        if (this->current == this->total) {
            printf(BACKSPACES);
            fflush(stdout);
        }
    }
}


inline void ProgressMonitor_init(ProgressMonitor* this, size_t total, char printProgress) {
    // Sumo 1 para que no haya divisiones por cero cuando current sea cero
    this->total = total+1;
    this->current = 1;
    this->printProgress = printProgress;
    this->endwait = 0;
    time(&(this->startTime));
    this->printCount = 0;
    ProgressMonitor_printProgress(this);
}


inline void ProgressMonitor_incProgress(ProgressMonitor* this) {
    this->current++;
    ProgressMonitor_printProgress(this);
}


inline void ProgressMonitor_addProgress(ProgressMonitor* this, size_t progress) {
    this->current += progress;
    ProgressMonitor_printProgress(this);
}


