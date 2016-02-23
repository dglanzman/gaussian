#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// writes a 2D gaussian distribution to the provided buffer
// sigma is standard deviation in pixels, distribution is normalized
int gaussian(double ** buffer, double sigma) {
    int size = round(6 * sigma);
    if (size % 2 == 0) size++;
    *buffer = malloc(size * size * sizeof (double));
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            double x_r2 = x - size / 2;
            x_r2 = x_r2 * x_r2;
            double y_r2 = y - size / 2;
            y_r2 = y_r2 * y_r2;
            double denom = 2 * sigma * sigma;
            double norm = 1 / (2 * M_PI * sigma * sigma);
            (*buffer)[y * size + x] = norm * exp(-((x_r2 + y_r2) / denom));
        }
    }
    return size;
}

// clamps i to [0, ceil)
int clamp(int i, int ceil) {
    if (i >= ceil) return ceil - 1;
    if (i < 0) return 0;
    return i;
}

int main() {
    int w, h;
    double * kernel;
    double pixel[3] = {0};
    scanf("P6 %u %u %*u\n", &w, &h);
    printf("P6\n%u %u\n255\n", w, h);
    unsigned char * image = malloc(3 * w * h);
    unsigned char * out = malloc(3 * w * h);
    fread(image, 3, w * h, stdin);
    int k_size = gaussian(&kernel, 1.5);
    clock_t start = clock();
    for (int y = 0; y < h; y++)
    for (int x = 0; x < w; x++) {
        for (int ky = 0; ky < k_size; ky++) 
        for (int kx = 0; kx < k_size; kx++) {
            int ix = clamp(x + kx - k_size / 2, w);
            int iy = clamp(y + ky - k_size / 2, h);
            int k_id = ky * k_size + kx;
            int img_id = (iy * w + ix) * 3;
            for (int i = 0; i < 3; i++) {
                pixel[i] += kernel[k_id] * image[img_id + i];
            }
        }
        for (int i = 0; i < 3; i++) {
            out[(y * w + x) * 3 + i] = round(pixel[i]);
            pixel[i] = 0;
        }
    }
    clock_t end = clock();
    fwrite(out, 3, w * h, stdout);
    fprintf(stderr, "Time = %lums\n", (end - start) / 1000);
    free(image);
    free(out);
    free(kernel);
    return 0;
}
