#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define PROC_PATH "/proc/"
#define PID_FILE "/test.pid"
#define R_RUNNING_PID_1 1

// Define the functions
int set_pid();
int getProcState(int iTempPid, const char* strTempName);
int getPid(const char* strPath);


int main() {

	if(R_RUNNING_PID_1 == getProcState(getPid(PID_FILE), PROGRAM_NAME)) {
		printf(" Already Running (PID : %d) , Current  (PID %d) \n", getPid(PID_FILE), getpid());
		return -1;
	}

	set_pid();

	int i = 0;
	while(true)
	{
		printf("time loop : %d\n",i);
		sleep(1);	
		i++;
	}

	return 0;
}

int set_pid() {
	FILE* fp = fopen (PID_FILE, "w" );
	if (fp) {
		fprintf (fp, "%d", getpid());
		fclose (fp);
		printf ("Save PID %d \n", getpid());
	}
	else {
		printf (" can not open file %s\n ",PID_FILE);
		return 0;
	}
	return 1;
}



// @return
//  return 1;              // running pid
//  return -1;             // file open error
//  return -2;             // PID Error
//  return -3;             // infomation mismatch
int getProcState(int iTempPid, const char* strTempName)
{
	char strPath[512];      // /proc 하위 파일 경로
	char strTmpPatch[512];
	char strTemp[512];
	char strRealName[512];
	memset(strPath, 0, 512);
	memset(strTemp, 0, 512);
	memset(strRealName, 0, 512);

	int iRealPid = 0;
	FILE * fp = NULL;

	if(iTempPid < 2)
	{
		// printf(" Invalid %s pid (%d)\n", strTempName, iTempPid);
		return -2;          // PID Error
	}
	sprintf(strPath,"%s/%d/status", PROC_PATH, iTempPid);

	//  printf(0, "[A] strPath = %s\n",strPath);

	if( (fp = fopen(strPath,"r")) == NULL)
	{
		// printf ("can not open status file (%s, %s)\n", strPath, strTempName);
		return -1;          // file open error
	}

	while(fscanf(fp,"%s", strTemp) != EOF)
	{
		//  printf(" strTemp = %s\n",strTemp);
		if(strcmp(strTemp,"Name:") == 0)
		{
			memset(strTemp, 0, 128);
			fscanf(fp,"%s", strTemp);
			strncpy(strRealName, strTemp, 128);
			memset(strTemp, 0, 128);
		}
		else if(strcmp(strTemp,"Pid:") == 0)
		{
			memset(strTemp, 0, 128);
			fscanf(fp, "%s", strTemp);
			iRealPid = atoi(strTemp);
			memset(strTemp, 0, 128);
		}
		if(iRealPid != 0)
		{
			break;
		}
	}
	fclose(fp);

	printf("strRealName = %s, iRealPid = %d\n",strRealName, iRealPid);
	
	// 프로그램명의 최대길이는 15바이트로 기존에 strcmp에서 strncmp(,,15)로 변경
	int name_len = 15;
	if(strlen(strTempName) < name_len) {
		name_len = strlen(strTempName);
	}

	if(strncmp(strTempName, strRealName, name_len) == 0 && iTempPid == iRealPid) {
		return 1;  // running pid
	}
	else {
		return -3; // infomation mismatch
	}
}


int getPid(const char* strPath)
{
	FILE * fp = NULL;
	int iPid = 0;
	if( (fp = fopen(strPath, "r")) == NULL)
	{
		printf(" can not open PID file(%s)\n", strPath);
		return -1;          // file open error
	}

	fscanf(fp,"%d", &iPid);

	fclose(fp);

	if(iPid < 2)
	{
		printf(" Invalid pid(%d)\n",iPid);
		return -2;          // PID Error
	}

	return iPid;
}





