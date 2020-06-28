#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MIN_DICE_NUM        1
#define MAX_DICE_NUM        6
#define MAX_TOKEN           4
#define MAX_BOARD_COUNT     52
#define MAX_TOKEN_TRAVELLED 57
#define TWO_PLAYER_GAME    0x01
#define THREE_PLAYER_GAME  0x02
#define FOUR_PLAYER_GAME   0x04
#define TURN_OF_PLAYER_A   0x01
#define TURN_OF_PLAYER_B   0x02
#define TURN_OF_PLAYER_C   0x04
#define TURN_OF_PLAYER_D   0x08

struct CurrentPlayer
{
    unsigned char player;
    struct CurrentPlayer *next;
};

typedef struct 
{
    int playerA[MAX_TOKEN];
    int playerB[MAX_TOKEN];
}TwoPlayerGame;

typedef struct 
{
    int playerA[MAX_TOKEN];
    int playerB[MAX_TOKEN];
    int playerC[MAX_TOKEN];
}ThreePlayerGame;

typedef struct 
{
    int playerA[MAX_TOKEN];
    int playerB[MAX_TOKEN];
    int playerC[MAX_TOKEN];
    int playerD[MAX_TOKEN];
}FourPlayerGame;
typedef struct
{
    unsigned char choice;
#if 0
    int playerA[MAX_TOKEN];
    int playerB[MAX_TOKEN];
    int playerC[MAX_TOKEN];
    int playerD[MAX_TOKEN];
#else
    TwoPlayerGame    TwoPlayers[MAX_BOARD_COUNT];
    ThreePlayerGame  ThreePlayers[MAX_BOARD_COUNT];
    FourPlayerGame   FourPlayers[MAX_BOARD_COUNT];
#endif
}BoardStatus;

typedef struct
{
#define TOKEN_ONE_STARTED     0x01
#define TOKEN_TWO_STARTED     0x02
#define TOKEN_THREE_STARTED   0x04
#define TOKEN_FOUR_STARTED    0x08
#define TOKEN_ONE_FINSHED     0x10
#define TOKEN_TWO_FINSHED     0x20
#define TOKEN_THREE_FINSHED  0x40
#define TOKEN_FOUR_FINSHED    0x80

    unsigned char choice;

    unsigned char    TokenStatusPlayerA;
    unsigned char    TokenStatusPlayerB;
    unsigned char    TokenStatusPlayerC;
    unsigned char    TokenStatusPlayerD;

#if 0
    int playerA[MAX_TOKEN];
    int playerB[MAX_TOKEN];
    int playerC[MAX_TOKEN];
    int playerD[MAX_TOKEN];
#else
    TwoPlayerGame    TwoPlayers[MAX_TOKEN_TRAVELLED];
    ThreePlayerGame  ThreePlayers[MAX_TOKEN_TRAVELLED];
    FourPlayerGame   FourPlayers[MAX_TOKEN_TRAVELLED];
#endif
}PlayerStatus;

int Rollthedice(char Player)
{
    int  random_number        = 0;
    int  total                = 0;

    do
    {
        fprintf(stdout,"Please roll the dice for user %c ",Player);

        getchar();

        srand(time(0));

        random_number = (rand()% (MAX_DICE_NUM)) + MIN_DICE_NUM;

        fprintf(stdout,"Rolled value for user %c is %d\n",Player,random_number);

        total += random_number;
    }while(6 == random_number);

    return total;
}

void LoadPlayers(struct CurrentPlayer **ptr,unsigned char player)
{
    struct CurrentPlayer *temp    = NULL;
    struct CurrentPlayer *current = NULL;

    current = *ptr;

    temp = (struct CurrentPlayer*)malloc((sizeof(struct CurrentPlayer)));
    if (NULL == temp)
    {
        fprintf(stderr,"Memory Allocation Failed ...!!!\n");
        exit(1);
    }

    temp->player = player;
    temp->next   = NULL;

    if(NULL == *ptr)
    {
        *ptr = temp;
        temp->next = *ptr;
    }
    else
    {
        while (*ptr != current->next)
        {
            current = current->next;
        }
        current->next = temp;
        temp->next = *ptr;
    }
}

int NodeCount(struct CurrentPlayer *ptr)
{
    struct CurrentPlayer *first = NULL;

    first = ptr;
    int count =0;
    if (NULL != ptr)
    {
        count = 1;
        while(first != ptr->next)
        {
            count++;
            ptr = ptr->next;
        }
    }
    else
    {
        printf("EMPTY LINK LIST\n");
    }

    return count;
}

void DisplayCircularLinkList(struct CurrentPlayer*ptr)
{
    struct CurrentPlayer *first = NULL;

    first = ptr;

    if (NULL != ptr)
    {
        while(first != ptr->next)
        {
            printf("%d-->",ptr->player);
            ptr = ptr->next;
        }
        printf("%d-->%d\n",ptr->player,first->player);
    }
    else
    {
        printf("EMPTY LINK LIST\n");
    }
}

void DeleteByValue(struct CurrentPlayer **ptr,unsigned char value)
{
    struct CurrentPlayer *current = NULL;
    struct CurrentPlayer *trailP  = NULL;

    current = *ptr;
    if (NULL != *ptr)
    {
        if ((*ptr == current)&&(value == current->player))
        {
            trailP = *ptr;
            while(*ptr != trailP->next)
            {
                trailP = trailP->next;
            }
            *ptr = current->next;
            trailP->next = *ptr;

            if (NULL != current)
            {
                free(current);
                current = NULL;
            }
        }
        else
        {
            while((*ptr != current->next) && (value != current->player))
            {
                trailP     = current;
                current    = current->next;
            }

            trailP->next = current->next;

            if (NULL != current)
            {
                free(current);
                current = NULL;
            }
        }
    }
    else
    {
        printf("EMPTY LINK LIST\n");
        exit(1);
    }
}
void roundSummary(PlayerStatus *ptr)
{
    static int posA[MAX_TOKEN]   = {0};
    static int posB[MAX_TOKEN]   = {0};
    static int posC[MAX_TOKEN]   = {0};
    static int posD[MAX_TOKEN]   = {0};
    int index                    = 0;
    int token                    = 0;
    
    switch(ptr->choice)
    {
        case TWO_PLAYER_GAME:
        {
            for (index = 0 ; index < MAX_TOKEN_TRAVELLED;index++)
            {
                for (token = 0;token < MAX_TOKEN ;token++)
                {
                    if (0 <  ptr->TwoPlayers[index].playerA[token])
                        posA[token] = index;

                    if (0 <  ptr->TwoPlayers[index].playerB[token])
                        posB[token] = index;
                }
            }
            for (token = 0; token < MAX_TOKEN ; token++)
                fprintf(stdout,"{'A[%d]':%d, 'B[%d]':%d,}\n",
                        token+1,posA[token],token+1,posB[token]);
            break;
        }
        case THREE_PLAYER_GAME:
        {
            for (index = 0 ; index < MAX_TOKEN_TRAVELLED;index++)
            {
                for (token = 0;token < MAX_TOKEN ;token++)
                {
                    if (0 <  ptr->ThreePlayers[index].playerA[token])
                        posA[token] = index;

                    if (0 <  ptr->ThreePlayers[index].playerB[token])
                        posB[token] = index;

                    if (0 <  ptr->ThreePlayers[index].playerC[token])
                        posC[token] = index;
                }
            }

            for (token = 0; token < MAX_TOKEN ; token++)
                fprintf(stdout,"{'A[%d]':%d, 'B[%d]':%d, 'C[%d]':%d'}\n",
                        token+1,posA[token],token+1,posB[token],token+1,posC[token]);

            break;
        }
        case FOUR_PLAYER_GAME:
        {
            for (index = 0 ; index < MAX_TOKEN_TRAVELLED;index++)
            {
                for (token = 0;token < MAX_TOKEN ;token++)
                {
                    if (0 <  ptr->FourPlayers[index].playerA[token])
                        posA[token] = index;

                    if (0 <  ptr->FourPlayers[index].playerB[token])
                        posB[token] = index;

                    if (0 <  ptr->FourPlayers[index].playerC[token])
                        posC[token] = index;

                    if (0 <  ptr->FourPlayers[index].playerD[token])
                        posD[token] = index;
                }

            }
            for (token = 0; token < MAX_TOKEN ; token++)
                fprintf(stdout,"{'A[%d]':%d, 'B[%d]':%d, 'C[%d]':%d, 'D[%d]:%d'}\n",
                        token+1,posA[token],token+1,posB[token],token+1,posC[token],token+1,posD[token]);

            break;
        }
        default:
        {
            fprintf(stderr,"Wrong Player Choice Some Error Occured\n");
            exit(0);
        }
    }
}

