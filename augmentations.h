// Comment out to disable subtree size queries
#ifndef SUBTREE_SIZE
    #define SUBTREE_SIZE
#endif

// Comment out to disable subtree sum queries
#ifndef SUBTREE_SUM
    #define SUBTREE_SUM
#endif

// Comment out to disable subtree min queries
#ifndef SUBTREE_MIN
    #define SUBTREE_MIN
#endif

// Comment out to disable subtree max queries
#ifndef SUBTREE_MAX
    #define SUBTREE_MAX
#endif

// Comment out to disable single node value updates
#ifndef POINT_UPDATE
    #define POINT_UPDATE
#endif

// Comment out to disable subtree value incrementation updates
#ifndef SUBTREE_INCREMENT
    #define SUBTREE_INCREMENT
    #ifdef SUBTREE_SUM
        #ifndef SUBTREE_SIZE
            #define SUBTREE_SIZE
        #endif
    #endif
#endif