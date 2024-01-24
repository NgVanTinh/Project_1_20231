#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#include <string>
#include <windows.h>

#define WIDTH 1100 //kich thuoc cua cua so winbgi
#define HEIGHT 730

#define WINDOW_SIZE 512
#define MAX_LAG 256
#define PEAK_THRESHOLD 8000  // nguong dinh

typedef struct header {
    char chunk_id[4];
    unsigned int chunk_size;
    char format[4];
    char subchunk1_id[4];
    unsigned int subchunk1_size;
    unsigned short audio_format;
    unsigned short num_channels;
    unsigned int sample_rate;
    unsigned int byte_rate;
    unsigned short block_align;
    unsigned short bits_per_sample;
    char subchunk2_id[4];
    unsigned int subchunk2_size;
} header; 

typedef struct header* header_p;

// ve duong net dut
void drawDashLineX(int x1, int y1, int x2, int y2,int dashLength, int gapLength) {
    int deltaX = x2 - x1;
    int length = abs(deltaX);

    // Tính toán so luong doan net dua tren chieu dai cua duong
    int numDashes = length / (dashLength + gapLength);

    // Tính toán khoang cach giua cac doan net
    float dashX = deltaX / numDashes;

    int currentX = x1;

    for (int i = 0; i < numDashes; i++) {
        setcolor(YELLOW);
        line(currentX, y1, currentX + dashX, y2);
		
		setcolor(LIGHTGRAY);
		line(currentX + dashX, y1, currentX + dashX + gapLength, y2);
		
        // Di chuyen
        currentX += dashX + gapLength;
        if(currentX + dashX + gapLength > x2) break;
    }
}

void drawDashLineY(int x1, int y1, int x2, int y2,int dashLength, int gapLength) {
    int deltaY = y2 - y1;
    int length = abs(deltaY);

    // Tính toán so luong doan net dua tren chieu dai cua duong
    int numDashes = length / (dashLength + gapLength);

    // Tính toán khoang cach giua cac doan net
    float dashY = deltaY / numDashes;

    int currentY = y1;

    for (int i = 0; i < numDashes; i++) {
        setcolor(YELLOW);
        line(x1, currentY, x2,currentY + dashY);
		
		setcolor(LIGHTGRAY);
		line(x1, currentY + dashY, x2, currentY + dashY + gapLength);
		
        // Di chuyen
        currentY += dashY + gapLength;
        if(currentY > y2) break;
    }
}

// reset cua so ve ham tu tuong quan
void resetDrawAutoCorrelation(int x1, int y1, int x2, int y2){
	
    setcolor(BLACK);

    // Ve vien hinh chu nhat
    rectangle(x1, y1, x2, y2);

    // Ðat mau nen va kieu to mau
    setfillstyle(SOLID_FILL, BLACK);

    // to mau cho nen hcn
    // tao hcn dac mau
    bar(x1 + 1, y1 + 1, x2, y2);
    
    // ve lai truc xy
    
    int x = x1;
    int y = y1;
	for(int i = 1; i <= 3; i++){
		x += (x2 - x1) / 4;
		y += (y2 - y1) / 4;
		drawDashLineX(x1,y,x2,y,4,4);
		drawDashLineY(x,y1,x,y2,4,4);		
	}
    
}

void drawFillRectangle(int left, int top, int right, int bottom, int color){
	
    setcolor(color);

    rectangle(left, top, right, bottom);

     // Ðat mau nen va kieu to mau
    setfillstyle(SOLID_FILL, color);

     // to mau cho nen hcn
    // tao hcn dac mau
    bar(left + 1, top + 1, right, bottom);
}

// ve giao dien

