#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_CS     2
#define TFT_DC     15
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

#define analogInPin 32 
int sensorValue = 0;        
float outputValue = 0.0; 

#define LTBLUE    0xB6DF
#define LTTEAL    0xBF5F
#define LTGREEN   0xBFF7
#define LTCYAN    0xC7FF
#define LTRED     0xFD34
#define LTMAGENTA 0xFD5F
#define LTYELLOW  0xFFF8
#define LTORANGE  0xFE73
#define LTPINK    0xFDDF
#define LTPURPLE  0xCCFF
#define LTGREY    0xE71C

#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000

#define DKBLUE    0x000D
#define DKTEAL    0x020C
#define DKGREEN   0x03E0
#define DKCYAN    0x03EF
#define DKRED     0x6000
#define DKMAGENTA 0x8008
#define DKYELLOW  0x8400
#define DKORANGE  0x8200
#define DKPINK    0x9009
#define DKPURPLE  0x4010
#define DKGREY    0x4A49

double a1, b1, c1, d1, phtnh;

boolean display1 = true;
double ox , oy ;
double y;
int x, xmin, xmax;

void setup() {
    x = 0; xmin = 0; xmax=60;
    Serial.begin(115200);
    tft.begin();
    tft.fillScreen(BLACK);
    tft.setRotation(1);
  
    a1 = 3.354016E-03 ;
    b1 = 2.569850E-04 ;
    c1 = 2.620131E-06 ;
    d1 = 6.383091E-08 ;

    delay(1000);
}

void loop() {
    sensorValue = analogRead(analogInPin);
    outputValue = (-0.0693 * sensorValue) + 7.3855;
    phtnh = outputValue;
    y = phtnh;
  
    if (x < xmax ){
        x++;
    } else {
        xmin = xmax;
        xmax = xmax + 60;
        display1 = true;
        tft.fillScreen(BLACK);
        delay(500);
    }

    if (x == 240){
        x = 0;
        xmin = 0;
        xmax = 60;
        display1 = true;
        tft.fillScreen(BLACK);
    }
  
    Graph(tft, x, y, 30, 210, 270, 180, xmin, xmax, 10, -10, 10, 2, "     Ph Tanah", " Time [s]", "Ph Tanah [deg c]", DKBLUE, RED, GREEN, WHITE, BLACK, display1); 
    Serial.print("Ph Tanah:"); Serial.println(y);
    delay(1000);
}

void Graph(Adafruit_ILI9341 &d, double x, double y, double gx, double gy, double w, double h, double xlo, double xhi, double xinc, double ylo, double yhi, double yinc, String title, String xlabel, String ylabel, unsigned int gcolor, unsigned int acolor, unsigned int pcolor, unsigned int tcolor, unsigned int bcolor, boolean &redraw) {
    double ydiv, xdiv;
    if (redraw == true) {
        redraw = false;
        ox = (x - xlo) * (w) / (xhi - xlo) + gx;
        oy = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
        
        // Adjusting y-axis division to multiples of 2 with 0 at the center
        ylo = -10;
        yhi = 10;
        yinc = 2;

        for (double i = ylo; i <= yhi; i += yinc) {
            double ph = (i - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
            d.drawLine(gx, ph, gx + w, ph, (i == 0) ? acolor : gcolor);
            d.setTextSize(1);
            d.setTextColor(tcolor, bcolor);
            d.setCursor(gx - 20, ph);
            d.println(int(i)); // Print y-axis label (rounded integer)
        }
        
        for (double i = xlo; i <= xhi; i += xinc) {
            double ph = (i - xlo) * (w) / (xhi - xlo) + gx;
            d.drawLine(ph, gy, ph, gy - h, (i == 0) ? acolor : gcolor);
            d.setTextSize(1);
            d.setTextColor(tcolor, bcolor);
            d.setCursor(ph - 5, gy + 10);
            d.println(int(i)); // Print x-axis label (rounded integer)
        }

        d.setTextSize(2);
        d.setTextColor(tcolor, bcolor);
        d.setCursor(gx , gy - h - 30);
        d.println(title);

        d.setTextSize(1);
        d.setTextColor(acolor, bcolor);
        d.setCursor(gx , gy + 20);
        d.println(xlabel);

        d.setTextSize(1);
        d.setTextColor(acolor, bcolor);
        d.setCursor(gx - 30, gy - h - 10);
        d.println(ylabel);
    }

    x = (x - xlo) * (w) / (xhi - xlo) + gx;
    y = (y - ylo) * (gy - h - gy) / (yhi - ylo) + gy;
    d.drawLine(ox, oy, x, y, pcolor);
    d.drawLine(ox, oy + 1, x, y + 1, pcolor);
    d.drawLine(ox, oy - 1, x, y - 1, pcolor);
    ox = x;
    oy = y;
}
