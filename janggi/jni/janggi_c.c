#include "janggi_c.h"
#include "inter.h"
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#define DEVICE "/dev/dev_driver"

int i=0;
JNIEXPORT void JNICALL Java_com_example_janggi_GameActivity_00024GameBoard_00024interrupt_1thread_sense_1interrupt
  (JNIEnv *env, jobject obj){
	int dev;
	char buf[2] = {0,};
	dev = open("/dev/dev_driver",O_RDWR);
	write(dev,buf,2);
	close(dev);
}

JNIEXPORT void JNICALL Java_com_example_janggi_GameActivity_00024GameBoard_timer_1start
  (JNIEnv *env, jobject obj){
	int dev;
	int cmd;
	char op;
	unsigned int data;
	op='s';
	cmd=syscall(378,&op);
	dev=open(DEVICE,O_RDWR);
	ioctl(dev,cmd,&data);
	i++;
	__android_log_print(ANDROID_LOG_INFO,"ioctl","%d",i);
	return;
}

JNIEXPORT void JNICALL Java_com_example_janggi_GameActivity_00024GameBoard_timer_1turn_1change
  (JNIEnv *env, jobject obj){
	int dev;
	int cmd;
	char op;
	unsigned int data;
	op='t';
	cmd=syscall(378,&op);
	dev=open(DEVICE,O_RDWR);
	ioctl(dev,cmd,&data);
	i++;
	__android_log_print(ANDROID_LOG_INFO,"i","%d",i);
	return;
}

