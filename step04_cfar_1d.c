#include <stdio.h>

#define NUM_BINS 32
#define REF_CELLS 2
#define GUARD_CELLS 1
#define ALPHA 3.0
#define MAX_TARGETS 5

typedef struct {
	int bin;
	float power;
	float threshold;
} Target;

int main(void) {
	float I[NUM_BINS];
	float Q[NUM_BINS];
	float power[NUM_BINS];
	float threshold[NUM_BINS] = { 0.0f };
	int detect[NUM_BINS] = { 0 };
	Target targets[MAX_TARGETS];

	int i, j;
	int targetCount = 0;

	// 1. 기본 배경 신호 생성
	for (i = 0; i < NUM_BINS; i++) {
		I[i] = 1.0f;
		Q[i] = 1.0f;
	}

	// 2. 표적 추가

	I[10] = 4.0f;
	Q[10] = 3.0f;

	I[15] = 5.0f;
	Q[15] = 4.0f;

	I[20] = 6.0f;
	Q[20] = 2.0f;

	I[26] = 3.5f;
	Q[26] = 3.0f;

	// 3. power 계산
	for (i = 0; i < NUM_BINS; i++) {
		power[i] = I[i] * I[i] + Q[i] * Q[i];
	}

	// 4. CA-CFAR threshold 계산
	for (i = REF_CELLS + GUARD_CELLS; i < NUM_BINS - (REF_CELLS + GUARD_CELLS); i++) {
		float sum = 0.0f;
		int count = 0;

	// 왼쪽 reference cells
		//for (j = i - GUARD_CELLS - REF_CELLS; j < i - GUARD_CELLS; j++) 
		  for(j = i + GUARD_CELLS +1; j <= i + GUARD_CELLS + REF_CELLS; j++)         {
			sum += power[j];
			count++;
		}

	// 오른쪽 reference cells
		for (j = i + GUARD_CELLS + 1; j < i - GUARD_CELLS; j++) {
			sum += power[j];
			count++;
		}

		threshold[i] = (sum / count) * ALPHA;
	}

	// 5. 탐지 결과 저장
	for (i = 0; i < NUM_BINS; i++) {
		if (threshold[i] > 0.0f && power[i] > threshold[i]) {
			detect[i] = 1;

			if (targetCount < MAX_TARGETS) {
				targets[targetCount].bin = i;
				targets[targetCount].power = power[i];
				targets[targetCount].threshold = threshold[i];
				targetCount++;
			}
		}
		else {
			detect[i] = 0;
		}
	}

	// 6. 전체 결과 출력
	printf("bin | power | threshold | detect\n");
	printf("--------------------------------\n");

	for (i = 0; i < NUM_BINS; i++) {
		printf("%2d | %5.2f | %8.2f | %d\n", i, power[i], threshold[i], detect[i]);
	}

	// 7. 탐지된 표적 리스트 출력
	printf("\n === Target List ===\n");
	printf("Detected Target Count = %d\n", targetCount);

	for (i = 0; i < targetCount; i++) {
		printf("Target %d -> bin: %d, power %.2f threshold : %.2f\n",
			i + 1,
			targets[i].bin,
			targets[i].power,
			targets[i].threshold);
	}

	return 0;
}