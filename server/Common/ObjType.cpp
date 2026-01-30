#include"ObjType.h"

POOL_INST(Message_log)
POOL_INST(Message_CreateScene);
POOL_INST(Message_Transfer);
POOL_INST(ObjPlayer);
POOL_INST(ObjMonster);
POOL_INST(Scene);

ObjPoolMonitorer g_ObjPoolMonitorer;
void ObjPoolMonitorer::Init()
{
    POOL_MONITOR(Message_log)
    POOL_MONITOR(Message_CreateScene);
    POOL_MONITOR(Message_Transfer);
    POOL_MONITOR(ObjPlayer);
    POOL_MONITOR(ObjMonster);
    POOL_MONITOR(Scene);
}