void LoadingPlayers(struct CurrentPlayer **ptr,unsigned char choice)
{
    switch(choice)
    {
        case TWO_PLAYER_GAME:
        {
            LoadPlayers(ptr,TURN_OF_PLAYER_A);
            LoadPlayers(ptr,TURN_OF_PLAYER_B);
            break;
        }
        case THREE_PLAYER_GAME:
        {
            LoadPlayers(ptr,TURN_OF_PLAYER_A);
            LoadPlayers(ptr,TURN_OF_PLAYER_B);
            LoadPlayers(ptr,TURN_OF_PLAYER_C);
            break;
        }
        case FOUR_PLAYER_GAME:
        {
            LoadPlayers(ptr,TURN_OF_PLAYER_A);
            LoadPlayers(ptr,TURN_OF_PLAYER_B);
            LoadPlayers(ptr,TURN_OF_PLAYER_C);
            LoadPlayers(ptr,TURN_OF_PLAYER_D);
            break;
        }
        default:
        {
            fprintf(stderr,"Wrong Player Choice Some Error Occured\n");
                break;
        }
    }
}

unsigned char MapPlayerInput(int num)
{
    if (2 == num)
        return TWO_PLAYER_GAME; 
    else if (3 == num)
        return THREE_PLAYER_GAME;
    else if (4 == num)
        return FOUR_PLAYER_GAME;
    else
    {
        fprintf(stderr,"Wrong player Number Exiting Game...!!!\n");
        exit(1);
    }
}

int currentOpenToken(PlayerStatus *ptr,unsigned char player)
{
    int ret_val = 0;
    switch(player)
    {
        case TURN_OF_PLAYER_A:
        {
            /*all token are open*/
            if (0x0F == (0x0F & ptr->TokenStatusPlayerA))
            {
                ret_val = 0;
                break;
            }
            /*ALL tokens are closed*/
            else if (0x00 == (0x00 ^ ptr->TokenStatusPlayerA))
            {
                ret_val = 1;
                break;
            }
            /*Some Tokens are open and other are closed*/
            else
                ret_val = 2;
            break;
        }
        case TURN_OF_PLAYER_B:
        {
            /*all token are open*/
            if (0x0F == (0x0F & ptr->TokenStatusPlayerB))
            {
                ret_val = 0;
                break;
            }
            /*ALL tokens are closed*/
            else if (0x00 == (0x00 ^ ptr->TokenStatusPlayerB))
            {
                ret_val = 1;
                break;
            }
            /*Some Tokens are open and other are closed*/
            else
                ret_val = 2;
            break;
        }
        case TURN_OF_PLAYER_C:
        {
            /*all token are open*/
            if (0x0F == (0x0F & ptr->TokenStatusPlayerC))
            {
                ret_val = 0;
                break;
            }
            /*ALL tokens are closed*/
            else if (0x00 == (0x00 ^ ptr->TokenStatusPlayerC))
            {
                ret_val = 1;
                break;
            }
            /*Some Tokens are open and other are closed*/
            else
                ret_val = 2;
            break;
        }
        case TURN_OF_PLAYER_D:
        {
            /*all token are open*/
            if (0x0F == (0x0F & ptr->TokenStatusPlayerC))
            {
                ret_val = 0;
                break;
            }
            /*ALL tokens are closed*/
            else if (0x00 == (0x00 ^ ptr->TokenStatusPlayerC))
            {
                ret_val = 1;
                break;
            }
            /*Some Tokens are open and other are closed*/
            else
                ret_val = 2;
            break;
        }

        default:
        {
            fprintf(stderr,"Wrong Player Choice...!!! Some Error Occured...!!!\n");
            exit(1);
        }
    }
    return ret_val;
}

unsigned char mapTokenInput(int token_pos)
{
    if (1 == token_pos)
        return TOKEN_ONE_FINSHED; 
    else if (2 == token_pos)
        return TOKEN_TWO_STARTED;
    else if (3 == token_pos)
        return TOKEN_THREE_STARTED;
    else if (4 == token_pos)
        return TOKEN_FOUR_STARTED;
    else
    {
        fprintf(stderr,"Wrong Token Number Exiting Game...!!!\n");
        exit(1);
    }
}

unsigned char mapTokenInputForClosedToken(int token_pos)
{
    if (1 == token_pos)
        return TOKEN_ONE_STARTED; 
    else if (2 == token_pos)
        return TOKEN_TWO_STARTED;
    else if (3 == token_pos)
        return TOKEN_THREE_STARTED;
    else if (4 == token_pos)
        return TOKEN_FOUR_STARTED;
    else
    {
        fprintf(stderr,"Wrong Token Number Exiting Game...!!!\n");
        exit(1);
    }
}

int MapPlayerBoardIndex(int board_pos)
{
    int ret_val = 0;
    int index   = 0;
    if (board_pos < MAX_BOARD_COUNT)
        ret_val = board_pos;
    else
    {
        index = board_pos - MAX_BOARD_COUNT;

        if (index)
            ret_val = index;
        else
            ret_val = 0;
    }
    return ret_val;
}

