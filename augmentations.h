

#ifndef SUBTREE_SIZE
    #define SUBTREE_SIZE
#endif

#ifndef SUBTREE_SUM
    #define SUBTREE_SUM
#endif

#ifndef SUBTREE_MIN
    #define SUBTREE_MIN
#endif

#ifndef SUBTREE_MAX
    #define SUBTREE_MAX
#endif

#ifndef POINT_UPDATE
    #define POINT_UPDATE
#endif

#ifndef SUBTREE_INCREMENT
    #define SUBTREE_INCREMENT
    #ifdef SUBTREE_SUM
        #ifndef SUBTREE_SIZE
            #define SUBTREE_SIZE
        #endif
    #endif
#endif