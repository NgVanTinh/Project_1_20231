#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <conio.h>
#include <graphics.h>
#include <math.h>
#define WIDTH 900
#define HEIGHT 630
#define WINDOW_SIZE 512
#define MAX_K 256

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

void drawWaveform(int16_t *data, int num_samples, int max_amplitude, int color, int startY) {
//	k/c giua cac mau
	
    float sample_spacing = (float)getmaxx() /  num_samples;

    for ( int i = 0; i < num_samples ; ++i) {
        int x1 = i * sample_spacing; 
        int y1 = startY - (data[i] * 100 / max_amplitude); 
        int x2 = (i + 1) * sample_spacing; 
        int y2 = startY - (data[i + 1] * 100 / max_amplitude); 

        setcolor(color);
        line(x1, y1, x2, y2);
    }
}

// reset cua so ve ham tu tuong quan
void resetDrawAutoCorrelation(int left, int top, int right, int bottom){
	// Ðat màu vi?n hình ch? nh?t (ví d?: tr?ng)
    setcolor(BLACK);

    // V? vi?n hình ch? nh?t
    rectangle(left, top, right, bottom);

    // Ð?t màu n?n và ki?u tô màu
    setfillstyle(SOLID_FILL, BLACK);

    // Tô màu n?n cho hình ch? nh?t
    // S? d?ng bar d? t?o hình ch? nh?t d?c màu
    bar(left + 1, top + 1, right, bottom);
}

// ve duong doc o wave form tro de vi tri am thanh dang phan tich ham R(k);
void drawLineInWaveForm(int start, int num_samples, int color){
	static bool isDrawn = false;
    static int *buffer = NULL;
	
	float sample_spacing = (float)getmaxx() /  num_samples;
    int x1 = start * sample_spacing; 
    int y1 = 0;
    int x2 = (start + 1) * sample_spacing; 
    int y2 = 210; 
    if (!isDrawn) {
        // L?n d?u tiên hàm du?c g?i, luu và v? hình ch? nh?t
        int size = imagesize(x1, y1, x2, y2);
        buffer = (int*) malloc(size);
        getimage(x1, y1, x2, y2, buffer);
        setcolor(color);
        rectangle(x1, y1, x2, y2);
        isDrawn = true;
    } else {
        // Khi hàm du?c g?i l?n th? hai, xóa hình ch? nh?t
        putimage(x1, y1, buffer, COPY_PUT);
        free(buffer);
        buffer = NULL;
        isDrawn = false;
    }

}

// truyen vao du lieu, vi tri mau bat dau cua mau, so mau, mang chua gia tri ham R(k), mau, toa do y bat dau ve(giong nhau cua ca 2 do thi) con toa do x mac dinh la 0
void calculateAndDrawAutoCorrelationAndWWaveForm(int16_t *data, int start, int window_size, float *autocorr, int color, int startY) {
	
	// ve do thi song
	float sample_spacing = (float)450.0 /  window_size;

    for ( int i = 0; i < window_size ; ++i) {
        int x1 = i * sample_spacing; 
        int y1 = startY - (data[i + start] * 100 / 32767); 
        int x2 = (i + 1) * sample_spacing; 
        int y2 = startY - (data[i + start + 1] * 100 / 32767); 

        setcolor(color);
        line(x1, y1, x2, y2);
    }
    
    
    // ve do thi ham tu tuong quan
  
    memset(autocorr, 0, sizeof(float) * window_size);

    // Tính ham tu tuong quan 
    for (int delay = 0; delay < window_size; ++delay) {
        for (int i = start; i < start + window_size - delay; ++i) {
            autocorr[delay] += data[i] * data[i + delay];
        }
        autocorr[delay] /= (window_size - delay); // Chia trung bình
    }

    // Tìm giá tri max
	
	float max_val = autocorr[0]; 
	for (int i = 1; i < window_size; ++i) {
	    if (autocorr[i] > max_val) {
	        max_val = autocorr[i];
	    }
	}


    // V? d? th? t? tuong quan
    for (int i = 0; i < window_size - 1; ++i) {
        int y1 = startY - (autocorr[i] / max_val) * 100; 
        int y2 = startY - (autocorr[i+1] / max_val) * 100; 
        int x1 = 500 + i * sample_spacing;
        int x2 = 500 + (i + 1) * sample_spacing;

        setcolor(color);
        line(x1, y1, x2, y2);
    }
    
    // ve hinh chu nhat tro den vi tri 512 mau am thanh dang xet ham tu tuong quan
    

}

// truyen vao du lieu, so mau cua du lieu, mau , vi tri bat dau ve
void drawAndClearAutoCorrelation(int16_t *data, int num_samples, int color, int startY) {
    int window_size = WINDOW_SIZE;
    float autocorr[window_size];
    int start;

    for (start = 0; start <= num_samples - window_size; start += window_size) {
    	int i = 0;
		resetDrawAutoCorrelation(0,200,1000,440);
        calculateAndDrawAutoCorrelationAndWWaveForm(data, start, window_size, autocorr, color, startY );
        delay(300); 
    }
}

int main() {
    FILE *infile;
    int BUFSIZE = 85000;
    header_p meta;

    infile = fopen("xebesvexchef.wav", "rb");
    if (infile == NULL) {
        perror("Error opening file");
        return 1;
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
    
    
//    int gd = DETECT, gm;
//    initgraph(&gd, &gm, NULL);
    int width = WIDTH;
    int height = HEIGHT;
    initwindow(width, height);
	drawWaveform(inbuff16, SampleNumber,32767, GREEN, getmaxy() / 6);
	
	for (int start = 0; start < SampleNumber - WINDOW_SIZE; start += WINDOW_SIZE/2) {
	    drawAndClearAutoCorrelation(inbuff16, SampleNumber, GREEN, getmaxy() / 2 );
	}

    getch();

    closegraph();
    free(inbuff16);
    fclose(infile);

    return 0;
}


