/* 
 * File:   ArgumentValidator.h
 * Author: Carlos Illobre
 *
 * Created on 25 de marzo de 2011, 00:27
 */

#ifndef ARGUMENTVALIDATOR_H
#define	ARGUMENTVALIDATOR_H

#ifdef	__cplusplus
extern "C" {
#endif


#define ARG_COMPR "-c"
#define ARG_DESC "-x"

#define ARG_NONE "-none"
#define ALG_NONE 1

#define ARG_DHUFFMAN "-dhuffman"
#define ALG_DHUFFMAN 2

#define ARG_MTF "-mtf"
#define ALG_MTF 3

#define ARG_BS "-bs"
#define ALG_BS 4

#define ARG_BSMTF "-bs+mtf"
#define ALG_BSMTF 5

#define ARG_LZ77 "-lz77"
#define ALG_LZ77 6

#define ARG_BSMTFBS "-bs+mtf+bs"
#define ALG_BSMTFBS 7

#define ARG_BSMTF_DHUFFMAN "-bs+mtf+dhuffman"
#define ALG_BSMTF_DHUFFMAN 8

#define ARG_LZSS "-lzss"
#define ALG_LZSS 9

#define ARG_LZHUFF "-lzhuff"
#define ALG_LZHUFF 10


#define ERROR_CODE_NO_COMPRESS_INPUT_FILE 1
#define ERROR_CODE_NO_DECOMPRESS_INPUT_FILE 2
#define ERROR_CODE_INVALID_ARGUMENTS 3
#define HELP_CODE 4


    /**
     *
     * @param argc Cantidad de argumentos recibidos
     * @param argv Contiene los argumentos recibidos
     * @return El codigo del mensaje de error si los argumentos son invalidos o
     * cero si no hay errores.
     */
    int ArgumentValidator_validate(int argc, char** argv);

    /**
     *
     * @param argc
     * @param argv
     * @return true si se quiere comprimir y false en otro caso
     */
    int ArgumentValidator_hasCompressArg(int argc, char** argv);

    int ArgumentValidator_getCompressAlgoritm(int argc, char** argv);


#ifdef	__cplusplus
}
#endif

#endif	/* ARGUMENTVALIDATOR_H */