void changeTokenPos
(
 BoardStatus **B_Status,
 PlayerStatus **P_Status,
 int token_pos,
 int board_pos,
 unsigned char player
 )
{
    BoardStatus *p_Board       = NULL;
    PlayerStatus *p_player     = NULL;

    p_Board  = *B_Status;
    p_player = *P_Status;

    switch(p_player->choice)
    {
        case TWO_PLAYER_GAME:
        {
            if (TURN_OF_PLAYER_A == player)
            {
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->TwoPlayers[board_pos].playerA[token_pos] = 1;
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->TwoPlayers[board_pos].playerA[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player A reached Finish",token_pos);
                    p_player->TokenStatusPlayerA |= mapTokenInput(token_pos);
                }
            }
            else if (TURN_OF_PLAYER_B == player)
            {
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->TwoPlayers[board_pos].playerB[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player B reached Finish",token_pos);
                    p_player->TokenStatusPlayerB |= mapTokenInput(token_pos);
                }

                board_pos = MapPlayerBoardIndex(board_pos);
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->TwoPlayers[board_pos].playerB[token_pos] = 1;
            }
            break;
        }
        case THREE_PLAYER_GAME:
        {
            if (TURN_OF_PLAYER_A == player)
            {
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->ThreePlayers[board_pos].playerA[token_pos] = 1;
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->ThreePlayers[board_pos].playerA[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player A reached Finish",token_pos);
                    p_player->TokenStatusPlayerA |= mapTokenInput(token_pos);
                }
            }
            else if (TURN_OF_PLAYER_B == player)
            {
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->ThreePlayers[board_pos].playerB[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player B reached Finish",token_pos);
                    p_player->TokenStatusPlayerB |= mapTokenInput(token_pos);
                }

                board_pos = MapPlayerBoardIndex(board_pos);
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->ThreePlayers[board_pos].playerB[token_pos] = 1;
            }
            else if (TURN_OF_PLAYER_C == player)
            {
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->ThreePlayers[board_pos].playerC[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player C reached Finish",token_pos);
                    p_player->TokenStatusPlayerC |= mapTokenInput(token_pos);
                }
                board_pos = MapPlayerBoardIndex(board_pos);
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->ThreePlayers[board_pos].playerC[token_pos] = 1;

            }
            break;
        }
        case FOUR_PLAYER_GAME:
        {
            if (TURN_OF_PLAYER_A == player)
            {
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->FourPlayers[board_pos].playerA[token_pos] = 1;
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->FourPlayers[board_pos].playerA[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player A reached Finish",token_pos);
                    p_player->TokenStatusPlayerA |= mapTokenInput(token_pos);
                }

            }
            else if (TURN_OF_PLAYER_B == player)
            {
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->FourPlayers[board_pos].playerB[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player B reached Finish",token_pos);
                    p_player->TokenStatusPlayerB |= mapTokenInput(token_pos);
                }

                board_pos = MapPlayerBoardIndex(board_pos);
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->FourPlayers[board_pos].playerB[token_pos] = 1;
            }
            else if (TURN_OF_PLAYER_C == player)
            {
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->FourPlayers[board_pos].playerC[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player C reached Finish",token_pos);
                    p_player->TokenStatusPlayerC |= mapTokenInput(token_pos);
                }

                board_pos = MapPlayerBoardIndex(board_pos);
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->FourPlayers[board_pos].playerC[token_pos] = 1;

            }
            else if (TURN_OF_PLAYER_D == player)
            {
                if ((0 <= board_pos)&& (MAX_TOKEN_TRAVELLED > board_pos))
                    p_player->FourPlayers[board_pos].playerD[token_pos] = 1;
                if (MAX_TOKEN_TRAVELLED -1 == board_pos)
                {
                    fprintf(stdout,"Token %d of Player D reached Finish",token_pos);
                    p_player->TokenStatusPlayerD |= mapTokenInput(token_pos);
                }

                board_pos = MapPlayerBoardIndex(board_pos);
                if ((0 <= board_pos)&& (MAX_BOARD_COUNT > board_pos))
                    p_Board->FourPlayers[board_pos].playerD[token_pos] = 1;
            }
            break;
        }
        default:
        {
            fprintf(stderr,"Wrong Player Choice Some Error Occured\n");
            exit(1);
        }
    }

}

