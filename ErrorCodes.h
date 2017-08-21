/* 
 * File:   ErrorCodes.h
 * Author: Carlos Illobre
 *
 * Created on 2 de junio de 2011, 00:21
 */

#ifndef ERRORCODES_H
#define	ERRORCODES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>

#define ERROR_CODE_INPUT_FILE_NOT_FOUND 1
#define ERROR_CODE_CANT_CREATE_OUTPUT_FILE 2
#define ERROR_CODE_NOT_FOUND 3
#define ERROR_CODE_DECOMPRESSOR_NOT_IMPLEMENTED 4
#define ERROR_CODE_OUT_OF_MEMORY 5

#define BUFFER_SIZE 1024*8
#define WINDOW_SIZE 256


#ifdef	__cplusplus
}
#endif

#endif	/* ERRORCODES_H */