void drawInterface(){

	// ve cac truc toa do xy cua cua so ve dang song	
	drawDashLineX(100,80,1000,80,4,4);
	drawDashLineX(100,130,1000,130,4,4);
	drawDashLineX(100,180,1000,180,4,4);
	
	int x = 100;
	for(int i = 1; i <= 8; i++){
		x += 100;
		drawDashLineY(x,30,x,230,4,4);		
	}
	// ve ca truc cua cua so 512 mau va cua so ham tu tuong quan
	
	resetDrawAutoCorrelation(100,260,520,460);
	resetDrawAutoCorrelation(580,260,1000,460);
	
	// ve cac truc xy cua cua so ve tan so
	drawDashLineX(100,540,1000,540,4,4);
	drawDashLineX(100,590,1000,590,4,4);
	drawDashLineX(100,640,1000,640,4,4);
	
	int x3 = 100;
	for(int i = 1; i <= 8; i++){
		x3 += 100;
		drawDashLineY(x3,490,x3,690,4,4);		
	}
	
	// ve cac khoi hinh xam bao phu 
    drawFillRectangle(0,0,99,720,LIGHTGRAY);
    drawFillRectangle(1000,0,1100,720,LIGHTGRAY);
    
    drawFillRectangle(100,0,1000,30,LIGHTGRAY);
    drawFillRectangle(100,231,1000,260,LIGHTGRAY);
    drawFillRectangle(100,461,1000,490,LIGHTGRAY);
    drawFillRectangle(100,691,1000,720,LIGHTGRAY);
    
    drawFillRectangle(521,261,580,460,LIGHTGRAY);
    
    // ve cac diem 
    char string[20];
    setbkcolor(LIGHTGRAY);
	setcolor(BLACK);
	strcpy(string,"29851");
	outtextxy(40, 30, string);
	strcpy(string,"0");
	outtextxy(85, 130, string);
	strcpy(string,"-29851");
	outtextxy(40, 230, string);
	strcpy(string,"5313.0ms");
	outtextxy(1001, 130, string);
	strcpy(string,"K = 256");
	outtextxy(1001, 360, string);
	strcpy(string,"0");
	outtextxy(70, 691, string);
	strcpy(string,"100");
	outtextxy(70, 640, string);
	strcpy(string,"200");
	outtextxy(70, 590, string);
	strcpy(string,"300");
	outtextxy(70, 540, string);
	strcpy(string,"400Hz");
	outtextxy(60, 490, string);
	
	// ve cac toa do x cua cua so ve dang song va ve tan so
	float number = 0;
	int x1 = 100;
	for(int i = 1; i <= 8; i++){
		x1 += 100;
		number += 483;
		char charArr[10];
		sprintf(charArr, "%.1f", number);
		outtextxy(x1 - 15, 231, charArr);		
	}
	
	number = 0;
	int x2 = 100;
	for(int i = 1; i <= 8; i++){
		x2 += 100;
		number += 483;
		char charArr[10];
		sprintf(charArr, "%.1f", number);
		outtextxy(x2 - 15, 691, charArr);		
	}
	
}

void drawWaveform(int16_t *data, int num_samples, int max_amplitude, int color, int startY) {
//	k/c giua cac mau
	
    float sample_spacing = (float)900 /  num_samples;

    for ( int i = 0; i < num_samples ; ++i) {
        int x1 = 101 + i * sample_spacing; 
        int y1 = startY - (data[i] * 100 / max_amplitude); 
        int x2 = 101 + (i + 1) * sample_spacing; 
        int y2 = startY - (data[i + 1] * 100 / max_amplitude); 

        setcolor(color);
        line(x1, y1, x2, y2);
    }
}

// ve duong doc o wave form tro de vi tri am thanh dang phan tich ham R(k);
void drawLineInWaveForm(int start, float sample_spacing, int color){
	static bool isDrawn = false;
    static int *buffer = NULL;
	
    int x1 = 101 + start * sample_spacing; 
    int y1 = 31;
    int x2 = 101 + (start + WINDOW_SIZE) * sample_spacing; 
    int y2 = 230; 

    if (!isDrawn) {
        int size = imagesize(x1, y1, x2, y2);
        buffer = (int*) malloc(size);
        getimage(x1, y1, x2, y2, buffer);
        setcolor(color);
        rectangle(x1, y1, x2, y2);
        isDrawn = true;
    } else {
        // Khi hàm goi lan thu 2 thi se xoa hinh chu nhat
        putimage(x1, y1, buffer, COPY_PUT);
        free(buffer);
        buffer = NULL;
        isDrawn = false;
    }

}

 //Ham kiem tra tinh tuan hoan va tinh f0
bool find_periodic_peak(float autocorr[], int size, double *f0,int *position) {
    int max_peak_lag = 0;
    double max_peak_value = 0.0;

    // Tim dinh co gia tri tuong quan lon nhat - co nhung tan so sai, bi gap boi len
    for (int i = 10; i < 115 ; i++) {
        if (autocorr[i] > max_peak_value && autocorr[i] > autocorr[i-1] && autocorr[i] > autocorr[i+1]) {
            max_peak_lag = i;
            max_peak_value = autocorr[i];
        }
    }

    if (max_peak_value > PEAK_THRESHOLD ) {
    	*position = max_peak_lag;
        *f0 = 16000 / (double)max_peak_lag;
        float sample_spacing_corr = (float)420.0 /  MAX_LAG;
        int x = 580 + (int) max_peak_lag * sample_spacing_corr;
        setcolor(LIGHTRED);
        line(x,260,x,460); // yeah
        return true; 
    } else {
        return false; 
    }
}