void TokenCondition(PlayerStatus *p_player,unsigned char player)
{
    switch(player)
    {
        case TURN_OF_PLAYER_A:
        {
            /*status of open Token of Player A*/
            if (TOKEN_ONE_STARTED & p_player->TokenStatusPlayerA)
                fprintf(stdout,"Player A token 1 is open\n");
            if (TOKEN_TWO_STARTED & p_player->TokenStatusPlayerA)
                fprintf(stdout,"Player A token 2 is open\n");
            if (TOKEN_THREE_STARTED & p_player->TokenStatusPlayerA)
                fprintf(stdout,"Player A token 3 is open\n");
            if (TOKEN_FOUR_STARTED & p_player->TokenStatusPlayerA)
                fprintf(stdout,"Player A token 4 is open\n");
            /*status of finished Token of Player A*/
            if (TOKEN_ONE_FINSHED & p_player->TokenStatusPlayerA)
                fprintf(stdout,"Player A token 1  is Reached Finish\n");
            if (TOKEN_TWO_FINSHED & p_player->TokenStatusPlayerA)
                fprintf(stdout,"Player A token 2  is Reached Finish\n");
            if (TOKEN_THREE_FINSHED & p_player->TokenStatusPlayerA)
                fprintf(stdout,"Player A token 3  is Reached Finish\n");
            if (TOKEN_FOUR_FINSHED & p_player->TokenStatusPlayerA)
                fprintf(stdout,"Player A token 4  is Reached Finish\n");
            break;
        }
        case TURN_OF_PLAYER_B:
        {
            /*status of open Token of Player B*/
            if (TOKEN_ONE_STARTED & p_player->TokenStatusPlayerB)
                fprintf(stdout,"Player B token 1 is open\n");
            if (TOKEN_TWO_STARTED & p_player->TokenStatusPlayerB)
                fprintf(stdout,"Player B token 2 is open\n");
            if (TOKEN_THREE_STARTED & p_player->TokenStatusPlayerB)
                fprintf(stdout,"Player B token 3 is open\n");
            if (TOKEN_FOUR_STARTED & p_player->TokenStatusPlayerB)
                fprintf(stdout,"Player B token 4 is open\n");
            /*status of finished Token of Player B*/
            if (TOKEN_ONE_FINSHED & p_player->TokenStatusPlayerB)
                fprintf(stdout,"Player B token 1  is Reached Finish\n");
            if (TOKEN_TWO_FINSHED & p_player->TokenStatusPlayerB)
                fprintf(stdout,"Player B token 2  is Reached Finish\n");
            if (TOKEN_THREE_FINSHED & p_player->TokenStatusPlayerB)
                fprintf(stdout,"Player B token 3  is Reached Finish\n");
            if (TOKEN_FOUR_FINSHED & p_player->TokenStatusPlayerB)
                fprintf(stdout,"Player B token 4  is Reached Finish\n");
            break;
        }
        case TURN_OF_PLAYER_C:
        {
            /*status of open Token of Player C*/
            if (TOKEN_ONE_STARTED & p_player->TokenStatusPlayerC)
                fprintf(stdout,"Player C token 1 is open\n");
            if (TOKEN_TWO_STARTED & p_player->TokenStatusPlayerC)
                fprintf(stdout,"Player C token 2 is open\n");
            if (TOKEN_THREE_STARTED & p_player->TokenStatusPlayerC)
                fprintf(stdout,"Player C token 3 is open\n");
            if (TOKEN_FOUR_STARTED & p_player->TokenStatusPlayerC)
                fprintf(stdout,"Player C token 4 is open\n");
            /*status of finished Token of Player C*/
            if (TOKEN_ONE_FINSHED & p_player->TokenStatusPlayerC)
                fprintf(stdout,"Player C token 1  is Reached Finish\n");
            if (TOKEN_TWO_FINSHED & p_player->TokenStatusPlayerC)
                fprintf(stdout,"Player C token 2  is Reached Finish\n");
            if (TOKEN_THREE_FINSHED & p_player->TokenStatusPlayerC)
                fprintf(stdout,"Player C token 3  is Reached Finish\n");
            if (TOKEN_FOUR_FINSHED & p_player->TokenStatusPlayerC)
                fprintf(stdout,"Player C token 4  is Reached Finish\n");
            break;
        }
        case TURN_OF_PLAYER_D:
        {
            /*status of open Token of Player D*/
            if (TOKEN_ONE_STARTED & p_player->TokenStatusPlayerD)
                fprintf(stdout,"Player D token 1 is open\n");
            if (TOKEN_TWO_STARTED & p_player->TokenStatusPlayerD)
                fprintf(stdout,"Player D token 2 is open\n");
            if (TOKEN_THREE_STARTED & p_player->TokenStatusPlayerD)
                fprintf(stdout,"Player D token 3 is open\n");
            if (TOKEN_FOUR_STARTED & p_player->TokenStatusPlayerD)
                fprintf(stdout,"Player D token 4 is open\n");
            /*status of finished Token of Player D*/
            if (TOKEN_ONE_FINSHED & p_player->TokenStatusPlayerD)
                fprintf(stdout,"Player D token 1  is Reached Finish\n");
            if (TOKEN_TWO_FINSHED & p_player->TokenStatusPlayerD)
                fprintf(stdout,"Player D token 2  is Reached Finish\n");
            if (TOKEN_THREE_FINSHED & p_player->TokenStatusPlayerD)
                fprintf(stdout,"Player D token 3  is Reached Finish\n");
            if (TOKEN_FOUR_FINSHED & p_player->TokenStatusPlayerD)
                fprintf(stdout,"Player D token 4  is Reached Finish\n");
            break;
        }
        default:
        {
            fprintf(stderr,"Wrong Player Choice Some Error Occured\n");
            exit(0);
        }
    }
}

