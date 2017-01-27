#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>


#define C 30
#define A 20
#define SPEED 15
typedef struct point{
  int x;
  int y;
} point;

struct snake{
  point head;
  point tail[C*A];
  point dir;
  int isDead;
  int size;
} snk;

void init();
void draw();
void update();
int checkDead();
void n_food(int mode);
int kbhit(void);

point food;
char in;
//function kbhit() is used from https://cboard.cprogramming.com/c-programming/63166-kbhit-linux.html
int kbhit(void){
  struct termios oldt, newt;
  int ch;
  int oldf;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);
  if(ch != EOF)
    {
      ungetc(ch, stdin);
      return 1;
    }
  return 0;
}

int main(){
  init();
  while(!snk.isDead){
    if(clock()%(CLOCKS_PER_SEC/SPEED)==0){
      if(kbhit()){
	in=getchar();
	if(in=='w' || in=='W'){
	  if (snk.dir.y==0){
	    snk.dir.x=0;
	    snk.dir.y=-1;
	  }
	}
	if(in=='a' || in=='A'){
	  if (snk.dir.x==0){
	    snk.dir.x=-1;
	    snk.dir.y=0;
	  }
	}
	if(in=='s' || in=='S'){
	  if (snk.dir.y==0){
	    snk.dir.x=0;
	    snk.dir.y=1;
	  }
	}
	if(in=='d' || in=='D'){
	  if (snk.dir.x==0){
	    snk.dir.x=1;
	    snk.dir.y=0;
	  }
	}
      }
      update();
      if (snk.head.x==food.x && snk.head.y==food.y){
	snk.size++;
	n_food(1);
      }
      draw();
      snk.isDead=checkDead();
      if(snk.isDead) break;
    }
  }
  printf("You died with %d points\n",snk.size);
  printf("Try again?(y/n) ");
  scanf("%c",&in);
  if(in=='y' || in=='Y')
    main();
  else return 0;
}

void init(){
  snk.size=0;
  snk.dir.x=1;
  snk.dir.y=0;
  snk.head.x=C/2;
  snk.head.y=A/2;
  snk.isDead=0;
  n_food(1);
}

void update(){
  int i;
  if (snk.size!=0){
    for(i=snk.size-1;i>=0;i--){
      snk.tail[i]=snk.tail[i-1];
    }
    snk.tail[0]=snk.head;
  }
  snk.head.x+=snk.dir.x;
  snk.head.y+=snk.dir.y;
  if (snk.head.x==C+1)
    snk.head.x=0;
  else if (snk.head.x==-1)
    snk.head.x=C;
  if (snk.head.y==A+1)
    snk.head.y=0;
  else if (snk.head.y==-1)
    snk.head.y=A;
}



int checkDead(){
  int i;
  for(i=0;i<snk.size;i++){
    if (snk.head.x==snk.tail[i].x && snk.head.y==snk.tail[i].y)
      return 1;
  }
  return 0;
}

void n_food(int mode){
  srand(time(NULL));
  int i;
  if (mode==1){
    food.x=rand()%(C-2)+1;
    food.y=rand()%(A-2)+1;
    while(snk.head.x==food.x && snk.head.y==food.y){
      food.x=rand()%(C-2)+1;
      food.y=rand()%(A-2)+1;
    }
    for(i=0;i<snk.size;i++){
      while(food.x==snk.tail[i].x && food.y==snk.tail[i].y){
	food.x=rand()%(C-2)+1;
	food.y=rand()%(A-2)+1;
      }
    }
  }
}


void draw(){
  int i,j,k,flag=0;
  system("clear");
  printf("  Points: %d \t",snk.size);
  for(i=0;i<2*C-14;i++){
    printf("-");
  }
  printf("\n");
  for(i=0;i<=A;i++){
    printf("|");
    for(j=0;j<=C;j++){
      if(snk.head.x==j && snk.head.y==i){
	printf("0 ");
	flag=1;
      }
      if (food.x==j && food.y==i && flag==0){
	printf("* ");
	flag=1;
      }
      for(k=0;k<snk.size;k++){
	if(snk.tail[k].x==j && snk.tail[k].y==i && flag==0){
	  printf("o ");
	  flag=1;
	}
      }
      if (flag==0){
	printf("  ");
      }
      flag=0;
    }
    printf("|\n");
  }
  printf("  ");
  for(i=0;i<2*C;i++){
    printf("-");
  }
  printf("\n");
}
