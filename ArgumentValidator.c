#include "ArgumentValidator.h"
#include <string.h>


int ArgumentValidator_validate(int argc, char** argv) {
    switch (argc) {
        case 1:
            return HELP_CODE;
        case 2:
            if (strcmp(argv[1], ARG_COMPR) == 0) return ERROR_CODE_NO_COMPRESS_INPUT_FILE;
            if (strcmp(argv[1], ARG_DESC) == 0) return ERROR_CODE_NO_DECOMPRESS_INPUT_FILE;
            return ERROR_CODE_INVALID_ARGUMENTS;
        case 3:
            if (strcmp(argv[1], ARG_COMPR) == 0 || strcmp(argv[1], ARG_DESC) == 0) return 0;
            return ERROR_CODE_INVALID_ARGUMENTS;
        case 4:
            if ((strcmp(argv[1], ARG_COMPR) == 0 || strcmp(argv[1], ARG_DESC) == 0) &&
                    ArgumentValidator_getCompressAlgoritm(argc, argv) != -1
                    ) return 0;
            return ERROR_CODE_INVALID_ARGUMENTS;
        default:
            return ERROR_CODE_INVALID_ARGUMENTS;
    }
}


int ArgumentValidator_hasCompressArg(int argc, char** argv) {
    return argc > 1 && strcmp(argv[1], ARG_COMPR) == 0;
}


int ArgumentValidator_getCompressAlgoritm(int argc, char** argv) {
    if (argc == 3) return 0;
    if (argc > 3) {
        if (strcmp(argv[3], ARG_NONE) == 0) return ALG_NONE;
        if (strcmp(argv[3], ARG_DHUFFMAN) == 0) return ALG_DHUFFMAN;
        if (strcmp(argv[3], ARG_MTF) == 0) return ALG_MTF;
        if (strcmp(argv[3], ARG_BS) == 0) return ALG_BS;
        if (strcmp(argv[3], ARG_BSMTF) == 0) return ALG_BSMTF;
        if (strcmp(argv[3], ARG_LZ77) == 0) return ALG_LZ77;
        if (strcmp(argv[3], ARG_BSMTFBS) == 0) return ALG_BSMTFBS;
        if (strcmp(argv[3], ARG_BSMTF_DHUFFMAN) == 0) return ALG_BSMTF_DHUFFMAN;
        if (strcmp(argv[3], ARG_LZSS) == 0) return ALG_LZSS;
        if (strcmp(argv[3], ARG_LZHUFF) == 0) return ALG_LZHUFF;
    }
    return -1;
}
