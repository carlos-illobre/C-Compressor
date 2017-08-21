#include "SimpleNode.h"


inline size_t SimpleNode_sizeOf() {
    return sizeof(void*) + sizeof(SimpleNode*);
}
