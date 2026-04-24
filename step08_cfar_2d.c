/*#include <stdio.h>

#define NUM_RANGE 16
#define NUM_DOPPLER 8
#define MAX_TARGETS 20

#define RANGE_RESOLUTION 1.5f
#define VELOCITY_RESOLUTION 2.0f

#define REF_RANGE 1
#define REF_DOPPLER 1
#define GUARD_RANGE 1
#define GUARD_DOPPLER 1
#define ALPHA 3.0f

typedef struct {
	int rangeBin;
	int dopplerBin;
	float range;
	float velocity;
	float power;
	float threshold;
} Target;

int main(void) {
	float I[NUM_RANGE][NUM_DOPPLER];
	float Q[NUM_RANGE][NUM_DOPPLER];
	float power[NUM_RANGE][NUM_DOPPLER];
	float threshold[NUM_RANGE][NUM_DOPPLER] = { 0.0f };

	int detect[NUM_RANGE][NUM_DOPPLER] = { 0 };
	int peakDetect[NUM_RANGE][NUM_DOPPLER] = { 0 };

	Target targets[MAX_TARGETS];
	int targetCount = 0;

	int r, d, rr, dd;

	// 1. 기본 배경 신호 생성
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			I[r][d] = 1.0f;
			Q[r][d] = 1.0f;
		}
	}

	// 2. 표적 삽입
	I[5][5] = 4.0f; Q[5][5] = 3.0f;
	I[5][4] = 2.0f; Q[5][4] = 1.5f;
	I[4][5] = 2.1f; Q[4][5] = 1.8f;
	I[6][5] = 2.3f; Q[6][5] = 1.6f;

	I[11][2] = 5.0f; Q[11][2] = 2.0f;
	I[10][2] = 2.5f; Q[10][2] = 1.8f;
	I[12][2] = 2.4f; Q[12][2] = 1.7f;
	I[11][3] = 2.0f; Q[11][3] = 1.6f;

	//3. power 계산
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			power[r][d] = I[r][d] * I[r][d] + Q[r][d] * Q[r][d];
		}
	}

	// 4. 2D CA-CFAR threshold 계산
	for (r = REF_RANGE + GUARD_RANGE; r < NUM_RANGE - (REF_RANGE + GUARD_RANGE); r++) {
		for (d = REF_DOPPLER + GUARD_DOPPLER; d < NUM_DOPPLER - (REF_DOPPLER + GUARD_DOPPLER); d++) {
			float sum = 0.0f;
			int count = 0;

			for (rr = r - (REF_RANGE + GUARD_RANGE); rr <= r + (REF_RANGE + GUARD_RANGE); rr++) {
				for (dd = d - (REF_DOPPLER + GUARD_DOPPLER); dd <= d + (REF_DOPPLER + GUARD_DOPPLER); dd++) {

					//GUARD 영역 + CUT 제외
					if (rr >= r - GUARD_RANGE && rr <= r + GUARD_RANGE &&
						dd >= d - GUARD_DOPPLER && dd <= d + GUARD_DOPPLER) {
						continue;
					}

					sum += power[rr][dd];
					count++;
				}
			}

			if (count > 0) {
				threshold[r][d] = (sum / count) * ALPHA;
			}
		}
	}

	//5. CFAR 탐지
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			if (threshold[r][d] > 0.0f && power[r][d] > threshold[r][d]) {
				detect[r][d] = 1;
			}
			else {
				detect[r][d] = 0;
			}
		}

	}

	//6. 2D peak picking
	for (r = 1; r < NUM_RANGE - 1; r++) {
		for (d = 1; d < NUM_DOPPLER - 1; d++) {
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

	//7. 최종 target list 저장
	for (r = 0; r < NUM_RANGE; r++) {
		for (d = 0; d < NUM_DOPPLER; d++) {
			if (peakDetect[r][d] == 1 && targetCount < MAX_TARGETS) {
				targets[targetCount].rangeBin = r;
				targets[targetCount].dopplerBin = d;
				targets[targetCount].range = r * RANGE_RESOLUTION;
				targets[targetCount].velocity =
					(d - (NUM_DOPPLER / 2)) * VELOCITY_RESOLUTION;
				targets[targetCount].power = power[r][d];
				targets[targetCount].threshold = threshold[r][d];
				targetCount++;
			}
		}
	}

	//8. 결과 출력
	printf("=== Final Target List===\n");
	printf("Final Target Count = %d\n", targetCount);

	for (r = 0; r < targetCount; r++) {
		printf("Target %d -> rangeBin: %d, dopplerBin: %d, range:%.2f m, velocity: %.2f m/s, power: %.2f, threshold: %.2f\n",
			r + 1,
		    targets[r].rangeBin,
			targets[r].dopplerBin,
			targets[r].range,
			targets[r].velocity,
			targets[r].power,
			targets[r].threshold);

	}

	return 0;
}*/