#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>

#include "config.h"

#define CCR CheckCollisionRecs

typedef struct {
	Rectangle area;
	float velocity;
	int score;
} Bird;

struct Pipe {
	Rectangle areabtm;
	Rectangle areatop;
};

static struct Pipe p1;
static struct Pipe p2;

static char scoreText[14];

static const int P1_INIT_X = 650;
static const int P2_INIT_X = 1000;

static void updateBird(Bird *b, float delta);
static void renderBird(Bird *b);
static void killBird(Bird *b);
static void updateScoreText(char *t, Bird *b);
static void updatePipe(struct Pipe *p, float delta);
static void renderPipe(struct Pipe *p);
static void resetPipes(struct Pipe *p1, struct Pipe *p2);

int
main()
{
	srand(time(NULL));

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
	SetTargetFPS(TARGET_FPS);

	p1.areabtm.x = p1.areatop.x = P1_INIT_X;
	p1.areabtm.y = (rand() % 200) + 200;
	p1.areatop.y = p1.areabtm.y - 1150;
	p1.areabtm.width = p1.areatop.width = 100;
	p1.areabtm.height = p1.areatop.height = 1000;

	p2.areabtm.x = p2.areatop.x = P2_INIT_X;
	p2.areabtm.y = (rand() % 200) + 200;
	p2.areatop.y = p2.areabtm.y - 1150;
	p2.areabtm.width = p2.areatop.width = 100;
	p2.areabtm.height = p2.areatop.height = 1000;

	Bird player;
	player.area.width = player.area.height = 30;
	player.area.x = 20;
	player.area.y = (float)SCREEN_HEIGHT / 2;

	player.score = 0;

	float deltaTime = 0.f;

	updateScoreText(scoreText, &player);

	while (!WindowShouldClose()) {
		deltaTime = GetFrameTime();
		updateBird(&player, deltaTime);
		updatePipe(&p1, deltaTime);
		updatePipe(&p2, deltaTime);

		BeginDrawing();
			ClearBackground(BG_COLOR);
			DrawRectangleRec(player.area, RED);
			renderBird(&player);
			renderPipe(&p1);
			renderPipe(&p2);
			DrawText(scoreText, 1, 1, 32, WHITE);
		EndDrawing();
	}
	CloseWindow();
}

static void
updateBird(Bird *b, float delta)
{
	b->velocity += 25 * delta;

	if (IsKeyPressed(KEY_UP)) {
		b->velocity = -8.f;
	}

	if (floorf(b->area.x) == floorf(p1.areabtm.x) + 50 ||
			floorf(b->area.x) == floorf(p2.areabtm.x) + 50) {
		b->score++;
		updateScoreText(scoreText, b);
	}

	if (b->area.y > (SCREEN_HEIGHT - b->area.height) || b->area.y < 0) {
		killBird(b);
	}

	if (CCR(b->area, p1.areabtm) || CCR(b->area, p1.areatop) ||
			CCR(b->area, p2.areabtm) || CCR(b->area, p2.areatop)) {
		killBird(b);
	}

	b->area.y += b->velocity;
}

static void
renderBird(Bird *b)
{
	DrawRectangleRec(b->area, YELLOW);
}

static void
killBird(Bird *b)
{
	b->score = 0;
	updateScoreText(scoreText, b);
	b->velocity = 0.f;
	b->area.y = (float)SCREEN_HEIGHT / 2;

	resetPipes(&p1, &p2);
}

static void
updateScoreText(char *t, Bird *b)
{
	sprintf(t, "Score: %i", b->score);
}

static void
updatePipe(struct Pipe *p, float delta)
{
	p->areatop.x -= 250 * delta;
	p->areabtm.x = p->areatop.x;

	if (p->areabtm.x < -100) {
		p->areabtm.x = 650;
		p->areatop.x = p->areabtm.x;
		p->areabtm.y = (rand() % 200) + 200;
		p->areatop.y = p->areabtm.y - 1150;
	}
}

static void
renderPipe(struct Pipe *p)
{
	DrawRectangleRec(p->areabtm, GREEN);
	DrawRectangleRec(p->areatop, GREEN);
}

static void
resetPipes(struct Pipe *p1, struct Pipe *p2)
{
	p1->areabtm.x = p1->areatop.x = P1_INIT_X;
	p1->areabtm.y = (rand() % 200) + 200;
	p1->areatop.y = p1->areabtm.y - 1150;

	p2->areabtm.x = p2->areatop.x = P2_INIT_X;
	p2->areabtm.y = (rand() % 200) + 200;
	p2->areatop.y = p2->areabtm.y - 1150;
}
