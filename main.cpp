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
#define PEAK_THRESHOLD 100000  // nguong dinh
#define MIN_PEAK_LAG 40

// cau truc header file .WAV
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

// ve hinh hcn dac voi mau duoc quy dinh 
void drawFillRectangle(int left, int top, int right, int bottom, int color){
	
    setcolor(color);

    rectangle(left, top, right, bottom);

     // Ðat mau nen va kieu to mau
    setfillstyle(SOLID_FILL, color);

     // to mau cho nen hcn
    // tao hcn dac mau
    bar(left + 1, top + 1, right, bottom);
}

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

// ve giao dien
void drawInterface(){

	// to mau xam ca hop thoai winbgi
	drawFillRectangle(0,0,2000,2000,LIGHTGRAY);
	
	// ve cac hcn mau den hien thi cac cua so ve do thi
	drawFillRectangle(100,30,1000,230,BLACK);
    drawFillRectangle(100,260,520,460,BLACK);
    drawFillRectangle(580,260,1000,460,BLACK);
    drawFillRectangle(100,490,1000,690,BLACK);
    
	// ve cac truc toa do xy cua cua so ve dang song	
	drawDashLineX(100,80,1000,80,4,4);
	drawDashLineX(100,130,1000,130,4,4);
	drawDashLineX(100,180,1000,180,4,4);
	
	int x = 100;
	for(int i = 1; i <= 8; i++){
		x += 100;
		drawDashLineY(x,30,x,230,4,4);		
	}
	
	// ve cac truc xy cua cua so ve tan so
	drawDashLineX(100,540,1000,540,4,4);
	drawDashLineX(100,590,1000,590,4,4);
	drawDashLineX(100,640,1000,640,4,4);
	
	int x3 = 100;
	for(int i = 1; i <= 8; i++){
		x3 += 100;
		drawDashLineY(x3,490,x3,690,4,4);		
	}
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
		outtextxy(x1 - 15, 232, charArr);		
	}
	
	number = 0;
	int x2 = 100;
	for(int i = 1; i <= 8; i++){
		x2 += 100;
		number += 483;
		char charArr[10];
		sprintf(charArr, "%.1f", number);
		outtextxy(x2 - 15, 692, charArr);		
	}
	strcpy(string,"5313.0 ms");
	outtextxy(985, 692, string);
}

// ham ve do thi dang song cua file am thanh
void drawWaveform(int16_t *data, int num_samples, int max_amplitude, int color, int startY) {

    float sample_spacing = (float)900 /  num_samples; //	k/c giua cac mau

    for ( int i = 0; i < num_samples ; ++i) {
        int x1 = 101 + i * sample_spacing; 
        int y1 = startY - (data[i] * 100 / max_amplitude); 
        int x2 = 101 + (i + 1) * sample_spacing; 
        int y2 = startY - (data[i + 1] * 100 / max_amplitude); 

        setcolor(color);
        line(x1, y1, x2, y2);
    }
}

// reset cua so ve ham tu tuong quan
void resetDrewWindow(int x1, int y1, int x2, int y2){
	
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

// ve duong doc o wave form tro de vi tri am thanh dang phan tich ham R(k);
void drawLineInWaveForm(int start, float sample_spacing, int color){
	static bool is_drawn = false;
    static int *buffer = NULL;
	
    int x1 = 101 + start * sample_spacing; 
    int y1 = 31;
    int x2 = 101 + (start + WINDOW_SIZE) * sample_spacing; 
    int y2 = 230; 

    if (!is_drawn) {
        int size = imagesize(x1, y1, x2, y2);
        buffer = (int*) malloc(size);
        getimage(x1, y1, x2, y2, buffer);
        setcolor(color);
        rectangle(x1, y1, x2, y2);
        is_drawn = true;
    } else {
        // Khi hàm goi lan thu 2 thi se xoa hinh chu nhat
        putimage(x1, y1, buffer, COPY_PUT);
        free(buffer);
        buffer = NULL;
        is_drawn = false;
    }

}

// ham tinh toan va ve do thi ham tu tuong quan va do thi dang song cua 512 mau 
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
        //autocorr[delay] /= (window_size - delay); // Chia trung bình de chuan hoa gia tri ham tuong quan
    }

    // Tìm giá tri max cua ham tu tuong quang trong doan 512 mau dang xet de 
    // xac dinh bien do lon nhat cua do thi ham tu tuong quan 
	
	
	float max_am = fabs(autocorr[0]);
	
	float max_autocorr = 0, min_autocorr = autocorr[0];
	for (int i = 1; i < window_size - 1; ++i) {
	    if (autocorr[i] > max_autocorr) {
	        max_autocorr = autocorr[i];
	    }
	    if(autocorr[i] < min_autocorr) {
	    	min_autocorr = autocorr[i];
		}
	}
		
	float sample_spacing_corr = (float)420.0 /  MAX_LAG;
    // ve do thi ham tu tuong quan
    for (int i = 0; i < MAX_LAG - 1; ++i) {
        int y1 = startY -( (autocorr[i] / max_am) * 95 ); 
        int y2 = startY -( (autocorr[i+1] / max_am) * 95 ); 
        int x1 = 581 + i * sample_spacing_corr;
        int x2 = 581 + (i + 1) * sample_spacing_corr;

        setcolor(color);
        line(x1, y1, x2, y2);
    }
    
    setbkcolor(BLACK);
    char buffer[50] = {}, rightBuffer[15] = {};
	strcat(buffer, "Max=");
	sprintf(rightBuffer, "%.1f", max_autocorr);
	strcat(buffer, rightBuffer);
	strcat(buffer, " Min=");
	sprintf(rightBuffer, "%.1f", min_autocorr);
	strcat(buffer, rightBuffer);
	outtextxy(610, 262, buffer);
	
}

 //Ham kiem tra tinh tuan hoan va tinh f0
