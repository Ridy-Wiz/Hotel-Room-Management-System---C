#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BOOKINGS 1000
#define MAX_LINE 512

typedef struct {
    int roomNo;
    char name[64];
    char nid[32];
    char phone[32];
    char address[128];
    char checkinDate[11];
    char checkinTime[6];
    char checkoutDate[11];
    char checkoutTime[6];
} Booking;

typedef struct {
    int roomNo;
    int floor;
    char type[32];
    float costPerNight;
} Room;

Booking bookings[MAX_BOOKINGS];
int bookingCount = 0;

Room *roomList = NULL;
int totalRooms = 0;
int roomCapacity = 100;

static void clearInputBuffer(void){
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void rstrip(char *s){
    int n = (int)strlen(s);
    while (n > 0 && (s[n-1] == '\n' || s[n-1] == '\r' || isspace((unsigned char)s[n-1])))
        s[--n] = '\0';
}

static int isLeap(int y){
    return ((y%400==0) || (y%4==0 && y%100!=0));
}

static int daysInMonth(int m, int y){
    int dpm[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if(m==2 && isLeap(y)) return 29;
    return dpm[m];
}

static int isValidDate(int d,int m,int y){
    if(y<1900 || y>3000) return 0;
    int dim = daysInMonth(m,y);
    return (d>=1 && d<=dim);
}

static int isValidTime(int h,int m){
    return (h>=0 && h<=23 && m>=0 && m<=59);
}

static long long parseDateTimeKey(const char *date, const char *time){
    int D,M,Y,h,mi;
    if (sscanf(date,"%2d/%2d/%4d",&D,&M,&Y)!=3) return -1;
    if (sscanf(time,"%2d:%2d",&h,&mi)!=2) return -1;
    if (!isValidDate(D,M,Y)||!isValidTime(h,mi)) return -1;
    return (long long)Y*100000000LL + M*1000000LL + D*10000LL + h*100LL + mi;
}

static int intervalsOverlap(long long a1,long long a2,long long b1,long long b2){
    return (a1<b2)&&(b1<a2);
}

static int isActiveAt(const Booking *b,const char *date,const char *time){
    long long q=parseDateTimeKey(date,time);
    long long cin=parseDateTimeKey(b->checkinDate,b->checkinTime);
    long long cout=parseDateTimeKey(b->checkoutDate,b->checkoutTime);
    return (q>=cin && q<cout);
}

static int isRoomAvailableForInterval(int roomNo,const char *cinD,const char *cinT,const char *coutD,const char *coutT){
    long long cinKey=parseDateTimeKey(cinD,cinT);
    long long coutKey=parseDateTimeKey(coutD,coutT);
    if (!(cinKey<coutKey)) return 0;

    for(int i=0;i<bookingCount;i++){
        if(bookings[i].roomNo!=roomNo) continue;
        long long bIn=parseDateTimeKey(bookings[i].checkinDate,bookings[i].checkinTime);
        long long bOut=parseDateTimeKey(bookings[i].checkoutDate,bookings[i].checkoutTime);
        if(intervalsOverlap(cinKey,coutKey,bIn,bOut)) return 0;
    }
    return 1;
}

int main(){
    printf("Hotel Management System Loaded Successfully!\n");
    printf("Note: Use rooms.txt and booked.txt for full functionality.\n");
    return 0;
}
