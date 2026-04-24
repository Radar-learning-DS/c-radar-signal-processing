#include <stdio.h>
#include <math.h>

#define MAX_TARGETS 10
#define MAX_TRACKS 10
#define DIST_THRESHOLD 5.0f

typedef struct {
	int id;
	float range;
	float velocity;
	float power;
} Target;

typedef struct {
	int trackId;
	float range;
	float velocity;
	float power;
	int matched;
} Track;

int main(void) {
	Target prevTargets[2];
	Target currTargets[2];

	Track tracks[MAX_TRACKS];

	int prevCount = 2;
	int currCount = 2;
	int trackCount = 0; 

	int i, j;

	//==========================
	// 1. 이전 프레임 표적 리스트
	//==========================
	prevTargets[0].id = 1;
	prevTargets[0].range = 10.0f;
	prevTargets[0].velocity = 2.0f;
	prevTargets[0].power = 25.0f;

	prevTargets[1].id = 2;
	prevTargets[1].range = 20.0f;
	prevTargets[1].velocity = -4.0f;
	prevTargets[1].power = 29.0f;

	//==========================
	// 2. 현재 프레임 표적 리스트
	//==========================
	currTargets[0].id = 0; // 아직 미할당
	currTargets[0].range = 30.0f;
	currTargets[0].velocity = 8.0f;
	currTargets[0].power = 24.0f;

	currTargets[1].id = 0; // 아직 미할당
	currTargets[1].range = 19.0f;
	currTargets[1].velocity = -4.0f;
	currTargets[1].power = 30.0f;

	//==========================
    // 3. 이전 프레임 기반 track 초기화
    //==========================
	for (i = 0; i < prevCount; i++) {
		tracks[i].trackId = prevTargets[i].id;
		tracks[i].range = prevTargets[i].range;
		tracks[i].velocity = prevTargets[i].velocity;
		tracks[i].power = prevTargets[i].power;
		tracks[i].matched = 0;
	}
	trackCount = prevCount;

	//==========================
	// 4. 현재 표적과 기존 track 매칭
	//==========================
	for (i = 0; i < currCount; i++) {
		float bestCost = 1e9f;
		int bestTrack = -1;

		for (j = 0; j < trackCount; j++) {
			float dRange = currTargets[i].range - tracks[j].range;
			float dVelocity = currTargets[i].velocity - tracks[j].velocity;

			float cost = fabsf(dRange) + fabsf(dVelocity);

			if (cost < bestCost && cost < DIST_THRESHOLD) {
				bestCost = cost;
				bestTrack = j;
			}
		}

		if (bestTrack != -1) {
			//기존 track에 매칭
			currTargets[i].id = tracks[bestTrack].trackId;
			tracks[bestTrack].range = currTargets[i].range;
			tracks[bestTrack].velocity = currTargets[i].velocity;
			tracks[bestTrack].power = currTargets[i].power;
			tracks[bestTrack].matched = 1;
		}
		else {
			//새 표적이면 새 track 생성
			if (trackCount < MAX_TRACKS) {
				int newId = trackCount + 1;
				currTargets[i].id = newId;

				tracks[trackCount].trackId = newId;
				tracks[trackCount].range = currTargets[i].range;
				tracks[trackCount].velocity = currTargets[i].velocity;
				tracks[trackCount].power = currTargets[i].power;
				tracks[trackCount].matched = 1;

				trackCount++;
			}
		}

	}
	// =========================
	// 5. 결과 출력
	// =========================
	printf("=== Previous Frame Targets ===\n");
	for (i = 0; i < prevCount; i++) {
		printf("Prev Target %d -> ID: %d, Range: %.2f m, Velocity: %.2f m/s, Power: %.2f\n",
			i + 1,
			prevTargets[i].id,
			prevTargets[i].range,
			prevTargets[i].velocity,
			prevTargets[i].power);
	}

	printf("\n=== Current Frame Targets (After Tracking) ===\n");
	for (i = 0; i < currCount; i++) {
		printf("Curr Target %d -> ID : %d, Range: %.2f m, velocity: %.2f m/s, Power: %.2f\n",
			i+1,
			currTargets[i].id,
			currTargets[i].range,
			currTargets[i].velocity,
			currTargets[i].power);
	}

	printf("\n=== Track Table===\n");
	for (i = 0; i < trackCount; i++) {
		printf("Track %d -> Range: %.2f m, Velocity: %.2f m/s, Power: %.2f, Matched: %d\n"
			, tracks[i].trackId
			, tracks[i].range
			, tracks[i].velocity
			, tracks[i].power
			, tracks[i].matched);
	}

	return 0;
}