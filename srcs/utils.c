#include <stdlib.h>

double pow(double x, double y) {
    if (x == 0) {
        return 0.0;
    }
    if (y == 0) {
        return 1.0;
    }
    double result = 1.0;
    for (int i = 0; i < y; ++i) {
        result *= x;
    }
    return result;
}

double sqrt(double x) {
    double result = 1.0;
    for (int i = 0; i < 100; ++i) {
        result = (result + x / result) / 2;
    }
    return result;
}

double min(double data[], size_t size) {
    if (!data || size == 0)
        return 0.0;
    double min = data[0];
    for (int i = 1; i < size; ++i) {
        if (data[i] < min) {
            min = data[i];
        }
    }
    return min;
}

double max(double data[], size_t size) {
    if (!data || !size)
        return 0.0;
    double max = data[0];
    for (int i = 1; i < size; ++i) {
        if (data[i] > max) {
            max = data[i];
        }
    }
    return max;
}

double avg(double data[], size_t size) {
    if (!data || size == 0)
        return 0.0;
    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        if (data[i] == 0.0)
            return 0.0;
        sum += data[i];
    }
    return sum / size;
}

double stddev(double data[], size_t size) {
    if (!data)
        return 0.0;
    if (size == 0)
        return 0.0;
    if (size == 1)
        return data[0];

    double mean, SD = 0.0;
    mean = avg(data, size);
    for (int i = 0; i < size; ++i) {
        SD += pow(data[i] - mean, 2);
    }
    return sqrt(SD / size);
}