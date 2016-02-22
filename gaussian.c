#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

int print_kernel() {
    double * kernel;
    int size = gaussian(&kernel, 5);
    printf("P6 %u %u %u\n", size, size, 255);
    for (int i = 0; i < size * size; i++) {
        putchar(round(255 * kernel[i]));
        putchar(round(255 * kernel[i]));
        putchar(round(255 * kernel[i]));
    }
    return 0;
}

int clamp(int i, int ceil) {
    if (i >= ceil) return ceil - 1;
    if (i < 0) return 0;
    return i;
}

struct pixel_data {
    int x;
    int y;
    int x_max;
    int y_max;
    char * image;
    double * kernel;
    int k_size;
    char * out;
};

void pixel(struct pixel_data * p) {
    double pixel[3] = {0, 0, 0};
    for (int ky = 0; ky < p->k_size; ky++) {
        for (int kx = 0; kx < p->k_size; kx++) {
            int ix = clamp(kx - p->x, p->x_max);
            int iy = clamp(ky - p->y, p->y_max);
            int k_id = ky * p->k_size + kx;
            int img_id = (iy * p->x_max + ix) * 3;
            for (int i = 0; i < 3; i++) {
                pixel[i] += p->kernel[k_id] * p->image[img_id + i];
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        p->out[(p->y * p->x_max + p->x) * 3 + i] = round(pixel[i]);
    }
}

int main() {
    struct pixel_data p;
    scanf("P6 %u %u %*u\n", &(p.x_max), &(p.y_max));
    printf("P6\n%u %u\n255\n", p.x_max, p.y_max);
    p.image = malloc(3 * p.x_max * p.y_max);
    p.out = malloc(3 * p.x_max * p.y_max);
    fread(p.image, 3, p.x_max * p.y_max, stdin);
    p.k_size = gaussian(&(p.kernel), 3);
    for (p.y = 0; p.y < p.y_max; p.y++) {
        for (p.x = 0; p.x < p.x_max; p.x++) {
            pixel(&p);
        }
    }
    fwrite(p.out, 3, p.x_max * p.y_max, stdout);
    return 0;
}