void calculateAndDrawAutoCorrelationAndWWaveForm(int16_t *data, int start, int window_size, float *autocorr, int color, int startY) {
	
	float sample_spacing = (float)420.0 /  window_size;

	// ve do thi dang song cua doan 512 mau dang duyet
    for ( int i = 0; i < window_size ; ++i) {
        int x1 = 100 + i * sample_spacing; 
        int y1 = startY - (data[i + start] * 100 / 29851); 
        int x2 = 100 + (i + 1) * sample_spacing; 
        int y2 = startY - (data[i + start + 1] * 100 / 29851); 

        setcolor(color);
        line(x1, y1, x2, y2);
    }
    // ve do thi ham tu tuong quan
  
    memset(autocorr, 0, sizeof(float) * window_size);

    // Tính ham tu tuong quan 
    for (int delay = 0; delay <= MAX_LAG; ++delay) {
        for (int i = start; i < start + window_size - delay; ++i) {
            autocorr[delay] += data[i] * data[i + delay];
        }
        autocorr[delay] /= (window_size - delay); // Chia trung bình de chuan hoa gia tri ham tuong quan
    }

    // Tìm giá tri max cua ham tu tuong quang trong doan 512 mau dang xet de 
    // xac dinh bien do lon nhat cua do thi ham tu tuong quan 
	
	float max_am = fabs(autocorr[0]); 
	for (int i = 1; i < window_size; ++i) {
	    if (fabs(autocorr[i])> max_am) {
	        max_am = fabs(autocorr[i]);
	    }
	}
		
	float sample_spacing_corr = (float)420.0 /  MAX_LAG;
    // ve do thi ham tu tuong quan
    for (int i = 0; i < MAX_LAG - 1; ++i) {
        int y1 = startY -( (autocorr[i] / max_am) * 90 ); 
        int y2 = startY -( (autocorr[i+1] / max_am) * 90 ); 
        int x1 = 581 + i * sample_spacing_corr;
        int x2 = 581 + (i + 1) * sample_spacing_corr;

        setcolor(color);
        line(x1, y1, x2, y2);
    }
  
}

// truyen vao du lieu, so mau cua du lieu, mau , vi tri bat dau ve
void drawAndClearAutoCorrelation(int16_t *data, int num_samples, int color, int startY) {
    int window_size = WINDOW_SIZE;
    float autocorr[window_size];
    bool isPeriodic = false;
    int start;
	float sample_spacing = (float)900 /  num_samples;
	
    for (start = 0; start <= num_samples - window_size/2; start += window_size/2) {
		resetDrawAutoCorrelation(100,260,520,460);
		resetDrawAutoCorrelation(580,260,1000,460);
		drawLineInWaveForm(start,sample_spacing,WHITE);
        calculateAndDrawAutoCorrelationAndWWaveForm(data, start, window_size, autocorr, color, startY); 
        
        // ve do thi tan so
        int position = 0;
        double f0 = 0.0;
        if (find_periodic_peak(autocorr, 256, &f0, &position) ){
	        if(f0 <= 360 && f0 > 150){
	        	//printf("%f\n",f0);
				int x = 100 + sample_spacing * (start + position) ;
				int y = (int) 690 - f0/2;
				setcolor(LIGHTGREEN);
				circle(x,y,3);
				}
			}
		
        delay(300);
        if(start + window_size/2 < num_samples ) drawLineInWaveForm(start,sample_spacing,WHITE);
	}
          
}

int main(int argc, char *argv[]) {

    int BUFSIZE = 85000;
    header_p meta;
    int width = WIDTH;
    int height = HEIGHT;
    initwindow(width, height);
	setcolor(LIGHTGRAY);
    setbkcolor(LIGHTGRAY);

    //Open and read file
	FILE* infile;
//	infile = fopen("xebesvexchef.wav", "rb");
//    if (infile == NULL) {
//        perror("Error opening file");
//        return 1;
//    }
    
	OPENFILENAME ofn;
    char szFile[260];
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "WAV Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        // M? t?p tin dã ch?n
        infile = fopen(ofn.lpstrFile, "rb");
        if (infile == NULL) {
        	perror("Error opening file");
        	exit(1);
        }
    }
    else {
        // Ngu?i dùng dã h?y h?p tho?i ch?n t?p tin
        printf("Nguoi dung da huy.\n");
        exit(1);
    }
    
    meta = (header_p)malloc(sizeof(header));
    fread(meta, 1, 36, infile); // Read the first 36 bytes

    // Read the rest of Subchunk 1, if it's larger than 16 bytes
    if (meta->subchunk1_size > 16) {
        fseek(infile, meta->subchunk1_size - 16, SEEK_CUR);
    }

    // Now read Subchunk 2 ID and size
    fread(meta->subchunk2_id, 1, 4, infile);
    fread(&meta->subchunk2_size, 1, 4, infile);
    
    // Read the header
	
	int SampleNumber = meta -> chunk_size/2;
	int Fs = meta -> sample_rate;
	float Ts = 1.0/(float)Fs;
    
	int16_t* inbuff16;
    inbuff16 = (int16_t*)malloc(2 * sizeof(int16_t) * BUFSIZE);
    fread(inbuff16, 2, SampleNumber, infile);

	// do dai thoi gian cua file
	float timeLength = (meta->subchunk2_size * 8.0)/(meta->sample_rate * meta->num_channels * meta->bits_per_sample);
    
    // Ve cac do thi
    
    drawInterface();
    
	drawWaveform(inbuff16, SampleNumber,29851, GREEN, 130);
	
	drawAndClearAutoCorrelation(inbuff16, SampleNumber, GREEN, 360 );

    getch();

    closegraph();
    free(inbuff16);
    fclose(infile);

    return 0;
}


