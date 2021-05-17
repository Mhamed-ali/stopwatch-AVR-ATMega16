void incsec0();
void incsec1();
void incmin0();
void incmin1();
void inchour0();
void inchour1();

void INT0_Init(void);
void INT1_Init(void);
void INT2_Init(void);
void Timer1_CTC_Init(void);
void clock_init();
void reset();
void pause_timer1();
void display();


unsigned char INT0_Flag = 0;
unsigned char INT1_Flag = 0;
unsigned char INT2_Flag = 0;
unsigned char TIME1_Flag = 0;

unsigned char sec0=0;
unsigned char sec1=0;
unsigned char min0=0;
unsigned char min1=0;
unsigned char hour0=0;
unsigned char hour1=0;
