
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


// 인게임
void gameRound();

void lock();

void combi();

void roll();


// 유저 입력
void choice();

// 출력
void scoreboard();

void rules();

// 메인
int main();

//주사위관련
int dice[5] = {0};
int locking[5] = {0};
int numbers[6] = {0};
int remain = 0;

// 유저 입력
char says[100];

//판정 관련
int combi_score = 0;
int yahtzee_bonus = 0;
char *combiList[13] = {
        "One", "Two", "Three", "Four", "Five", "Six", "Three of Kinds", "Four of Kinds", "Full House", "Small Straight",
        "Large Straight", "Yahtzee", "Chance"
};
int isStop;

//점수 관련
int score[13] = {0};
int checking[13] = {0};
int total = 0;

//난수
int i;


//라운드가 시작할때 작동됩니다.
void gameRound() {
    int R = 0;
    while (R < 13) {
        R++;
        for (i = 0; i < 6; i++) {
            numbers[i] = 0;
        }
        for (i = 0; i < 5; i++) {
            locking[i] = 0;
        }
        remain = 3;

        printf("주사위를 굴리기 위해 아무 글자나 입력하세요\n");
        choice();

        roll();

        isStop = 0;
        lock();
    }

    // GAME END
    scoreboard();
    printf("최종 점수 : %d", total);
}

//주사위를 굴립니다.
void roll() {
    srand(time(NULL));
    for (i = 0; i < 5; i++) {
        if (locking[i] == 0) {
            dice[i] = rand() % 6 + 1;
        }
    }
    // 여기서 서버로 전송함

    remain = remain - 1;
}

//락을 시킬 때 발동됩니다.
void lock() {
    while (1) {
        scoreboard();

        printf("\n\n주사위는...\n");
        for (i = 0; i < 5; i++) {
            if (locking[i] == 1) {
                printf("[%d] ", dice[i]);
            } else {
                printf("%d ", dice[i]);
            }
        }

        printf("\n남은 Roll 수 : %d\n", remain);

        if (remain > 0 && isStop == 0) {
            printf("락을 시키려거나 해제하려면 주사위 자리번호를 쓰세요.\n");
            printf("주사위 자리번호는 왼쪽부터 1번입니다.\n");
            printf("주사위를 다시 굴리고 싶다면 \"roll\"을 치세요\n");
            printf("조합을 채우고 라운드를 끝내고 싶다면 \"stop\"을 쳐서 조합 선택 메뉴로 들어가세요.\n");
            choice();
            if (!strcmp(says, "1") || !strcmp(says, "2") || !strcmp(says, "3") || !strcmp(says, "4") || !
                    strcmp(says, "5")) {
                // lock을 변경함 1->0 0->1
                int temp = atoi(says) - 1;
                locking[temp] = locking[temp] == 1 ? 0 : 1;
            } else if (strcmp(says, "roll") == 0) {
                roll();
            } else if (strcmp(says, "stop") == 0) {
                isStop = 1;
            } else {
                printf("잘못 입력하셨습니다.\n");
            }
        } else {
            combi();
            return;
        }
    }
}