void TokenMovement
(
 unsigned char player,
 BoardStatus **B_Status,
 PlayerStatus **P_Status,
 int DiceNum
)
{
    static int TokenA[MAX_TOKEN]   = {0};
    static int TokenB[MAX_TOKEN]   = {0};
    static int TokenC[MAX_TOKEN]   = {0};
    static int TokenD[MAX_TOKEN]   = {0};
    int        tokenIndex          = 0;
    int        boardIndex          = 0;
    int        Index               = 0;

    PlayerStatus *p_player     = NULL;
    p_player = *P_Status;

    switch(player)
    {
        case TURN_OF_PLAYER_A:
        {
            if (0 == currentOpenToken(p_player,TURN_OF_PLAYER_A))
            {
                fprintf(stdout,"All tokens are open which one do you wanna move:\n");
                scanf("%d",&tokenIndex);
                /*If DICE value is greater than 6*/
                if (6 < DiceNum)
                {
                    Index = DiceNum - MAX_DICE_NUM; 
                    fprintf(stdout,"Which Token You want to move\n");
                    scanf("%d",&tokenIndex);
                    if(Index)
                    {
                        TokenA[tokenIndex-1] += MAX_DICE_NUM; 
                        boardIndex = TokenA[tokenIndex-1];
                        changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                        /*If DICE value is greater than 12*/
                        if (6 < Index)
                        {
                            Index = Index - MAX_DICE_NUM; 
                            fprintf(stdout,"Which Token You want to move\n");
                            scanf("%d",&tokenIndex);
                            TokenA[tokenIndex-1] += MAX_DICE_NUM; 
                            boardIndex = TokenA[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                            /*if dice roll is like 6,6,5*/
                            fprintf(stdout,"Which Token You want to move\n");
                            scanf("%d",&tokenIndex);
                            TokenA[tokenIndex-1] += Index; 
                            boardIndex = TokenA[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                        }
                        /*If dice roll is like 6,5*/
                        else
                        {
                            fprintf(stdout,"Which Token You want to move \n");
                            scanf("%d",&tokenIndex);
                            TokenA[tokenIndex-1] += Index; 
                            boardIndex = TokenA[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                        }
                    }
                }
                else
                {
                    TokenA[tokenIndex-1] += DiceNum; 
                    boardIndex = TokenA[tokenIndex-1];
                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                }
            }
            else if (1 == currentOpenToken(p_player,TURN_OF_PLAYER_A))
            {
                fprintf(stdout,"All tokens are closed\n");
                /*If Dice value is greater than 6*/
                if (6 < DiceNum)
                {
                    Index = DiceNum - MAX_DICE_NUM; 
                    fprintf(stdout,"Which Token You want to open\n");
                    scanf("%d",&tokenIndex);
                    p_player->TokenStatusPlayerA |= mapTokenInputForClosedToken(tokenIndex);
                    if(Index)
                    {
                        TokenA[tokenIndex-1] = 0; 
                        boardIndex = TokenA[tokenIndex-1];
                        changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                        /*If DICE value is greater than 12*/
                        if (6 < Index)
                        {
                            Index = Index - MAX_DICE_NUM; 
                            fprintf(stdout,"Which Token You want to move/open\n");
                            scanf("%d",&tokenIndex);
                            if (mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerA)
                            {
                                fprintf(stdout,"Token is already open...!!! moving Token %d\n",tokenIndex);
                                boardIndex = MAX_DICE_NUM;
                                TokenA[tokenIndex-1] += MAX_DICE_NUM; 
                                boardIndex = TokenA[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                TokenA[tokenIndex-1] += Index; 
                                boardIndex = TokenA[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                            }
                            else
                            {
                                p_player->TokenStatusPlayerA |= mapTokenInputForClosedToken(tokenIndex);
                                TokenA[tokenIndex-1] = 0; 
                                boardIndex = TokenA[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                TokenA[tokenIndex-1] += Index; 
                                boardIndex = TokenA[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                            }
                        }
                        /*If dice roll is like 6,5*/
                        else
                        {
                            fprintf(stdout,"Which Token You want to move \n");
                            scanf("%d",&tokenIndex);
                            TokenA[tokenIndex-1] += Index; 
                            boardIndex = TokenA[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                        }
                    }
                }
            }
            if (2 == currentOpenToken(p_player,TURN_OF_PLAYER_A))
            {
                TokenCondition(p_player,TURN_OF_PLAYER_A);
                /*If DICE value is greater than 6*/
                if (6 < DiceNum)
                {
                    fprintf(stdout,"Which one do you wanna move:\n");
                    scanf("%d",&tokenIndex);
                    if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerA))
                    {
                        fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                        break;
                    }
                    else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerA))
                    {
                        Index = DiceNum - MAX_DICE_NUM; 
                        if(Index)
                        {
                            TokenA[tokenIndex-1] += MAX_DICE_NUM; 
                            boardIndex = TokenA[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                            /*If DICE value is greater than 12*/
                            if (6 < Index)
                            {
                                Index = Index - MAX_DICE_NUM; 
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerA))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerA))
                                {
                                    TokenA[tokenIndex-1] += MAX_DICE_NUM; 
                                    boardIndex = TokenA[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerA))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerA))
                                {
                                    TokenA[tokenIndex-1] += Index; 
                                    boardIndex = TokenA[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }

                            }
                            /*If dice roll is like 6,5*/
                            else
                            {
                                fprintf(stdout,"Which Token You want to move \n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerA))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerA))
                                {
                                    TokenA[tokenIndex-1] += Index; 
                                    boardIndex = TokenA[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                            }
                        }
                        else
                        {
                            fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                            break;
                        }
                    }
                    else
                    {
                        fprintf(stdout,"Which one do you wanna move:\n");
                        scanf("%d",&tokenIndex);
                        if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerA))
                        {
                            fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                            break;
                        }
                        else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerA))
                        {
                            TokenA[tokenIndex-1] += DiceNum; 
                            boardIndex = TokenA[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_A);
                        }
                        else
                        {
                            fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                            break;
                        }
                    }
                }
            }
            break;
        }
        case TURN_OF_PLAYER_B:
        {
            if (0 == currentOpenToken(p_player,TURN_OF_PLAYER_B))
            {
                fprintf(stdout,"All tokens are open which one do you wanna move:\n");
                scanf("%d",&tokenIndex);
                /*If DICE value is greater than 6*/
                if (6 < DiceNum)
                {
                    Index = DiceNum - MAX_DICE_NUM; 
                    fprintf(stdout,"Which Token You want to move\n");
                    scanf("%d",&tokenIndex);
                    if(Index)
                    {
                        TokenB[tokenIndex-1] += MAX_DICE_NUM; 
                        boardIndex = TokenB[tokenIndex-1];
                        changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                        /*If DICE value is greater than 12*/
                        if (6 < Index)
                        {
                            Index = Index - MAX_DICE_NUM; 
                            fprintf(stdout,"Which Token You want to move\n");
                            scanf("%d",&tokenIndex);
                            TokenB[tokenIndex-1] += MAX_DICE_NUM; 
                            boardIndex = TokenB[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                            /*if dice roll is like 6,6,5*/
                            fprintf(stdout,"Which Token You want to move\n");
                            scanf("%d",&tokenIndex);
                            TokenB[tokenIndex-1] += Index; 
                            boardIndex = TokenB[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                        }
                        /*If dice roll is like 6,5*/
                        else
                        {
                            fprintf(stdout,"Which Token You want to move \n");
                            scanf("%d",&tokenIndex);
                            TokenB[tokenIndex-1] += Index; 
                            boardIndex = TokenB[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                        }
                    }
                }
                else
                {
                    TokenB[tokenIndex-1] += DiceNum; 
                    boardIndex = TokenB[tokenIndex-1];
                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                }
            }
            else if (1 == currentOpenToken(p_player,TURN_OF_PLAYER_B))
            {
                fprintf(stdout,"All tokens are closed\n");
                /*If Dice value is greater than 6*/
                if (6 < DiceNum)
                {
                    Index = DiceNum - MAX_DICE_NUM; 
                    fprintf(stdout,"Which Token You want to open\n");
                    scanf("%d",&tokenIndex);
                    p_player->TokenStatusPlayerB |= mapTokenInputForClosedToken(tokenIndex);
                    if(Index)
                    {
                        TokenB[tokenIndex-1] = 0; 
                        boardIndex = TokenB[tokenIndex-1];
                        changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                        /*If DICE value is greater than 12*/
                        if (6 < Index)
                        {
                            Index = Index - MAX_DICE_NUM; 
                            fprintf(stdout,"Which Token You want to move/open\n");
                            scanf("%d",&tokenIndex);
                            if (mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerB)
                            {
                                fprintf(stdout,"Token is already open...!!! moving Token %d\n",tokenIndex);
                                TokenB[tokenIndex-1] += MAX_DICE_NUM; 
                                boardIndex = TokenB[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                TokenB[tokenIndex-1] += Index; 
                                boardIndex = TokenB[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                            }
                            else
                            {
                                p_player->TokenStatusPlayerB |= mapTokenInputForClosedToken(tokenIndex);
                                TokenB[tokenIndex-1] = 0; 
                                boardIndex = TokenB[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                TokenB[tokenIndex-1] += Index; 
                                boardIndex = TokenB[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                            }
                        }
                        /*If dice roll is like 6,5*/
                        else
                        {
                            fprintf(stdout,"Which Token You want to move \n");
                            scanf("%d",&tokenIndex);
                            TokenB[tokenIndex-1] += Index; 
                            boardIndex = TokenB[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                        }
                    }
                }
            }
            if (2 == currentOpenToken(p_player,TURN_OF_PLAYER_B))
            {
                TokenCondition(p_player,TURN_OF_PLAYER_B);
                /*If DICE value is greater than 6*/
                if (6 < DiceNum)
                {
                    fprintf(stdout,"Which one do you wanna move:\n");
                    scanf("%d",&tokenIndex);
                    if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerB))
                    {
                        fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                        break;
                    }
                    else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerB))
                    {
                        Index = DiceNum - MAX_DICE_NUM; 
                        if(Index)
                        {
                            TokenB[tokenIndex-1] += MAX_DICE_NUM; 
                            boardIndex = TokenB[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                            /*If DICE value is greater than 12*/
                            if (6 < Index)
                            {
                                Index = Index - MAX_DICE_NUM; 
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerB))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerB))
                                {
                                    TokenB[tokenIndex-1] += MAX_DICE_NUM; 
                                    boardIndex = TokenB[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerB))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerB))
                                {
                                    TokenB[tokenIndex-1] += Index; 
                                    boardIndex = TokenB[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                            }
                            /*If dice roll is like 6,5*/
                            else
                            {
                                fprintf(stdout,"Which Token You want to move \n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerB))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerB))
                                {
                                    TokenB[tokenIndex-1] += Index; 
                                    boardIndex = TokenB[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                            }
                        }
                        else
                        {
                            fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                            break;
                        }
                    }
                    else
                    {
                        fprintf(stdout,"Which one do you wanna move:\n");
                        scanf("%d",&tokenIndex);
                        if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerB))
                        {
                            fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                            break;
                        }
                        else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerB))
                        {
                            TokenB[tokenIndex-1] += DiceNum; 
                            boardIndex = TokenB[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_B);
                        }
                        else
                        {
                            fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                            break;
                        }
                    }
                }
            }
            break;
        }
        case TURN_OF_PLAYER_C:
        {
            if (0 == currentOpenToken(p_player,TURN_OF_PLAYER_C))
            {
                fprintf(stdout,"All tokens are open which one do you wanna move:\n");
                scanf("%d",&tokenIndex);
                /*If DICE value is greater than 6*/
                if (6 < DiceNum)
                {
                    Index = DiceNum - MAX_DICE_NUM; 
                    fprintf(stdout,"Which Token You want to move\n");
                    scanf("%d",&tokenIndex);
                    if(Index)
                    {
                        TokenC[tokenIndex-1] += MAX_DICE_NUM; 
                        boardIndex = TokenC[tokenIndex-1];
                        changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                        /*If DICE value is greater than 12*/
                        if (6 < Index)
                        {
                            Index = Index - MAX_DICE_NUM; 
                            fprintf(stdout,"Which Token You want to move\n");
                            scanf("%d",&tokenIndex);
                            TokenC[tokenIndex-1] += MAX_DICE_NUM; 
                            boardIndex = TokenC[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                            /*if dice roll is like 6,6,5*/
                            fprintf(stdout,"Which Token You want to move\n");
                            scanf("%d",&tokenIndex);
                            TokenC[tokenIndex-1] += Index; 
                            boardIndex = TokenC[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                        }
                        /*If dice roll is like 6,5*/
                        else
                        {
                            fprintf(stdout,"Which Token You want to move \n");
                            scanf("%d",&tokenIndex);
                            TokenC[tokenIndex-1] += Index; 
                            boardIndex = TokenC[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                        }
                    }
                }
                else
                {
                    TokenC[tokenIndex-1] += DiceNum; 
                    boardIndex = TokenC[tokenIndex-1];
                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                }
            }
            else if (1 == currentOpenToken(p_player,TURN_OF_PLAYER_C))
            {
                fprintf(stdout,"All tokens are closed\n");
                /*If Dice value is greater than 6*/
                if (6 < DiceNum)
                {
                    Index = DiceNum - MAX_DICE_NUM; 
                    fprintf(stdout,"Which Token You want to open\n");
                    scanf("%d",&tokenIndex);
                    p_player->TokenStatusPlayerC |= mapTokenInputForClosedToken(tokenIndex);
                    if(Index)
                    {
                        TokenC[tokenIndex-1] = 0; 
                        boardIndex = TokenC[tokenIndex-1];
                        changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                        /*If DICE value is greater than 12*/
                        if (6 < Index)
                        {
                            Index = Index - MAX_DICE_NUM; 
                            fprintf(stdout,"Which Token You want to move/open\n");
                            scanf("%d",&tokenIndex);
                            if (mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerC)
                            {
                                fprintf(stdout,"Token is already open...!!! moving Token %d\n",tokenIndex);
                                TokenC[tokenIndex-1] += MAX_DICE_NUM; 
                                boardIndex = TokenC[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                TokenC[tokenIndex-1] += Index; 
                                boardIndex = TokenC[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                            }
                            else
                            {
                                p_player->TokenStatusPlayerC |= mapTokenInputForClosedToken(tokenIndex);
                                TokenC[tokenIndex-1] = 0; 
                                boardIndex = TokenC[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                TokenC[tokenIndex-1] += Index; 
                                boardIndex = TokenC[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                            }
                        }
                        /*If dice roll is like 6,5*/
                        else
                        {
                            fprintf(stdout,"Which Token You want to move \n");
                            scanf("%d",&tokenIndex);
                            TokenC[tokenIndex-1] += Index; 
                            boardIndex = TokenC[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                        }
                    }
                }
            }
            if (2 == currentOpenToken(p_player,TURN_OF_PLAYER_C))
            {
                TokenCondition(p_player,TURN_OF_PLAYER_C);
                /*If DICE value is greater than 6*/
                if (6 < DiceNum)
                {
                    fprintf(stdout,"Which one do you wanna move:\n");
                    scanf("%d",&tokenIndex);
                    if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerC))
                    {
                        fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                        break;
                    }
                    else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerC))
                    {
                        Index = DiceNum - MAX_DICE_NUM; 
                        if(Index)
                        {
                            TokenC[tokenIndex-1] += MAX_DICE_NUM; 
                            boardIndex = TokenC[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                            /*If DICE value is greater than 12*/
                            if (6 < Index)
                            {
                                Index = Index - MAX_DICE_NUM; 
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerC))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerC))
                                {
                                    TokenC[tokenIndex-1] += MAX_DICE_NUM; 
                                    boardIndex = TokenC[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerC))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerC))
                                {
                                    TokenC[tokenIndex-1] += Index; 
                                    boardIndex = TokenC[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                            }
                            /*If dice roll is like 6,5*/
                            else
                            {
                                fprintf(stdout,"Which Token You want to move \n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerC))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerC))
                                {
                                    TokenC[tokenIndex-1] += Index; 
                                    boardIndex = TokenC[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                            }
                        }
                        else
                        {
                            fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                            break;
                        }
                    }
                    else
                    {
                        fprintf(stdout,"Which one do you wanna move:\n");
                        scanf("%d",&tokenIndex);
                        if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerC))
                        {
                            fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                            break;
                        }
                        else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerC))
                        {
                            TokenC[tokenIndex-1] += DiceNum; 
                            boardIndex = TokenC[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_C);
                        }
                        else
                        {
                            fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                            break;
                        }
                    }
                }
            }
            break;
        }
        case TURN_OF_PLAYER_D:
        {
            if (0 == currentOpenToken(p_player,TURN_OF_PLAYER_D))
            {
                fprintf(stdout,"All tokens are open which one do you wanna move:\n");
                scanf("%d",&tokenIndex);
                /*If DICE value is greater than 6*/
                if (6 < DiceNum)
                {
                    Index = DiceNum - MAX_DICE_NUM; 
                    fprintf(stdout,"Which Token You want to move\n");
                    scanf("%d",&tokenIndex);
                    if(Index)
                    {
                        TokenD[tokenIndex-1] += MAX_DICE_NUM; 
                        boardIndex = TokenD[tokenIndex-1];
                        changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                        /*If DICE value is greater than 12*/
                        if (6 < Index)
                        {
                            Index = Index - MAX_DICE_NUM; 
                            fprintf(stdout,"Which Token You want to move\n");
                            scanf("%d",&tokenIndex);
                            TokenD[tokenIndex-1] += MAX_DICE_NUM; 
                            boardIndex = TokenD[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                            /*if dice roll is like 6,6,5*/
                            fprintf(stdout,"Which Token You want to move\n");
                            scanf("%d",&tokenIndex);
                            TokenD[tokenIndex-1] += Index; 
                            boardIndex = TokenD[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                        }
                        /*If dice roll is like 6,5*/
                        else
                        {
                            fprintf(stdout,"Which Token You want to move \n");
                            scanf("%d",&tokenIndex);
                            TokenD[tokenIndex-1] += Index; 
                            boardIndex = TokenD[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                        }
                    }
                }
                else
                {
                    TokenD[tokenIndex-1] += DiceNum; 
                    boardIndex = TokenD[tokenIndex-1];
                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                }
            }
            else if (1 == currentOpenToken(p_player,TURN_OF_PLAYER_D))
            {
                fprintf(stdout,"All tokens are closed\n");
                /*If Dice value is greater than 6*/
                if (6 < DiceNum)
                {
                    Index = DiceNum - MAX_DICE_NUM; 
                    fprintf(stdout,"Which Token You want to open\n");
                    scanf("%d",&tokenIndex);
                    p_player->TokenStatusPlayerD |= mapTokenInputForClosedToken(tokenIndex);
                    if(Index)
                    {
                        TokenD[tokenIndex-1] = 0; 
                        boardIndex = TokenD[tokenIndex-1];
                        changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                        /*If DICE value is greater than 12*/
                        if (6 < Index)
                        {
                            Index = Index - MAX_DICE_NUM; 
                            fprintf(stdout,"Which Token You want to move/open\n");
                            scanf("%d",&tokenIndex);
                            if (mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerD)
                            {
                                fprintf(stdout,"Token is already open...!!! moving Token %d\n",tokenIndex);
                                TokenD[tokenIndex-1] += MAX_DICE_NUM; 
                                boardIndex = TokenD[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                TokenD[tokenIndex-1] += Index; 
                                boardIndex = TokenD[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                            }
                            else
                            {
                                p_player->TokenStatusPlayerD |= mapTokenInputForClosedToken(tokenIndex);
                                TokenD[tokenIndex-1] = 0; 
                                boardIndex = TokenD[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                TokenD[tokenIndex-1] += Index; 
                                boardIndex = TokenD[tokenIndex-1];
                                changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                            }
                        }
                        /*If dice roll is like 6,5*/
                        else
                        {
                            fprintf(stdout,"Which Token You want to move \n");
                            scanf("%d",&tokenIndex);
                            TokenD[tokenIndex-1] += Index; 
                            boardIndex = TokenD[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                        }
                    }
                }
            }
            if (2 == currentOpenToken(p_player,TURN_OF_PLAYER_D))
            {
                TokenCondition(p_player,TURN_OF_PLAYER_D);
                /*If DICE value is greater than 6*/
                if (6 < DiceNum)
                {
                    fprintf(stdout,"Which one do you wanna move:\n");
                    scanf("%d",&tokenIndex);
                    if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerD))
                    {
                        fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                        break;
                    }
                    else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerD))
                    {
                        Index = DiceNum - MAX_DICE_NUM; 
                        if(Index)
                        {
                            TokenD[tokenIndex-1] += MAX_DICE_NUM; 
                            boardIndex = TokenD[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                            /*If DICE value is greater than 12*/
                            if (6 < Index)
                            {
                                Index = Index - MAX_DICE_NUM; 
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerD))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerD))
                                {
                                    TokenD[tokenIndex-1] += MAX_DICE_NUM; 
                                    boardIndex = TokenD[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                                /*if dice roll is like 6,6,5*/
                                fprintf(stdout,"Which Token You want to move\n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerD))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerD))
                                {
                                    TokenD[tokenIndex-1] += Index; 
                                    boardIndex = TokenD[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                            }
                            /*If dice roll is like 6,5*/
                            else
                            {
                                fprintf(stdout,"Which Token You want to move \n");
                                scanf("%d",&tokenIndex);
                                if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerD))
                                {
                                    fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                                    break;
                                }
                                else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerD))
                                {
                                    TokenD[tokenIndex-1] += Index; 
                                    boardIndex = TokenD[tokenIndex-1];
                                    changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                                }
                                else
                                {
                                    fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                                    break;
                                }
                            }
                        }
                        else
                        {
                            fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                            break;
                        }
                    }
                    else
                    {
                        fprintf(stdout,"Which one do you wanna move:\n");
                        scanf("%d",&tokenIndex);
                        if((mapTokenInput(tokenIndex) & p_player->TokenStatusPlayerD))
                        {
                            fprintf(stdout,"Token has already reached finish ...!!! skipping your Turn:\n");
                            break;
                        }
                        else if ((mapTokenInputForClosedToken(tokenIndex) & p_player->TokenStatusPlayerD))
                        {
                            TokenD[tokenIndex-1] += DiceNum; 
                            boardIndex = TokenD[tokenIndex-1];
                            changeTokenPos(B_Status,P_Status,tokenIndex-1,boardIndex, TURN_OF_PLAYER_D);
                        }
                        else
                        {
                            fprintf(stdout,"Token is at HOME ...!!! skipping your Turn:\n");
                            break;
                        }
                    }
                }
            }
            break;
        }
        default:
        {
            fprintf(stderr,"Wrong Player Choice...!!! Some Error Occured...!!!\n");
            exit(1);
        }
    }
}

