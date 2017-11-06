rem A simple script to show how to compile matmul and MW o
rem Windows. Only tested with Visual Studio 6

cl -DWINDOWS -DSOCKET_MASTER -DCONDOR_DIR=\".\" -Femaster -TP -GX -GR -I. -I../../src -I../../src/RMComm -I../../src/MWControlTasks ../../src/MWDriver.C ../../src/MWTask.C ../../src/MWWorker.C ../../src/MWGroup.C ../../src/MWprintf.C ../../src/MWStats.C ../../src/MWTaskContainer.C ../../src/MWWorkerID.C  ../../src/MWWinSystem.C ../../src/RMComm/MWRMComm.C ../../src/RMComm/MW-Socket/MWSocketRC.C Driver-Matmul.C Task-Matmul.C Worker-MatMul.C MasterMain-Matmul.C -link wsock32.lib 

cl -DWINDOWS -DCONDOR_DIR=\".\" -Feworker -TP -GX -GR -I. -I../../src -I../../src/RMComm -I../../src/MWControlTasks ../../src/MWDriver.C ../../src/MWTask.C ../../src/MWWorker.C ../../src/MWGroup.C ../../src/MWprintf.C ../../src/MWStats.C ../../src/MWTaskContainer.C ../../src/MWWorkerID.C  ../../src/MWWinSystem.C ../../src/RMComm/MWRMComm.C ../../src/RMComm/MW-Socket/MWSocketRC.C Driver-Matmul.C Task-Matmul.C Worker-MatMul.C WorkerMain-Matmul.C -link wsock32.lib