JNIEXPORT void JNICALL Java_com_example_janggi_GameActivity_00024GameBoard_timer_1end
  (JNIEnv *env, jobject obj){
	int dev;
	int cmd;
	char op;
	unsigned int data;
	op='e';
	cmd=syscall(378,&op);
	dev=open(DEVICE,O_RDWR);
	ioctl(dev,cmd,&data);
	return;
}
jint board[10][9];
const int BLK=9;
const int B_CH=1;
const int B_KI=2;
const int B_MA=3;
const int B_PA=4;
const int B_PO=5;
const int B_SA=6;
const int B_SAN=7;
const int R_CH=11;
const int R_KI=12;
const int R_MA=13;
const int R_PA=14;
const int R_PO=15;
const int R_SA=16;
const int R_SAN=17;
const int row=10;
const int col=9;
void blue_ch(int r,int c,int * result);
void blue_ki(int r,int c,int * result);
void blue_ma(int r,int c,int * result);
void blue_pa(int r,int c,int * result);
void blue_po(int r,int c,int * result);
void blue_sa(int r,int c,int * result);
void blue_san(int r,int c,int * result);
void red_ch(int r,int c,int * result);
void red_ki(int r,int c,int * result);
void red_ma(int r,int c,int * result);
void red_pa(int r,int c,int * result);
void red_po(int r,int c,int * result);
void red_sa(int r,int c,int * result);
void red_san(int r,int c,int * result);
JNIEXPORT jintArray JNICALL Java_com_example_janggi_GameActivity_00024GameBoard_Poss_1pos
  (JNIEnv *env, jobject obj, jint r, jint c,jobjectArray Board){
	int i,j;
	int turn=1;
	int result[40];
	for(i=0;i<10;i++){
		jintArray jia = (jintArray)(*env)->GetObjectArrayElement(env,Board,i);
		jint *body=(*env)->GetIntArrayElements(env,jia,0);
		for(j=0;j<9;j++)
			board[i][j]=body[j];
	}
	for(i=0;i<40;i++) result[i]=-1;
	if(board[r][c]<10) turn=0;
	switch(board[r][c]){
		case B_CH:
			blue_ch(r,c,result);
			break;
		case B_KI:
			blue_ki(r,c,result);
			break;
		case B_MA:
			blue_ma(r,c,result);
			break;
		case B_PA:
			blue_pa(r,c,result);
			break;
		case B_PO:
			blue_po(r,c,result);
			break;
		case B_SA:
			blue_sa(r,c,result);
			break;
		case B_SAN:
			blue_san(r,c,result);
			break;
		case R_CH:
			red_ch(r,c,result);
			break;
		case R_KI:
			red_ki(r,c,result);
			break;
		case R_MA:
			red_ma(r,c,result);
			break;
		case R_PA:
			red_pa(r,c,result);
			break;
		case R_PO:
			red_po(r,c,result);
			break;
		case R_SA:
			red_sa(r,c,result);
			break;
		case R_SAN:
			red_san(r,c,result);
			break;
	}
	jintArray jresult;
	jresult = (*env)->NewIntArray(env, 40);
	(*env)->SetIntArrayRegion(env, jresult, 0, 40, result);
	return jresult;
}
//__android_log_print(ANDROID_LOG_INFO,"jni","%d",board[new_r][new_c]);
void blue_ch(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	int i;
	for(i=c+1;i<9;i++){
		if(board[r][i]>9){
			result[idx_r++]=r;
			result[idx_c++]=i;
			break;
		}
		else if(board[r][i]<9){
			break;
		}
		else{
			result[idx_r++]=r;
			result[idx_c++]=i;
		}
	}
	for(i=c-1;i>=0;i--){
		if(board[r][i]>9){
			result[idx_r++]=r;
			result[idx_c++]=i;
			break;
		}
		else if(board[r][i]<9){
			break;
		}
		else{
			result[idx_r++]=r;
			result[idx_c++]=i;
		}
	}
	for(i=r+1;i<10;i++){
		if(board[i][c]>9){
			result[idx_r++]=i;
			result[idx_c++]=c;
			break;
		}
		else if(board[i][c]<9)
			break;
		else{
			result[idx_r++]=i;
			result[idx_c++]=c;
		}
	}
	for(i=r-1;i>=0;i--){
		if(board[i][c]>9){
			result[idx_r++]=i;
			result[idx_c++]=c;
			break;
		}
		else if(board[i][c]<9)
			break;
		else{
			result[idx_r++]=i;
			result[idx_c++]=c;
		}
	}
}
void blue_ki(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	int i,j;
	for(i=r-1;i<=r+1;i++){
		if(i<0 || i>2) continue;
		for(j=c-1;j<=c+1;j++){
			if(c<3 || c>5) continue;
			if((r+c)%2==0){
				if((i+j)%2==0) continue;
			}
			if(board[i][j]>=9){
				result[idx_r++]=i;
				result[idx_c++]=j;
			}
		}
	}
}
void blue_ma(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	if(r+2<10 && c+1<9 && board[r+2][c+1]>=9 && board[r+1][c]==9){
		result[idx_r++]=r+2;
		result[idx_c++]=c+1;
	}
	if(r+2<10 && c-1>=0 && board[r+2][c-1]>=9 && board[r+1][c]==9){
		result[idx_r++]=r+2;
		result[idx_c++]=c-1;
	}
	if(r-2>=0 && c+1<9 && board[r-2][c+1]>=9 && board[r-1][c]==9){
		result[idx_r++]=r-2;
		result[idx_c++]=c+1;
	}
	if(r-2>=0 && c-1>=0 && board[r-2][c-1]>=9 && board[r-1][c]==9){
		result[idx_r++]=r-2;
		result[idx_c++]=c-1;
	}

	if(r+1<10 && c+2<9 && board[r+1][c+2]>=9 && board[r][c+1]==9){
		result[idx_r++]=r+1;
		result[idx_c++]=c+2;
	}
	if(r+1<10 && c-2>=0 && board[r+1][c-2]>=9 && board[r][c-1]==9){
		result[idx_r++]=r+1;
		result[idx_c++]=c-2;
	}
	if(r-1>=0 && c+2<9 && board[r-1][c+2]>=9 && board[r][c+1]==9){
		result[idx_r++]=r-1;
		result[idx_c++]=c+2;
	}
	if(r-1>=0 && c-2>=0 && board[r-1][c-2]>=9 && board[r][c-1]==9){
		result[idx_r++]=r-1;
		result[idx_c++]=c-2;
	}
}
void blue_pa(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	if(r+1<10 && board[r+1][c]>=9){
		result[idx_r++]=r+1;
		result[idx_c++]=c;
	}
	if(c+1<9 && board[r][c+1]>=9){
		result[idx_r++]=r;
		result[idx_c++]=c+1;
	}
	if(c-1>=0 && board[r][c-1]>=9){
		result[idx_r++]=r;
		result[idx_c++]=c-1;
	}
}
void blue_po(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	int i;
	int check=0;
	for(i=r+1;i<10;i++){
		if(board[i][c]==15 || board[i][c]==5) break;
		if(check==1){
			if(board[i][c]>9){
				result[idx_r++]=i;
				result[idx_c++]=c;
				break;
			}
			else if(board[i][c]==9){
				result[idx_r++]=i;
				result[idx_c++]=c;
			}
			else break;
		}
		if(board[i][c]!=9) check=1;
	}
	check=0;
	for(i=r-1;i>=0;i--){
		if(board[i][c]==15  || board[i][c]==5 ) break;
		if(check==1){
			if(board[i][c]>9){
				result[idx_r++]=i;
				result[idx_c++]=c;
				break;
			}
			else if(board[i][c]==9){
				result[idx_r++]=i;
				result[idx_c++]=c;
			}
			else break;
		}
		if(board[i][c]!=9) check=1;
	}
	check=0;
	for(i=c+1;i<9;i++){
		if(board[r][i]==15  || board[r][i]==5) break;
		if(check==1){
			if(board[r][i]>9){
				result[idx_r++]=r;
				result[idx_c++]=i;
				break;
			}
			else if(board[r][i]==9){
				result[idx_r++]=r;
				result[idx_c++]=i;
			}
			else break;
		}
		if(board[r][i]!=9) check=1;
	}
	check=0;
	for(i=c-1;i>=0;i--){
		if(board[r][i]==15  || board[r][i]==5) break;
		if(check==1){
			if(board[r][i]>9){
				result[idx_r++]=r;
				result[idx_c++]=i;
				break;
			}
			else if(board[r][i]==9){
				result[idx_r++]=r;
				result[idx_c++]=i;
			}
			else break;
		}
		if(board[r][i]!=9) check=1;
	}
}
void blue_sa(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	int i,j;
	for(i=r-1;i<=r+1;i++){
		if(i<0 || i>2) continue;
		for(j=c-1;j<=c+1;j++){
			if(c<3 || c>5) continue;
			if((r+c)%2==0){
				if((i+j)%2==0) continue;
			}
			if(board[i][j]>=9){
				result[idx_r++]=i;
				result[idx_c++]=j;
			}
		}
	}
}
void blue_san(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	if(r+3<10 && c+2<9 && board[r+3][c+2]>=9 && board[r+1][c]==9 && board[r+2][c+1]==9){
		result[idx_r++]=r+3;
		result[idx_c++]=c+2;
	}
	if(r+3<10 && c-2>=0 && board[r+3][c-2]>=9 && board[r+1][c]==9 && board[r+2][c-1]==9){
		result[idx_r++]=r+3;
		result[idx_c++]=c-2;
	}
	if(r-3>=0 && c+2<9 && board[r-3][c+2]>=9 && board[r-1][c]==9 && board[r-2][c+1]==9){
		result[idx_r++]=r-3;
		result[idx_c++]=c+2;
	}
	if(r-3>=0 && c-2>=0 && board[r-3][c-2]>=9 && board[r-1][c]==9 && board[r-2][c-1]==9){
		result[idx_r++]=r-3;
		result[idx_c++]=c-2;
	}

	if(r+2<10 && c+3<9 && board[r+2][c+3]>=9 && board[r][c+1]==9 && board[r+1][c+2]==9){
		result[idx_r++]=r+2;
		result[idx_c++]=c+3;
	}
	if(r+2<10 && c-3>=0 && board[r+2][c-3]>=9 && board[r][c-1]==9 && board[r+1][c-2]==9){
		result[idx_r++]=r+2;
		result[idx_c++]=c-3;
	}
	if(r-2>=0 && c+3<9 && board[r-2][c+3]>=9 && board[r][c+1]==9 && board[r-1][c+2]==9){
		result[idx_r++]=r-2;
		result[idx_c++]=c+3;
	}
	if(r-2>=0 && c-3>=0 && board[r-2][c-3]>=9 && board[r][c-1]==9 && board[r-1][c-2]==9){
		result[idx_r++]=r-2;
		result[idx_c++]=c-3;
	}
}