unsigned char AlltokensReachedFinsish(PlayerStatus *P_status)
{
    unsigned char ret_val = 0x00;
    if (0xf0 & P_status->TokenStatusPlayerA)
    {
        ret_val = TURN_OF_PLAYER_A;
    }
    if (0xf0 & P_status->TokenStatusPlayerB)
    {
        ret_val = TURN_OF_PLAYER_B;
    }
    if (0xf0 & P_status->TokenStatusPlayerC)
    {
        ret_val = TURN_OF_PLAYER_C;
    }
    if (0xf0 & P_status->TokenStatusPlayerD)
    {
        ret_val = TURN_OF_PLAYER_D;
    }

    return ret_val;
}

void commenceRound
(
 struct CurrentPlayer *p_CurrentPlayer,
 BoardStatus **B_Status,
 PlayerStatus **P_Status
)
{
    struct CurrentPlayer *ptr  = NULL;
    int dice  = 0;
    int turn  = 0;
    static int winner  = 0;

    ptr = p_CurrentPlayer;

    while((NULL != ptr) && (turn !=  NodeCount(ptr)))
    {
        switch(ptr->player)
        {
            case TURN_OF_PLAYER_A:
            {
                fprintf(stdout,"User A's Total is %d\n",dice = Rollthedice('A'));

                TokenMovement(TURN_OF_PLAYER_A,B_Status,P_Status,dice);

                if (AlltokensReachedFinsish(*P_Status))
                {
                    winner++;
                    fprintf(stdout,"Player A's ALL token reached FINISH Player Position%d\n",winner);
                    ptr = ptr->next;
                    DeleteByValue(&p_CurrentPlayer,AlltokensReachedFinsish(*P_Status));
                    break;
                }

                ptr = ptr->next;
                break;
            }
            case TURN_OF_PLAYER_B:
            {
                fprintf(stdout,"User B's Total is %d\n",dice = Rollthedice('B'));

                TokenMovement(TURN_OF_PLAYER_B,B_Status,P_Status,dice);
                if (AlltokensReachedFinsish(*P_Status))
                {
                    winner++;
                    fprintf(stdout,"Player B's ALL token reached FINISH Player Position%d\n",winner);
                    ptr = ptr->next;
                    DeleteByValue(&p_CurrentPlayer,AlltokensReachedFinsish(*P_Status));
                    break;
                }

                ptr = ptr->next;
                break;
            }
            case TURN_OF_PLAYER_C:
            {
                fprintf(stdout,"User C's Total is %d\n",dice = Rollthedice('C'));

                TokenMovement(TURN_OF_PLAYER_C,B_Status,P_Status,dice);
                if (AlltokensReachedFinsish(*P_Status))
                {
                    winner++;
                    fprintf(stdout,"Player C's ALL token reached FINISH Player Position%d\n",winner);
                    ptr = ptr->next;
                    DeleteByValue(&p_CurrentPlayer,AlltokensReachedFinsish(*P_Status));
                    break;
                }

                ptr = ptr->next;
                break;
            }
            case TURN_OF_PLAYER_D:
            {
                fprintf(stdout,"User D's Total is %d\n",dice = Rollthedice('D'));

                TokenMovement(TURN_OF_PLAYER_D,B_Status,P_Status,dice);
                if (AlltokensReachedFinsish(*P_Status))
                {
                    winner++;
                    fprintf(stdout,"Player D's ALL token reached FINISH Player Position%d\n",winner);
                    ptr = ptr->next;
                    DeleteByValue(&p_CurrentPlayer,AlltokensReachedFinsish(*P_Status));
                    break;
                }

                ptr = ptr->next;
                break;
            }
            default:
            {
                fprintf(stderr,"Wrong Player Choice Some Error Occured\n");
                exit(1);
            }
        }
        turn++;
    }
}

