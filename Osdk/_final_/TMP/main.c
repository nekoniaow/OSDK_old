# 1 "main.c"
# 1 "C:\\Users\\laurent\\Desktop\\Dropbox\\Oric\\OSDK\\Osdk\\_final_\\include\\lib.h" 1


void exit(int retval);


int getchar(void);
int putchar(char c);
int printf(const char *format,...);

char *itoa(int n);

int isalpha(char c);
int isupper(char c);
int islower(char c);
int isdigit(char c);
int isspace(char c);
int ispunct(char c);
int isprint(char c);
int iscntrl(char c);
int isascii(char c);
char toupper(char c);
char tolower(char c);
char toascii(char c);

char* sbrk();
char* brk();

char *strcpy(char *s1,const char * s2);
int strcmp(const char *s1,const char * s2);
int strlen(const char *s);

int memcpy(void *dst, void *src, int n);

 

void hires(void);
void text(void);
void ping(void);
void shoot(void);
void zap(void);
void explode(void);
void kbdclick1(void);
void kbdclick2(void);
int ink(int color);
int paper(int color);
int curset(int x,int y,int mode);
int curmov(int dx,int dy,int mode);
int draw(int dx,int dy,int mode);
int circle(int radius,int mode);
int hchar(char c,int charset,int mode);
int fill(int height,int width,char c);
int point(int x,int y);
int pattern(char style);
int play(int soundchanels,int noisechanels,int envelop,int volume);
int music(int chanel,int octave,int key,int volume);
int sound(int chanel,int period,int volume);
void w8912(unsigned char reg,unsigned char value);
char key(void);
char is_overlay_enabled();


 

void cls();
void lores0(void);
void lores1(void);
void gotoxy(int x, int y);  
int  get(void);			    
void cwrite(char c);			   
int  cread();				  
void cwritehdr();		      
void call(int addr);		      

 
 
 
 
 

void sedoric(char *command);		 

 








 
 

int  lprintf(const char *format,...);


 

void file_unpack(void *ptr_dst,void *ptr_src);








 










 
 











 































 
 










 






 


 





extern unsigned char OsdkTableMod6[];
extern unsigned char OsdkTableDiv6[];


 
 
 
extern unsigned int randseedLow;
extern unsigned int randseedTop;
extern unsigned int pcrand();





# 1 "main.c" 2

void main()
{
}
