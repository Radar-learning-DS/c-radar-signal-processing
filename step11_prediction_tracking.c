#include <stdio.h>
#include <math.h>

#define MAX_TARGETS 10

typedef struct {
	int id;

	float range;
	float velocity;

	float predictedRange;

	int active;
} Track;

typedef struct {
	float range;
	float velocity;
} Detection;

int main() {

	Detection prevDetections[2] = {
		{100.0f, 5.0f},
		{200.0f, -3.0f}
	};

	Detection currDetections[2] = {
		{105.0f, 5.0f},
		{197.0f, -3.0f}
	};

	Track tracks[MAX_TARGETS];

	int trackCount = 2;

	float dt = 1.0f;

	int i, j;

	//Initialize tracks
	for (i = 0; i < trackCount; i++) {

		tracks[i].id = i + 1;

		tracks[i].range = prevDetections[i].range;
		tracks[i].velocity = prevDetections[i].velocity;

		tracks[i].active = 1;
	}

	printf("=== Prediction Tracking ===\n\n");

	// Prediction step
	for (i = 0; i < trackCount; i++) {

		tracks[i].predictedRange =
			tracks[i].range +
			tracks[i].velocity * dt;

		printf("Track %d Prediction = %.2f m\n",
			tracks[i].id,
			tracks[i].predictedRange);
	}

	printf("\n");

	//Association step
	for (i = 0; i < 2; i++) {

		float bestCost = 9999.0f;
		int bestTrack = -1;

		for (j = 0; j < trackCount; j++) {

			float cost =
				fabs(currDetections[i].range -
					tracks[j].predictedRange);

			if (cost < bestCost) {

				bestCost = cost;
				bestTrack = j;
			}
		}

		if (bestTrack != -1) {

			printf("Detection %.2f m matched with Track %d\n",
				currDetections[i].range,
				tracks[bestTrack].id);

			// Update track
			tracks[bestTrack].range =
				currDetections[i].range;

			tracks[bestTrack].velocity =
				currDetections[i].velocity;
		}
	}

	return 0;
}