int main()
{
    struct CurrentPlayer *p_CurrentPlayer = NULL;
    BoardStatus *p_BoardStatus            = NULL;
    PlayerStatus *p_PlayerStatus          = NULL;
    int round                             = 0;
    int playerNum                         = 0;

    /*Allocating Memory to PLayers and Board*/
    p_PlayerStatus = (PlayerStatus *)malloc(sizeof(PlayerStatus));
    if (NULL == p_PlayerStatus)
    {
        fprintf(stderr,"Memory Allocation Failed for p_PlayerStatus...!!!\n");
        exit(1);
    }

    p_BoardStatus = (BoardStatus *)malloc((sizeof(BoardStatus)));
    if (NULL == p_BoardStatus)
    {
        fprintf(stderr,"Memory Allocation Failed for p_BoardStatus...!!!\n");
        exit(1);
    }

    /*Initializing Memory*/
    memset(p_PlayerStatus,0,sizeof(PlayerStatus));
    memset(p_BoardStatus,0,sizeof(BoardStatus));

    /*Loading Players*/
    fprintf(stdout,"Enter Number of players 2 (Min) and 4(Max) : ");
    scanf("%d",&playerNum);

    p_PlayerStatus->choice = p_BoardStatus->choice = MapPlayerInput(playerNum); 
    LoadingPlayers(&p_CurrentPlayer,MapPlayerInput(playerNum));


#if 0
    //    for (int i = 0; i< 10 ; i++)
    fprintf(stdout,"User A's Total is %d\n",Rollthedice('A'));

    /*Testing Display and count*/
    DisplayCircularLinkList(p_CurrentPlayer);
    fprintf(stdout,"Node Count %d\n",NodeCount(p_CurrentPlayer));

    /*Testing deletion*/
    DeleteByValue(&p_CurrentPlayer,TURN_OF_PLAYER_A);
    DisplayCircularLinkList(p_CurrentPlayer);
    fprintf(stdout,"Node Count %d\n",NodeCount(p_CurrentPlayer));
    //DeleteByValue(&p_CurrentPlayer,TURN_OF_PLAYER_B);
    //DisplayCircularLinkList(p_CurrentPlayer);
    //fprintf(stdout,"Node Count %d\n",NodeCount(p_CurrentPlayer));
    //DeleteByValue(&p_CurrentPlayer,TURN_OF_PLAYER_C);
    //DisplayCircularLinkList(p_CurrentPlayer);
    //fprintf(stdout,"Node Count %d\n",NodeCount(p_CurrentPlayer));
    //DeleteByValue(&p_CurrentPlayer,TURN_OF_PLAYER_D);
    //DisplayCircularLinkList(p_CurrentPlayer);
    fprintf(stdout,"Node Count %d\n",NodeCount(p_CurrentPlayer));

    /*Testing Run summary*/
    fprintf(stdout,"After round %d counts are -\n",round);
    roundSummary(p_PlayerStatus);
#endif

#if 1
    /*Commencing Game*/
    do
    {
        round++;
        commenceRound(p_CurrentPlayer,&p_BoardStatus,&p_PlayerStatus);
        fprintf(stdout,"After round %d counts are - \n",round);
        roundSummary(p_PlayerStatus);
#if 0
        if (AlltokensReachedFinsish(p_PlayerStatus))
        {
            DeleteByValue(&p_CurrentPlayer,AlltokensReachedFinsish(p_PlayerStatus));
        }
#endif
    }while(1 <  NodeCount(p_CurrentPlayer));
#endif
    /*Freeing Memory Associated with Player and board*/
    if (NULL != p_PlayerStatus)
    {
        free(p_PlayerStatus);
        p_PlayerStatus = NULL;
    }
    if (NULL != p_BoardStatus)
    {
        free(p_BoardStatus);
        p_BoardStatus = NULL;
    }
    if (NULL != p_CurrentPlayer)
    {
        free(p_CurrentPlayer);
        p_CurrentPlayer = NULL;
    }

    return 0;
}