bool findPeriodicPeak(float autocorr[], int size, double *f0,int *position) {
    int max_peak_lag = 0;
    double max_peak_value = 0.0;

    // Tim dinh co gia tri tuong quan lon nhat 
    // do giong nu co tan so 150-400Hz nen duyet den mau thu 115
    for (int i = 10; i < 115 ; i++) {
        if (autocorr[i] > max_peak_value 
		//&& autocorr[i] > autocorr[i-1] && autocorr[i] > autocorr[i+1]
		) {
            max_peak_lag = i;
            max_peak_value = autocorr[i];
        }
    }

    if (max_peak_value > PEAK_THRESHOLD && max_peak_lag > MIN_PEAK_LAG ) { //PEAK_THRESHOLD la nguong ma neu nho hon no thi khong coi no la mot dinh
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

// ham ve do thi ham tu tuong quan cua file am thanh bang canh duyet tung 512 mau va ve,
// dong thoi cung tinh toan ra gia tri F0 va thuc hien ve
void drawAndClearAutoCorrelation(int16_t *data, int num_samples, int color, int startY) {
    int window_size = WINDOW_SIZE;
    float autocorr[window_size];
    int start;
	float sample_spacing = (float)900 /  num_samples;
	
    for (start = 0; start <= num_samples - window_size/2; start += window_size/2) {
		resetDrewWindow(100,260,520,460); // xoa hinh da ve o 512 mau truoc do cua do thi song
		resetDrewWindow(580,260,1000,460); // xoa hinh da ve o 512 mau truoc do cua do thi ham tu tuong quan
		drawLineInWaveForm(start,sample_spacing,WHITE);
        calculateAndDrawAutoCorrelationAndWWaveForm(data, start, window_size, autocorr, color, startY); 
        
        // ve do thi tan so
        int position = 0;
        double f0 = 0.0;
        if (findPeriodicPeak(autocorr, 256, &f0, &position) ){
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
    
	//	/ Khai bao cau truc OPENFILENAME de su dung cho hop thoai chon file
	OPENFILENAME ofn;
	// Mang ky tu de luu duong dan cua file duoc chon
	char szFile[260];
	// Khoi tao gia tri ban dau cho cau truc OPENFILENAME la 0
	ZeroMemory(&ofn, sizeof(ofn));
	// Thiet lap kich thuoc cua cau truc OPENFILENAME
	ofn.lStructSize = sizeof(ofn);
	// Handle cua cua so cha, NULL neu khong su dung cua so cha
	ofn.hwndOwner = NULL;
	// Gan mang ky tu szFile vao truong lpstrFile de luu duong dan file duoc chon
	ofn.lpstrFile = szFile;
	// Dat ky tu dau tien cua lpstrFile la '\0' de khoi tao chuoi rong
	ofn.lpstrFile[0] = '\0';
	// Thiet lap kich thuoc toi da cua duong dan file
	ofn.nMaxFile = sizeof(szFile);
	// Thiet lap bo loc file trong hop thoai (filter), o day la tat ca file va file .wav
	ofn.lpstrFilter = "WAV Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0";
	// Thiet lap chi so filter mac dinh khi hop thoai mo len
	ofn.nFilterIndex = 1;
	// Khong su dung, nen dat la NULL
	ofn.lpstrFileTitle = NULL;
	// Khong su dung, nen dat la 0
	ofn.nMaxFileTitle = 0;
	// Thiet lap thu muc ban dau khi hop thoai mo len la NULL, tuc la thu muc hien tai
	ofn.lpstrInitialDir = NULL;
	// Cac co dieu khien cho hop thoai: yeu cau duong dan phai ton tai va file phai ton tai
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	// Goi ham GetOpenFileName de hien thi hop thoai chon file

    if (GetOpenFileName(&ofn) == TRUE) {
        // Mo tep tin da chon
        infile = fopen(ofn.lpstrFile, "rb");
        if (infile == NULL) {
        	perror("Error opening file!");
        	exit(1);
        }
    }
    else {
        printf("Cancelled!\n");
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
    
    // Ve cac do thi o cac cua so
    
    drawInterface();
    
	drawWaveform(inbuff16, SampleNumber,29851, GREEN, 130);
	
	drawAndClearAutoCorrelation(inbuff16, SampleNumber, GREEN, 360 );

    getch();

    closegraph();
    free(inbuff16);
    fclose(infile);

    return 0;
}