//조합에 따라 움직입니다.
void combi() {
    while (1) {
        printf("\n조합의 번호를 입력하세요.\n");
        printf("돌아가려면 back을 입력하세요\n");

        // 숫자 배열 초기화
        for (i = 0; i < 6; i++) {
            numbers[i] = 0;
        }
        // 주사위로 숫자 배열 입력
        for (i = 0; i < 5; i++) {
            numbers[dice[i] - 1]++;
        }
        combi_score = 0;

        if ((numbers[0] == 5 || numbers[1] == 5 || numbers[2] == 5 || numbers[3] == 5 || numbers[4] == 5 || numbers[5]
                                                                                                            ==
                                                                                                            5) && score[11]) {
            if (score[11] != 50) {
                // 첫 번째 얏찌 확인
                printf("\nYahtzee!\n");
            } else {
                // 두번째 얏찌 확인
                printf("\nback-to-back Yahtzee bonus +50\n");
                yahtzee_bonus = 50;
                combi_score += 50;
            }
        }

        int selected = -1;
        // 입력 받는 부분 반복임
        while (1) {
            choice();
            if (strcmp(says, "back") == 0) {
                return;
            }

            char *selectedList[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13"};
            for (i = 0; i < 13; i++) {
                if (strcmp(says, selectedList[i]) == 0) {
                    selected = i;
                    break;
                }
            }

            if (selected == -1) {
                printf("잘못 입력하셨거나 이미 채워진 조합입니다.\n");
            } else {
                break;
            }
        }

        // One ~ Six
        for (i = 0; i < 6; i++) {
            if (selected == i && checking[i] == 0) {
                combi_score = numbers[i] * (i + 1);
            }
        }

        // Three of kinds
        if (selected == 6 && checking[6] == 0) {
            if (numbers[0] >= 3 || numbers[1] >= 3 || numbers[2] >= 3 || numbers[3] >= 3 || numbers[4] >= 3 || numbers[
                                                                                                                       5] >=
                                                                                                               3) {
                for (i = 0; i < 6; i++) {
                    combi_score += numbers[i] * (i + 1);
                }
            }
        }

        // Four of kinds
        if (selected == 7 && checking[7] == 0) {
            if (numbers[0] >= 4 || numbers[1] >= 4 || numbers[2] >= 4 || numbers[3] >= 4 || numbers[4] >= 4 || numbers[
                                                                                                                       5] >=
                                                                                                               4) {
                for (i = 0; i < 6; i++) {
                    combi_score += numbers[i] * (i + 1);
                }
            }
        }

        // Full house
        if (selected == 8 && checking[8] == 0) {
            int dou = 0, tri = 0;
            for (i = 0; i < 6; i++) {
                if (numbers[i] == 2) {
                    dou++;
                }
                if (numbers[i] == 3) {
                    tri++;
                }
            }

            if (dou == 1 && tri == 1) {
                combi_score = 25;
            }
        }

        // Small straight
        if (selected == 9 && checking[9] == 0) {
            if ((numbers[0] >= 1 && numbers[1] >= 1 && numbers[2] >= 1 && numbers[3] >= 1) || (
                    numbers[1] >= 1 && numbers[2] >= 1 && numbers[3] >= 1 && numbers[4] >= 1) || (
                        numbers[2] >= 1 && numbers[3] >= 1 && numbers[4] >= 1 && numbers[5] >= 1)) {
                combi_score = 30;
            }
        }
        // Large straight
        if (selected == 10 && checking[10] == 0) {
            if ((numbers[0] == 1 && numbers[1] == 1 && numbers[2] == 1 && numbers[3] == 1 && numbers[4] == 1) || (
                    numbers[1] == 1 && numbers[2] == 1 && numbers[3] == 1 && numbers[4] == 1 && numbers[5] == 1)) {
                combi_score = 40;
            }
        }
        // Yahtzee
        if (selected == 11 && checking[11] == 0) {
            for (i = 0; i < 6; i++) {
                if (numbers[i] == 5) {
                    combi_score = 50;
                }
            }
        }

        // Change
        if (selected == 12 && checking[12] == 0) {
            for (i = 0; i < 6; i++) {
                combi_score += numbers[i] * (i + 1);
            }
        }

        while (1) {
            printf("%s 의 점수는 %d입니다. 최종 결정은 yes, 다시 돌아가려면 no\n", combiList[selected], combi_score);
            choice();

            if (strcmp(says, "yes") == 0) {
                score[selected] = combi_score;
                checking[selected] = 1;
                return;
            }
            if (strcmp(says, "no") != 0) {
                printf("잘못 입력했습니다.\n");
            }
        }
    }
}

// 점수판에서 입력 안했을 때 0이면 헷갈리니까 -로 변환
char *formatScore(int index) {
    if (checking[index] == 1) {
        // score 숫자 복제
        static char temp[100];
        sprintf(temp, "%d", score[index]);
        return temp;
    } else {
        return "-";
    }
}

//점수판입니다.
void scoreboard() {
    // 유저 1, 2 점수 나눠야함
    total = 0;
    int bonus = 0;
    for (i = 0; i < 13; i++) {
        total = total + score[i];
    }
    total = total + yahtzee_bonus;
    for (i = 1; i <= 6; i++) {
        bonus = bonus + score[i];
    }
    if (bonus >= 63) {
        total = total + 35;
    }

    printf("-Yahtzee--------------- user1 | user2 -\n");
    printf("[1] One                   %-3s | %3s\n", formatScore(0), formatScore(0));
    printf("[2] Two                   %-3s | %3s\n", formatScore(1), formatScore(1));
    printf("[3] Three                 %-3s | %3s\n", formatScore(2), formatScore(2));
    printf("[4] Four                  %-3s | %3s\n", formatScore(3), formatScore(3));
    printf("[5] Five                  %-3s | %3s\n", formatScore(4), formatScore(4));
    printf("[6] Six                   %-3s | %3s\n", formatScore(5), formatScore(5));
    printf("bonus 63 /                %-3d | %3d\n", bonus, bonus);
    printf("--------------------------------------\n");
    printf("[7] Three of Kinds        %-3s | %3s\n", formatScore(6), formatScore(6));
    printf("[8] Four of Kinds         %-3s | %3s\n", formatScore(7), formatScore(7));
    printf("[9] Full House            %-3s | %3s\n", formatScore(8), formatScore(8));
    printf("[10] Small Straight       %-3s | %3s\n", formatScore(9), formatScore(9));
    printf("[11] Large Straight       %-3s | %3s\n", formatScore(10), formatScore(10));
    printf("[12] Yahtzee              %-3s | %3s\n", formatScore(11), formatScore(11));
    printf("[13] Chance               %-3s | %3s\n", formatScore(12), formatScore(12));
    printf("--------------------------------------\n");
    printf("Yahtzee bonus             %-3d | %3d\n", yahtzee_bonus, yahtzee_bonus);
    printf("Total score               %-3d | %3d\n", total, total);
    printf("--------------------------------------\n");
}

//룰 설명입니다.
void rules() {
    printf("\n이 게임은 주사위 5개를 이용하여 조합을 만드는 게임입니다.\n");
    printf("처음엔 한꺼번에 5개를 굴란 다음, 원하는 주사위를 킵한 후\n");
    printf("나머지 주사위를 굴릴 수 있으며,\n");
    printf("한 라운드에 모두 합쳐 총 3회까지 굴릴 수 있습니다.\n");
    printf("주사위 3회 안에 무조건 조합을 하나 선택해 점수를 얻어야 합니다.\n");
    printf("한번 점수를 얻은 조합 종류로는 다시 점수를 획득할 수 없습니다.\n\n");
    printf("조합 종류를 보시려면 \"next\"를 써주세요.\n");
    printf("메인 메뉴로 돌아가시려면 아무 글자나 써주세요.\n");
    choice();
    if (strcmp(says, "next") == 0) {
        printf("\n\n");
        printf("------Yahtzee------\n");
        printf("One : 주사위에서 1이 나온 주사위 개수 x1을 점수로 합니다.\n");
        printf("Two : 주사위에서 2이 나온 주사위 개수 x2를 점수로 합니다.\n");
        printf("Three : 주사위에서 3이 나온 주사위 개수 x3을 점수로 합니다.\n");
        printf("Four : 주사위에서 4이 나온 주사위 개수 x4를 점수로 합니다.\n");
        printf("Five : 주사위에서 5이 나온 주사위 개수 x5를 점수로 합니다.\n");
        printf("Six : 주사위에서 6이 나온 주사위 개수 x6을 점수로 합니다.\n");
        printf("bonus : One ~ Six의 점수의 합이 63점 이상이라면 보너스 35점을 획득합니다\n");
        printf("-------------------\n");
        printf("Three of Kinds : 같은 주사위가 3개 이상 있다면 주사위 눈의 총합을 점수로 합니다\n");
        printf("Four of Kinds : 같은 주사위가 4개 이상 있다면 주사위 눈의 총합을 점수로 합니다\n");
        printf("Full House : 같은주사위 3개 + 같은 주사위 2개가 있다면 25점을 획득합니다\n");
        printf("ex : [3] [3] [3] [5] [5]\n");
        printf("Small Straight :  주사위 숫자중 4개 이상이 연속된 숫자로 \n");
        printf("되어있다면 30점을 획득합니다.\n");
        printf("ex : [1] [2] [3] [4] [6]\n");
        printf("Large Straight : 주사위 숫자 모두가 연속된 숫자로\n");
        printf("되어있다면 40점을 획득합니다.\n");
        printf("ex : [6] [5] [4] [3] [2]\n");
        printf("Yahtzee! : 주사위 숫자가 모두 같다면 50점을 획득합니다.\n");
        printf("이 조합을 50점을 채운 이후 또다시 Yahtzee인 상황이 또다시 온다면\n");
        printf("어떤 조합으로 채우든 50점의 추가점수를 획득합니다\n");
        printf("Chance : 무조건 주사의 눈의 총합을 점수로 획득합니다\n\n\n");
    }
}

// 유저 입력 받는거임
void choice() {
    memset(says, 0, 100);
    scanf("%s", &says);
}

//처음 시작할때 발동되는 것들 입니다.
int main() {
    while (1) {
        printf("Welcome to Yahtzee!\n");
        printf("룰을 듣고싶다면 \"rule\"을\n");
        printf("게임을 시작하려면 \"start\"를 써주세요.\n");

        choice();
        if (strcmp(says, "rule") == 0) {
            rules();
        } else if (strcmp(says, "start") == 0) {
            gameRound();
            break;
        } else {
            printf("잘못 입력했습니다.\n\n");
        }
    }
}