void red_ch(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	int i;
	for(i=c+1;i<9;i++){
		if(board[r][i]<9){
			result[idx_r++]=r;
			result[idx_c++]=i;
			break;
		}
		else if(board[r][i]>9){
			break;
		}
		else{
			result[idx_r++]=r;
			result[idx_c++]=i;
		}
	}
	for(i=c-1;i>=0;i--){
		if(board[r][i]<9){
			result[idx_r++]=r;
			result[idx_c++]=i;
			break;
		}
		else if(board[r][i]>9){
			break;
		}
		else{
			result[idx_r++]=r;
			result[idx_c++]=i;
		}
	}
	for(i=r+1;i<10;i++){
		if(board[i][c]<9){
			result[idx_r++]=i;
			result[idx_c++]=c;
			break;
		}
		else if(board[i][c]>9)
			break;
		else{
			result[idx_r++]=i;
			result[idx_c++]=c;
		}
	}
	for(i=r-1;i>=0;i--){
		if(board[i][c]<9){
			result[idx_r++]=i;
			result[idx_c++]=c;
			break;
		}
		else if(board[i][c]>9)
			break;
		else{
			result[idx_r++]=i;
			result[idx_c++]=c;
		}
	}
}
void red_ki(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	int i,j;
	for(i=r-1;i<=r+1;i++){
		if(i>9 || i<7) continue;
		for(j=c-1;j<=c+1;j++){
			if(c<3 || c>5) continue;
			if((r+c)%2==1){
				if((i+j)%2==1) continue;
			}
			if(board[i][j]<=9){
				result[idx_r++]=i;
				result[idx_c++]=j;
			}
		}
	}
}
void red_ma(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	if(r+2<10 && c+1<9 && board[r+2][c+1]<=9 && board[r+1][c]==9){
		result[idx_r++]=r+2;
		result[idx_c++]=c+1;
	}
	if(r+2<10 && c-1>=0 && board[r+2][c-1]<=9 && board[r+1][c]==9){
		result[idx_r++]=r+2;
		result[idx_c++]=c-1;
	}
	if(r-2>=0 && c+1<9 && board[r-2][c+1]<=9 && board[r-1][c]==9){
		result[idx_r++]=r-2;
		result[idx_c++]=c+1;
	}
	if(r-2>=0 && c-1>=0 && board[r-2][c-1]<=9 && board[r-1][c]==9){
		result[idx_r++]=r-2;
		result[idx_c++]=c-1;
	}

	if(r+1<10 && c+2<9 && board[r+1][c+2]<=9 && board[r][c+1]==9){
		result[idx_r++]=r+1;
		result[idx_c++]=c+2;
	}
	if(r+1<10 && c-2>=0 && board[r+1][c-2]<=9 && board[r][c-1]==9){
		result[idx_r++]=r+1;
		result[idx_c++]=c-2;
	}
	if(r-1>=0 && c+2<9 && board[r-1][c+2]<=9 && board[r][c+1]==9){
		result[idx_r++]=r-1;
		result[idx_c++]=c+2;
	}
	if(r-1>=0 && c-2>=0 && board[r-1][c-2]<=9 && board[r][c-1]==9){
		result[idx_r++]=r-1;
		result[idx_c++]=c-2;
	}
}
void red_pa(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	if(r-1>=0 && board[r-1][c]<=9){
		result[idx_r++]=r-1;
		result[idx_c++]=c;
	}
	if(c+1<9 && board[r][c+1]<=9){
		result[idx_r++]=r;
		result[idx_c++]=c+1;
	}
	if(c-1>=0 && board[r][c-1]<=9){
		result[idx_r++]=r;
		result[idx_c++]=c-1;
	}
}
void red_po(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	int i;
	int check=0;
	for(i=r+1;i<10;i++){
		if(board[i][c]==15  || board[i][c]== 5) break;
		if(check==1){
			if(board[i][c]<9){
				result[idx_r++]=i;
				result[idx_c++]=c;
				break;
			}
			else if(board[i][c]==9){
				result[idx_r++]=i;
				result[idx_c++]=c;
			}
			else break;
		}
		if(board[i][c]!=9) check=1;
	}
	check=0;
	for(i=r-1;i>=0;i--){
		if(board[i][c]==15  || board[i][c]==5 ) break;
		if(check==1){
			if(board[i][c]<9){
				result[idx_r++]=i;
				result[idx_c++]=c;
				break;
			}
			else if(board[i][c]==9){
				result[idx_r++]=i;
				result[idx_c++]=c;
			}
			else break;
		}
		if(board[i][c]!=9) check=1;
	}
	check=0;
	for(i=c+1;i<9;i++){
		if(board[r][i]==15  || board[r][i]==5) break;
		if(check==1){
			if(board[r][i]<9){
				result[idx_r++]=r;
				result[idx_c++]=i;
				break;
			}
			else if(board[r][i]==9){
				result[idx_r++]=r;
				result[idx_c++]=i;
			}
			else break;
		}
		if(board[r][i]!=9) check=1;
	}
	check=0;
	for(i=c-1;i>=0;i--){
		if(board[r][i]==15  || board[r][i]==5 ) break;
		if(check==1){
			if(board[r][i]<9){
				result[idx_r++]=r;
				result[idx_c++]=i;
				break;
			}
			else if(board[r][i]==9){
				result[idx_r++]=r;
				result[idx_c++]=i;
			}
			else break;
		}
		if(board[r][i]!=9) check=1;
	}
}
void red_sa(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	int i,j;
	for(i=r-1;i<=r+1;i++){
		if(i>9 || i<7) continue;
		for(j=c-1;j<=c+1;j++){
			if(c<3 || c>5) continue;
			if((r+c)%2==1){
				if((i+j)%2==1) continue;
			}
			if(board[i][j]<=9){
				result[idx_r++]=i;
				result[idx_c++]=j;
			}
		}
	}
}
void red_san(int r,int c,int * result){
	int idx_r=0,idx_c=20;
	if(r+3<10 && c+2<9 && board[r+3][c+2]<=9 && board[r+1][c]==9 && board[r+2][c+1]==9){
		result[idx_r++]=r+3;
		result[idx_c++]=c+2;
	}
	if(r+3<10 && c-2>=0 && board[r+3][c-2]<=9 && board[r+1][c]==9 && board[r+2][c-1]==9){
		result[idx_r++]=r+3;
		result[idx_c++]=c-2;
	}
	if(r-3>=0 && c+2<9 && board[r-3][c+2]<=9 && board[r-1][c]==9 && board[r-2][c+1]==9){
		result[idx_r++]=r-3;
		result[idx_c++]=c+2;
	}
	if(r-3>=0 && c-2>=0 && board[r-3][c-2]<=9 && board[r-1][c]==9 && board[r-2][c-1]==9){
		result[idx_r++]=r-3;
		result[idx_c++]=c-2;
	}

	if(r+2<10 && c+3<9 && board[r+2][c+3]<=9 && board[r][c+1]==9 && board[r+1][c+2]==9){
		result[idx_r++]=r+2;
		result[idx_c++]=c+3;
	}
	if(r+2<10 && c-3>=0 && board[r+2][c-3]<=9 && board[r][c-1]==9 && board[r+1][c-2]==9){
		result[idx_r++]=r+2;
		result[idx_c++]=c-3;
	}
	if(r-2>=0 && c+3<9 && board[r-2][c+3]<=9 && board[r][c+1]==9 && board[r-1][c+2]==9){
		result[idx_r++]=r-2;
		result[idx_c++]=c+3;
	}
	if(r-2>=0 && c-3>=0 && board[r-2][c-3]<=9 && board[r][c-1]==9 && board[r-1][c-2]==9){
		result[idx_r++]=r-2;
		result[idx_c++]=c-3;
	}
}
