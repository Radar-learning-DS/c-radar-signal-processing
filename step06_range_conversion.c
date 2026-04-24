/*#include <stdio.h>

#define NUM_BINS 32
#define REF_CELLS 2
#define GUARD_CELLS 1
#define ALPHA 3.0f
#define MAX_TARGETS 10
#define RANGE_RESOLUTION 1.5f

typedef struct {
    int bin;
    float range;
    float power;
    float threshold;
} Target;

int main(void) {
    float I[NUM_BINS];
    float Q[NUM_BINS];
    float power[NUM_BINS];
    float threshold[NUM_BINS] = { 0.0f };

    int detect[NUM_BINS] = { 0 };
    int peakDetect[NUM_BINS] = { 0 };

    Target targets[MAX_TARGETS];

    int i, j;
    int targetCount = 0;

    // 1. 기본 배경 신호 생성
    for (i = 0; i < NUM_BINS; i++) {
        I[i] = 1.0f;
        Q[i] = 1.0f;
    }

    // 2. 표적/주변 bin 생성
    I[9] = 2.5f;  Q[9] = 2.0f;
    I[10] = 4.0f;  Q[10] = 3.0f;
    I[11] = 2.8f;  Q[11] = 2.2f;

    I[19] = 3.0f;  Q[19] = 2.0f;
    I[20] = 6.0f;  Q[20] = 2.0f;
    I[21] = 3.2f;  Q[21] = 2.5f;

    I[26] = 3.5f;  Q[26] = 3.0f;

    // 3. power 계산
    for (i = 0; i < NUM_BINS; i++) {
        power[i] = I[i] * I[i] + Q[i] * Q[i];
    }

    // 4. CA-CFAR threshold 계산
    for (i = REF_CELLS + GUARD_CELLS; i < NUM_BINS - (REF_CELLS + GUARD_CELLS); i++) {
        float sum = 0.0f;
        int count = 0;

        for (j = i - GUARD_CELLS - REF_CELLS; j < i - GUARD_CELLS; j++) {
            sum += power[j];
            count++;
        }

        for (j = i + GUARD_CELLS + 1; j <= i + GUARD_CELLS + REF_CELLS; j++) {
            sum += power[j];
            count++;
        }

        threshold[i] = (sum / count) * ALPHA;
    }

    // 5. 1차 탐지 결과 저장
    for (i = 0; i < NUM_BINS; i++) {
        if (threshold[i] > 0.0f && power[i] > threshold[i]) {
            detect[i] = 1;
        }
        else {
            detect[i] = 0;
        }
    }

    // 6. Peak picking
    for (i = 1; i < NUM_BINS - 1; i++) {
        if (detect[i] == 1 &&
            power[i] > power[i - 1] &&
            power[i] > power[i + 1]) {
            peakDetect[i] = 1;
        }
        else {
            peakDetect[i] = 0;
        }
    }

    // 7. 최종 target list 저장 (거리 변환 포함)
    for (i = 0; i < NUM_BINS; i++) {
        if (peakDetect[i] == 1) {
            if (targetCount < MAX_TARGETS) {
                targets[targetCount].bin = i;
                targets[targetCount].range = i * RANGE_RESOLUTION;
                targets[targetCount].power = power[i];
                targets[targetCount].threshold = threshold[i];
                targetCount++;
            }
        }
    }

    // 8. 전체 결과 출력
    printf("bin | power | threshold | detect | peak\n");
    printf("----------------------------------------\n");

    for (i = 0; i < NUM_BINS; i++) {
        printf("%2d  | %6.2f | %8.2f |   %d    |  %d\n",
            i, power[i], threshold[i], detect[i], peakDetect[i]);
    }

    // 9. 최종 표적 리스트 출력
    printf("\n=== Final Target List ===\n");
    printf("Final Target Count = %d\n", targetCount);

    for (i = 0; i < targetCount; i++) {
        printf("Target %d -> bin: %d, range: %.2f m, power: %.2f, threshold: %.2f\n",
            i + 1,
            targets[i].bin,
            targets[i].range,
            targets[i].power,
            targets[i].threshold);
    }

    return 0;
}*/