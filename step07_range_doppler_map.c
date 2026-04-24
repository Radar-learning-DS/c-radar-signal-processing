#include <stdio.h>

#define NUM_RANGE 16
#define NUM_DOPPLER 8
#define MAX_TARGETS 20

#define RANGE_RESOLUTION 1.5f
#define VELOCITY_RESOLUTION 2.0f
#define THRESHOLD 15.0f

typedef struct {
	int rangeBin;
	int dopplerBin;
	float range;
	float velocity;
	float power;
} Target;

int main(void) {
	float I[NUM_RANGE][NUM_DOPPLER];
	float Q[NUM_RANGE][NUM_DOPPLER];
	float power[NUM_RANGE][NUM_DOPPLER];

	int detect[NUM_RANGE][NUM_DOPPLER] = { 0 };
	int peakDetect[NUM_RANGE][NUM_DOPPLER] = { 0 };

	Target targets[MAX_TARGETS];
	int targetCount = 0;

	int r, d;

	// 1. 기본 배경 신호 생성
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			I[r][d] = 1.0f;
			Q[r][d] = 1.0f;
		}
	}

	// 2. 표적 삽입
	// 표적 1. range bin 5, doppler bin 6
	I[5][6] = 4.0f;
	Q[5][6] = 3.0f;

	// 주변 값 조금 추가 
	I[5][5] = 2.0f; Q[5][5] = 1.5f;
	I[5][7] = 2.2f; Q[5][7] = 1.7f;
	I[4][6] = 2.1f; Q[4][6] = 1.8f;
	I[6][6] = 2.3f; Q[6][6] = 1.6f;

	// 표적 2 : range bin 11, doppler bin 2
	I[11][2] = 5.0f;
	Q[11][2] = 2.0f;

	// 주변 값 조금 추가
	I[10][2] = 2.5f; Q[10][2] = 1.8f;
	I[12][2] = 2.4f; Q[12][2] = 1.7f;
	I[11][1] = 2.1f; Q[11][1] = 1.5f;
	I[11][3] = 2.0f; Q[11][3] = 1.6f;

	//3. power 계산
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			power[r][d] = I[r][d] * I[r][d] + Q[r][d] * Q[r][d];
		}
	}

	//4. 고정 threshold 기반 탐지
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			if (power[r][d] > THRESHOLD) {
				detect[r][d] = 1;
			}
			else {
				detect[r][d] = 0;
			}
		}
	}

	//5. 2D peak picking
	//가장자리 제외하고 상하좌우와 비교
	for (r = 1; r < NUM_RANGE-1; r++) {
		for (d = 1; d < NUM_DOPPLER-1; d++) {
			if (detect[r][d] == 1 &&
				power[r][d] > power[r - 1][d] &&
				power[r][d] > power[r + 1][d] &&
				power[r][d] > power[r][d - 1] &&
				power[r][d] > power[r][d + 1]) {
				peakDetect[r][d] = 1;
			}
			else {
				peakDetect[r][d] = 0;
			}
		}
	}

//6. 최종 target list 저장
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			if (peakDetect[r][d] == 1) {
				if (targetCount < MAX_TARGETS) {
					targets[targetCount].rangeBin = r;
					targets[targetCount].dopplerBin = d;
					targets[targetCount].range = r * RANGE_RESOLUTION;
					targets[targetCount].velocity =
						(d - (NUM_DOPPLER / 2)) * VELOCITY_RESOLUTION;
					targets[targetCount].power = power[r][d];
					targetCount++;
				}
			}
		}
	}

	//7. power 맵 출력
	printf("=== Power map===\n");
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			printf("%5.1f", power[r][d]);
		}
		printf("\n");
	}

	//8. detect map 출력
	printf("\n=== Detect Map===\n");
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			printf("%d", detect[r][d]);
		}
		printf("\n");
	}

	//9. peak map 출력
	printf("\n=== Peak Map ===\n");
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			printf("%d ", peakDetect[r][d]);
		}
		printf("\n");
	}

	//10. 최종 표적 리스트 출력
	printf("\n=== Final Target List ===\n");
	printf("Final Target Count = %d\n", targetCount);

	for (r = 0; r < targetCount; r++) {
		printf("Target %d -> rangeBin: %d, dopplerbin: %d, range : %.2f m, velocity: %.2f m/s, power: %.2f\n"
			, r + 1
			, targets[r].rangeBin
			, targets[r].dopplerBin
			, targets[r].range
			, targets[r].velocity
			, targets[r].power);
	}

	return 0;
}