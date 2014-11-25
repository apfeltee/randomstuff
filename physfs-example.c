

#include <stdio.h>
#include <physfs.h>


int main(int argc, char* argv[])
{
    PHYSFS_init(NULL);

    {
        PHYSFS_Version compiled;
        PHYSFS_Version linked;

        PHYSFS_VERSION(&compiled);
        PHYSFS_getLinkedVersion(&linked);
        printf("We compiled against PhysFS version %d.%d.%d ...\n",
                compiled.major, compiled.minor, compiled.patch);
        printf("But we linked against PhysFS version %d.%d.%d.\n",
                linked.major, linked.minor, linked.patch);
    }

    {
        const PHYSFS_ArchiveInfo** i;
        for(i = PHYSFS_supportedArchiveTypes(); *i != NULL; i++)
        {
            printf("Supported archive: [%s], which is [%s].\n",
                (*i)->extension, (*i)->description);
        }
    }

    {
        char **cds = PHYSFS_getCdRomDirs();
        char **i;
        for (i = cds; *i != NULL; i++)
            printf("cdrom dir [%s] is available.\n", *i);
        PHYSFS_freeList(cds);
    }

    {
        char **i;
        for (i = PHYSFS_getSearchPath(); *i != NULL; i++)
            printf("[%s] is in the search path.\n", *i);
        PHYSFS_freeList(i);
    }

    PHYSFS_deinit();
}
