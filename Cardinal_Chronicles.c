#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct cards
{
    char element[10];
    int damage;
    int manacost;
    int isblocked;
    char name[30];
};
struct player_cards
{
    char element[10];
    int damage;
    int manacost;
    int isblocked;
    char name[30];
};
struct bot_cards
{
    char element[10];
    int damage;
    int manacost;
    int isblocked;
    char name[30];
};
struct player
{
    int mana;
    int health;
};
struct bot
{
    int mana;
    int health;
};

typedef void (*ptr_to_spells)(struct player_cards *, struct bot_cards *, struct bot *, struct player *, int);
struct player_spells
{
    char name[20];
    ptr_to_spells spell;
    int manacost;
};
struct bot_spells
{
    char name[20];
    ptr_to_spells spell;
    int manacost;
};
struct spells
{
    char name[20];
    ptr_to_spells spell;
    int manacost;
};
int bot_decides_which_card_to_block(struct player_cards *ptr_to_player_cards, struct bot *ptr_to_bot, struct player *ptr_to_player)
{
    int k;
    for (k = 0; k < 4; k++)
    {
        if (ptr_to_bot->health > (ptr_to_player_cards + k)->damage)
        {
            if (ptr_to_player->mana >= (ptr_to_player_cards + k)->manacost)
            {
                return k;
            }
        }
    }
}
void health_steal(struct player_cards *ptr_to_player_cards, struct bot_cards *ptr_to_bot_cards, struct bot *ptr_to_bot, struct player *ptr_to_player, int who)
{
    if (who == 1)
    {
        ptr_to_player->health += 15;
        ptr_to_bot->health -= 15;
    }
    if (who == 2)
    {
        ptr_to_player->health -= 15;
        ptr_to_bot->health += 15;
    }
}
void mana_steal(struct player_cards *ptr_to_player_cards, struct bot_cards *ptr_to_bot_cards, struct bot *ptr_to_bot, struct player *ptr_to_player, int who)
{
    if (who == 1)
    {
        ptr_to_player->mana += 15;
        ptr_to_bot->mana -= 15;
    }
    if (who == 2)
    {
        ptr_to_player->mana -= 15;
        ptr_to_bot->mana += 15;
    }
}
void heal(struct player_cards *ptr_to_player_cards, struct bot_cards *ptr_to_bot_cards, struct bot *ptr_to_bot, struct player *ptr_to_player, int who)
{
    if (who == 1)
    {
        ptr_to_player->health += 20;
    }
    if (who == 2)
    {
        ptr_to_bot->health += 20;
    }
}
void block_card(struct player_cards *ptr_to_player_cards, struct bot_cards *ptr_to_bot_cards, struct bot *ptr_to_bot, struct player *ptr_to_player, int who)
{
    int i;

    if (who == 1)
    {
        printf("select the card you want to block \n1.%s(mana cost=%d)(damage:%d)\n2.%s(mana cost=%d)(damage:%d)\n3.%s(mana cost=%d)(damage:%d)\n4.%s(mana cost=%d)(damage:%d)\n", (ptr_to_player_cards)->name, (ptr_to_player_cards)->manacost, (ptr_to_player_cards)->damage, (ptr_to_player_cards + 1)->name, (ptr_to_player_cards + 1)->manacost, (ptr_to_player_cards + 1)->damage, (ptr_to_player_cards + 2)->name, (ptr_to_player_cards + 2)->manacost, (ptr_to_player_cards + 2)->damage, (ptr_to_player_cards + 3)->name, (ptr_to_player_cards + 3)->manacost, (ptr_to_player_cards + 3)->damage);
        scanf("%d", &i);
        i--;
        (ptr_to_bot_cards + i)->isblocked = 1;
    }
    if (who == 2)
    {
        i = bot_decides_which_card_to_block(ptr_to_player_cards, ptr_to_bot, ptr_to_player);
        (ptr_to_player_cards + i)->isblocked = 1;
    }
}
int player_turn(struct player *ptr_to_player, struct bot *ptr_to_bot, struct player_cards *ptr_to_player_cards, struct bot_cards *ptr_to_bot_cards, int player_element, struct player_spells *ptr_to_player_spells, int Level)
{
    int i, type;
    int who = 1;
    if (ptr_to_player->mana < 10)
    {
        printf("bot won the match");
        return 2;
    }
    printf("\nplayer turn:\nwhich type of card do u want to choose\n1-Attack Card \n2-SpellCard\n");
    scanf("%d", &type);
    if (type == 2)
    {
        printf("choose the spell card u want to use\n");
        for (i = 0; i < 1 + Level; i++)
        {
            printf("%d . %s\n", i + 1, ((ptr_to_player_spells + i)->name));
        }

        scanf("%d", &i);
        i--;
        ((ptr_to_player_spells + i)->spell)(ptr_to_player_cards, ptr_to_bot_cards, ptr_to_bot, ptr_to_player, who);
    }
    if (type == 1)
    {
        int k;
        k=0;
        do
        {
            // Add the printf statements to show cards                                                                                                                                                                                          (ptr_to_player_cards)->manacost   (ptr_to_player_cards)->damage
            //  printf("%d , %d \n" , ptr_to_bot->health , ptr_to_player ->mana);
            printf("select the attack card u want to choose\n1.%s special card(mana cost=%d)(damage:fuck around and find out)\n", (ptr_to_player_cards + k)->element, (ptr_to_player_cards + k)->manacost);
            for (k = 1; k < 4 + Level; k++)
            {
                printf("%d.%s(mana cost=%d)(damage:%d)\n", k + 1, (ptr_to_player_cards + k)->name, (ptr_to_player_cards + k)->manacost, (ptr_to_player_cards + k)->damage);
            }
            scanf("%d", &i);
            i--;
            if (i < 0 && i > 4 + Level)
            {
                printf("invalid move , try again");
                continue;
            }
            if ((ptr_to_player_cards + i)->isblocked == 1)
            {
                printf("this card is blocked for this turn");
                continue;
            }

            if (ptr_to_player->mana < (ptr_to_player_cards + i)->manacost)
            {
                printf("your mana is not enough choose another card");
                continue;
                // return player_turn(ptr_to_player , ptr_to_bot , ptr_to_player_cards , ptr_to_bot_cards , player_element);
            }
            else
            {
                break;
            }
        } while (i != 20);

        ptr_to_bot->health = ptr_to_bot->health - (ptr_to_player_cards + i)->damage;
        ptr_to_player->mana = ptr_to_player->mana - (ptr_to_player_cards + i)->manacost;
    }
    printf("--------------------------------------------------");
    printf("\nyou inflicted a damage of :%d\nremaining bot health : %d\n", 100 - ptr_to_bot->health, ptr_to_bot->health);
    printf("how much mana is left for you  : %d\n", ptr_to_player->mana);
    printf("-------------------------------------------------------------------------------------");
    for (i = 0; i < 4 + Level; i++)
    {
        (ptr_to_player_cards + i)->isblocked = 0;
    }

    if (ptr_to_bot->health < 1)
    {
        printf("\ntake that you evil mf!!!\n you have succesfully defeated the monster\n\n");
        return 1;
    }
    else
    {
        return 0;
    }
}
void bot_move_decider(struct player *ptr_to_player, struct bot *ptr_to_bot, struct player_cards *ptr_to_player_cards, struct bot_cards *ptr_to_bot_cards, int bot_element, struct bot_spells *ptr_to_bot_spells, int Level, int *ptr_to_arr_of_moves)
{
    int i;
    for (i = 0; i < 4 + Level; i++)
    {
        if ((ptr_to_bot_cards + i)->isblocked == 0)
        {
            if (ptr_to_player->health <= (ptr_to_bot_cards + i)->damage)
            {
                if ((ptr_to_bot_cards + i)->manacost <= ptr_to_bot->mana)
                {
                    *ptr_to_arr_of_moves = i;
                    *(ptr_to_arr_of_moves + 1) = 1;
                    return;
                }
            }
        }
    }
    for (i = 0; i < 1 + Level; i++)
    {
        if (ptr_to_player->mana < 25 || ptr_to_bot->mana < 10)
        {
            if ((ptr_to_bot_spells + i)->manacost <= ptr_to_bot->mana)
            {
                if (strcmp((ptr_to_bot_spells + i)->name, "Mana Steal") == 0)
                {
                    *ptr_to_arr_of_moves = i;
                    *(ptr_to_arr_of_moves + 1) = 2;
                    return;
                }
            }
        }
        else if ((ptr_to_player)->health <= 15)
        {
            if ((ptr_to_bot_spells + i)->manacost <= ptr_to_bot->mana)
            {
                if (strcmp((ptr_to_bot_spells + i)->name, "HP Steal") == 0)
                {
                    *ptr_to_arr_of_moves = i;
                    *(ptr_to_arr_of_moves + 1) = 2;
                    return;
                }
            }
        }
        else if (ptr_to_bot->health <= 10)
        {
            if ((ptr_to_bot_spells + i)->manacost <= ptr_to_bot->mana)
            {
                if (strcmp((ptr_to_bot_spells + i)->name, "Heal Spell") == 0)
                {
                    *ptr_to_arr_of_moves = i;
                    *(ptr_to_arr_of_moves + 1) = 2;
                    return;
                }
            }
        }
    }
    for (i = 0; i < 1; i++)
    {
        if ((ptr_to_bot_spells + i)->manacost <= ptr_to_bot->mana)
        {
            if (strcmp((ptr_to_bot_spells + i)->name, "Card Blocker") == 0)
            {
                for (int k = 0; k < 4; k++)
                {
                    if (ptr_to_bot->health > (ptr_to_player_cards + k)->damage)
                    {
                        if (ptr_to_player->mana >= (ptr_to_player_cards + k)->manacost)
                        {
                            *ptr_to_arr_of_moves = i;
                            *(ptr_to_arr_of_moves + 1) = 2;
                            return;
                        }
                    }
                }
            }
        }
    }

    float dmage_per_mana;
    float heighest = 0.000;

    for (i = 0; i < 4 + Level; i++)
    {
        if (ptr_to_bot->mana >= (ptr_to_bot_cards + i)->manacost)
        {
            if ((ptr_to_bot_cards + i)->isblocked == 0)
            {
                dmage_per_mana = ((ptr_to_bot_cards + i)->damage) / ((ptr_to_bot_cards + i)->manacost);
                if (dmage_per_mana > heighest)
                {
                    heighest = dmage_per_mana;
                }
            }
        }
    }

    for (i = 0; i < 4 + Level; i++)
    {
        if ((ptr_to_bot_cards + i)->isblocked == 0)
        {
            dmage_per_mana = ((ptr_to_bot_cards + i)->damage) / ((ptr_to_bot_cards + i)->manacost);
            if (dmage_per_mana == heighest)
            {
                *ptr_to_arr_of_moves = i;
                *(ptr_to_arr_of_moves + 1) = 1;
                return;
            }
        }
    }
}
int bot_turn(struct player *ptr_to_player, struct bot *ptr_to_bot, struct player_cards *ptr_to_player_cards, struct bot_cards *ptr_to_bot_cards, int bot_element, struct bot_spells *ptr_to_bot_spells, int Level, struct player_spells *ptr_to_player_spells)
{
    int i;
    int arr_of_moves[2] = {0, 0};
    int *ptr_to_arr_of_moves = arr_of_moves;
    if (ptr_to_bot->mana < 10)
    {
        printf("congrats,U have won");
    }
    int who = 2;
    bot_move_decider(ptr_to_player, ptr_to_bot, ptr_to_player_cards, ptr_to_bot_cards, bot_element, ptr_to_bot_spells, Level, ptr_to_arr_of_moves);
    int card_type = *(ptr_to_arr_of_moves + 1);
    i = *ptr_to_arr_of_moves;
    if (card_type == 1)
    {
        ptr_to_player->health = ptr_to_player->health - (ptr_to_bot_cards + i)->damage;
        ptr_to_bot->mana = ptr_to_bot->mana - (ptr_to_bot_cards + i)->manacost;
        printf("\nmonster used:%s", (ptr_to_player_cards + i)->name);
        printf("\nthe monster inflicted %d damage\nremaining player health : %d\n", (ptr_to_bot_cards + i)->damage, ptr_to_player->health);
        printf("monster has %d mana left\n", ptr_to_bot->mana);
        printf("--------------------------------------------------------------");
    }
    if (card_type == 2)
    {
        printf("\nthe monster used %s\n", (ptr_to_player_spells + i)->name);
        ((ptr_to_bot_spells + i)->spell)(ptr_to_player_cards, ptr_to_bot_cards, ptr_to_bot, ptr_to_player, who);
    }

    for (i = 0; i < 4 + Level; i++)
    {
        (ptr_to_bot_cards + i)->isblocked = 0;
    }

    if (ptr_to_player->health < 1)
    {
        printf("\nBUAHAHAHAHAHHAH TAKE THAT FUCKER\n GAME OVER! YOU FAILED\n\n");
        return 1;
    }
    else
    {
        return 0;
    }
}
void player_cards_assigning(struct cards *ptr_to_all_cards, struct player_cards *ptr_to_player_cards, char elements[4][10], int player_element, struct player_spells *ptr_to_player_spells, struct spells *ptr_to_all_spells)
{
    int i, k, j;
    k = 0;
    j = player_element;
    // for(i=0;i<4;i++){
    //     printf("%s",elements[i]);
    // }4
    // printf("%d",j);
    strcpy((ptr_to_player_cards + k)->element, elements[j]);
    (ptr_to_player_cards + k)->damage = rand() % 50 + 40;
    (ptr_to_player_cards + k)->manacost = 65;
    strcpy((ptr_to_player_cards + k)->name, (ptr_to_all_cards + i)->name);

    for (k = 1; k < 4; k++)
    {
        i = rand() % 12;
        strcpy((ptr_to_player_cards + k)->element, (ptr_to_all_cards + i)->element);
        (ptr_to_player_cards + k)->damage = (ptr_to_all_cards + i)->damage;
        (ptr_to_player_cards + k)->manacost = (ptr_to_all_cards + i)->manacost;
        strcpy((ptr_to_player_cards + k)->name, (ptr_to_all_cards + i)->name);
        if (strcmp((ptr_to_player_cards + k)->element, elements[j]) == 0)
        {
            (ptr_to_player_cards + k)->damage += 0.1 * ((ptr_to_player_cards + k)->damage);
        }
    }

    i = 0;
    j = rand() % 4;
    (ptr_to_player_spells + i)->spell = (ptr_to_all_spells + j)->spell;
    strcpy((ptr_to_player_spells + i)->name, (ptr_to_all_spells + j)->name);
    (ptr_to_player_spells + i)->manacost = (ptr_to_all_spells + j)->manacost;
}
void bot_cards_assigning(struct cards *ptr_to_all_cards, struct bot_cards *ptr_to_bot_cards, char elements[4][10], int bot_element, struct bot_spells *ptr_to_bot_spells, struct spells *ptr_to_all_spells)
{
    int i, k, j;
    k = 0;
    j = bot_element;
    strcpy((ptr_to_bot_cards + k)->element, elements[j]);
    (ptr_to_bot_cards + k)->damage = (rand() % 60) + 40;
    (ptr_to_bot_cards + k)->manacost = 65;
    strcpy((ptr_to_bot_cards + k)->name, (ptr_to_all_cards + i)->name);

    for (k = 1; k < 4; k++)
    {
        i = rand() % 12;
        strcpy((ptr_to_bot_cards + k)->element, (ptr_to_all_cards + i)->element);
        (ptr_to_bot_cards + k)->damage = (ptr_to_all_cards + i)->damage;
        (ptr_to_bot_cards + k)->manacost = (ptr_to_all_cards + i)->manacost;
        strcpy((ptr_to_bot_cards + k)->name, (ptr_to_all_cards + i)->name);
        if (strcmp((ptr_to_bot_cards + k)->element, elements[j]) == 0)
        {
            (ptr_to_bot_cards + k)->damage += 0.1 * ((ptr_to_bot_cards + k)->damage);
        }
    }
    i = 0;
    j = rand() % 4;
    (ptr_to_bot_spells + i)->spell = (ptr_to_all_spells + j)->spell;
    strcpy((ptr_to_bot_spells + i)->name, (ptr_to_all_spells + j)->name);
    (ptr_to_bot_spells + i)->manacost = (ptr_to_all_spells + j)->manacost;
}
void card_making(struct cards *ptr_to_all_cards)
{
    int i, j, k = 0;
    char elements[4][10] = {"Earth", "Water", "Fire", "Air"};
    char type[3][20] = {"Zeko", "BTC", "Zoige"};
    char mana_type[3][10] = {"Neevin", "Hionfi", "Fessoni"};
    int damage[3] = {(rand() % 10) + 40, (rand() % 20) + 40, (rand() % 20) + 40};
    int mana[3] = {10, 20, 30};

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 3; j++)
        {
            strcpy((ptr_to_all_cards + k)->element, elements[i]);
            (ptr_to_all_cards + k)->damage = damage[j];
            (ptr_to_all_cards + k)->manacost = mana[j];
            strcpy((ptr_to_all_cards + k)->name, elements[i]);
            strcat((ptr_to_all_cards + k)->name, " - ");
            strcat((ptr_to_all_cards + k)->name, type[j]);
            strcat((ptr_to_all_cards + k)->name, " - ");
            strcat((ptr_to_all_cards + k)->name, mana_type[j]);
            k++;
        }
    }
}
void reallocating_player_cards(struct spells *ptr_to_all_spells, struct cards *ptr_to_all_cards, struct player_cards *ptr_to_player_cards, struct player_spells *ptr_to_player_spells, int Level)
{
    int newSize;
    newSize = 4 + Level;

    printf("HERE'S THE CHEST OF LOST SHADOWS\nSelect a new attack card:\n");
    for (int i = 0; i < 12; i++)
    {
        printf("%d. %s (Mana Cost: %d, Damage: %d)\n", i + 1, (ptr_to_all_cards + i)->name, (ptr_to_all_cards + i)->manacost, (ptr_to_all_cards + i)->damage);
    }
    int cardChoice;
    printf("Enter the number corresponding to the attack card you want to choose: ");
    scanf("%d", &cardChoice);
    if (cardChoice >= 1 && cardChoice <= 12)
    {
        cardChoice--;
        strcpy((ptr_to_player_cards + (newSize - 1))->element, (ptr_to_all_cards + cardChoice)->element);
        (ptr_to_player_cards + (newSize - 1))->damage = (ptr_to_all_cards + cardChoice)->damage;
        (ptr_to_player_cards + (newSize - 1))->manacost = (ptr_to_all_cards + cardChoice)->manacost;
        strcpy((ptr_to_player_cards + (newSize - 1))->name, (ptr_to_all_cards + cardChoice)->name);
    }
    else
    {
        printf("Invalid card choice.\n");
    }
    int new_Size;
    new_Size = 1;
    new_Size += Level;

    printf("\nSelect a new spell:\n");
    for (int i = 0; i < 4; i++)
    {
        printf("%d. %s\n", i + 1, (ptr_to_all_spells + i)->name);
    }
    int spellChoice;
    printf("Enter the number corresponding to the spell you want to choose: ");
    scanf("%d", &spellChoice);
    if (spellChoice >= 1 && spellChoice <= 4)
    {
        spellChoice--;

        (ptr_to_player_spells + (new_Size - 1))->spell = (ptr_to_all_spells + spellChoice)->spell;
        strcpy((ptr_to_player_spells + (new_Size - 1))->name, (ptr_to_all_spells + spellChoice)->name);
    }
}
void reallocating_bot_cards(struct spells *ptr_to_all_spells, struct cards *ptr_to_all_cards, struct bot_cards *ptr_to_bot_cards, struct player_spells *ptr_to_bot_spells, int Level)
{
    int newSize = 4 + Level;
    while (1)
    {
        struct bot_cards *temp_bot_cards = realloc(ptr_to_bot_cards, newSize * sizeof(struct bot_cards));
        if (temp_bot_cards != NULL)
        {
            ptr_to_bot_cards = temp_bot_cards;

            break;
        }
    }

    int cardChoice;
    cardChoice = rand() % 12;
    strcpy((ptr_to_bot_cards + newSize)->element, (ptr_to_all_cards + cardChoice)->element);
    (ptr_to_bot_cards + newSize)->damage = (ptr_to_all_cards + cardChoice)->damage;
    (ptr_to_bot_cards + newSize)->manacost = (ptr_to_all_cards + cardChoice)->manacost;
    strcpy((ptr_to_bot_cards + newSize)->name, (ptr_to_all_cards + cardChoice)->name);

    int new_Size;
    new_Size += Level;
    ptr_to_bot_spells = realloc(ptr_to_bot_spells, new_Size * sizeof(struct bot_spells));

    int spellChoice;
    spellChoice = rand() % 4;
    (ptr_to_bot_spells + new_Size)->spell = (ptr_to_all_spells + spellChoice)->spell;
    strcpy((ptr_to_bot_spells + new_Size)->name, (ptr_to_all_spells + spellChoice)->name);
}

void upgrade_attributes(struct player *ptr_to_player, struct player_cards *ptr_to_player_cards, int points)
{
    int choice, upgrade_amount, card_index;
    ptr_to_player->health = 100;
    ptr_to_player->mana = 100;
    while (points > 0)
    {
        printf("You have %d points. Choose an option:\n", points);
        printf("1. Increase health\n");
        printf("2. Increase card damage\n");
        printf("3. Exit\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter the amount of health to increase (up to your total points of %d): ", points);
            scanf("%d", &upgrade_amount);
            if (upgrade_amount > points)
            {
                printf("You don't have enough points to increase health by this amount. Try again.\n");
                continue;
            }
            ptr_to_player->health += upgrade_amount;
            points -= upgrade_amount;
            printf("Your health is now %d and you have %d points left.\n", ptr_to_player->health, points);
            break;
        case 2:
            printf("Enter the index of the card to upgrade (1 to 4): ");
            scanf("%d", &card_index);
            card_index--;
            if (card_index < 0 || card_index > 3)
            {
                printf("Invalid card index. Try again.\n");
                continue;
            }
            printf("Enter the amount of damage to add to the card '%s' (up to your total points of %d): ", (ptr_to_player_cards + card_index)->name, points);
            scanf("%d", &upgrade_amount);
            if (upgrade_amount > points)
            {
                printf("You don't have enough points to increase card damage by this amount. Try again.\n");
                continue;
            }
            (ptr_to_player_cards + card_index)->damage += upgrade_amount;
            points -= upgrade_amount;
            printf("The damage of your card '%s' is now %d and you have %d points left.\n", (ptr_to_player_cards + card_index)->name, (ptr_to_player_cards + card_index)->damage, points);
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    }
}
void spell_card_making(struct spells *ptr_to_all_spells, ptr_to_spells spell_cards[4])
{
    int i;
    char spell_names[4][20] = {"HP Stealer", "Mana Stealer", "Heal Spell", "Card Blocker"};
    int mana_cost[4] = {12, 12, 10, 15};
    for (i = 0; i < 4; i++)
    {
        (ptr_to_all_spells + i)->spell = spell_cards[i];
        (ptr_to_all_spells + i)->manacost = mana_cost[i];
        strcpy((ptr_to_all_spells + i)->name, spell_names[i]);
    }
}
void level1()
{
    printf("------------------------------------------------------------------");
    printf("\nYou opened your eyes, stood up from the ground, and found yourself in an ominous dark forest.\n you remember the task master fushiguiro gave you to collect the 2 forbidden cards\n");
    printf("In front of you lies a mysterious cave, and the dense forest surrounds you.\n");
    printf("What choice will you make?\n1.go inside the cave\n2.explore the dense forest\n\n");
    int temp;
    scanf("%d", &temp);
    printf("--------------------------------------------------------------");
    if (temp == 1)
    {
        printf("\nyou're inside the cave all gasping for air\n you discover a lake inside the cave\n");
        printf("BLARIE ZEGOST:SO YOU'RE THE ONE WHOS LOOKING FOR THE 2 CARDS OF INFINITY YOU DONT WANT TO MESS WITH ME\n ");
        printf("Asta:your omnious aura is so evil lets see what you got\n\n");
        printf("--------------------------------------------------------------");
    }
    else
    {

        printf("\nyou're deep inside the forest losing vision in the dark\n you discover a trench under the oak tree\n");
        printf("BLARIE ZEGOST:SO YOU'RE THE ONE WHOS LOOKING FOR THE 2 CARDS OF INFINITY YOU DONT WANT TO MESS WITH ME\n ");
        printf("Asta:your omnious aura is so evil lets see what you got\n\n");
        printf("--------------------------------------------------------------");
    }
}
void level2()
{
    printf("\nYou've overcome the initial challenges and found yourself in an ancient temple.\n");
    printf("In the temple, you can see two paths. One leading to a golden door glowing with a strange aura, and the other one is a dark tunnel.\n");
    printf("What choice will you make?\n1.Go through the golden door\n2.Traverse the dark tunnel\n\n");
    int temp;
    scanf("%d", &temp);
    printf("--------------------------------------------------------------");
    if (temp == 1)
    {
        printf("\nYou're in a grand hall filled with golden treasures.\n");
        printf("Suddenly, a stone statue comes to life and speaks,\n");
        printf("STONE GUARDIAN: YOU DARE TO SEEK THE 2 CARDS OF INFINITY. PREPARE TO FACE MY WRATH!\n");
        printf("Asta: Your golden shine doesn't scare me, Guardian. Bring it on!\n\n");
        printf("--------------------------------------------------------------");
    }
    else
    {
        printf("\nYou're in a labyrinth of tunnels, with echoes of strange whispers.\n");
        printf("Suddenly, the whispering voices materialize into a shadowy figure.\n");
        printf("SHADOW WRAITH: YOU SEEK THE 2 CARDS OF INFINITY. YOU WILL NOT LEAVE THIS PLACE ALIVE!\n");
        printf("Asta: You may be a Wraith, but your shadows don't frighten me. Let's do this!\n\n");
        printf("--------------------------------------------------------------");
    }
}
void level3()
{
    printf("After battling through the temple, you find yourself at the edge of a vast lake.\n");
    printf("There is a small boat by the shore, and a path that leads into a dense, thorny forest.\n");
    printf("What choice will you make?\n1. Sail across the lake\n2. Venture into the thorny forest\n");
    int temp;
    scanf("%d", &temp);
    printf("--------------------------------------------------------------");
    if (temp == 1)
    {
        printf("\nYou're in the middle of the lake. The water starts swirling, and a water spirit emerges.\n");
        printf("WATER SPIRIT: SO, YOU'RE THE ONE SEEKING THE 2 CARDS OF INFINITY. YOU'LL HAVE TO GET PAST ME FIRST!\n");
        printf("Asta: Spirit or not, I'm not backing down. Let's see what you got!\n");
        printf("--------------------------------------------------------------");
    }
    else
    {
        printf("\nYou're in the heart of the forest, surrounded by thorny vines. Suddenly, a forest specter appears.\n");
        printf("FOREST SPECTER: YOU SEEK THE 2 CARDS OF INFINITY. THESE WOODS WILL BE YOUR GRAVE!\n");
        printf("Asta: Your threats mean nothing to me, specter. Time for a showdown!\n");
        printf("--------------------------------------------------------------");
    }
}
void finalLevel()
{
    printf("\nAfter overcoming numerous trials and defeating formidable foes, you've finally collected the 2 Infinity Stones and 2 Forbidden Spell Cards.\n");
    printf("With a sense of accomplishment, you make your way back to Master Fushiguiro.\n");
    printf("Upon reaching the Master's quarters, a horrifying sight greets you. Master Fushiguiro has been secretly murdered!\n");
    printf("The room is in chaos, and the aura of the perpetrator still lingers in the air. You clench your fists, vowing to avenge your Master.\n");
    printf("To be continued in Semester 2...\n");
}
int main()
{
    int Level = 0;
    int i = 0; // this is not part of the code will be removed
    srand(time(NULL));
    // struct cards card_s[12];
    // yahaan pointer to structre array ko initialize kara hai
    struct cards *ptr_to_all_cards = (struct cards *)calloc(12, sizeof(struct cards));
    card_making(ptr_to_all_cards);

    ptr_to_spells spell_cards[4] = {health_steal, mana_steal, heal, block_card};
    struct spells *ptr_to_all_spells = (struct spells *)calloc(4, sizeof(struct spells));
    spell_card_making(ptr_to_all_spells, spell_cards);

    // this block is like the element deciding block
    char elements[4][10] = {"earth", "water", "fire", "air"};
    int player_element, bot_element;
    printf("choose your main element\n1-Earth\n2-water\n3-Fire\n4-air\n");
    scanf("%d", &player_element);
    printf("----------------------------------------------");
    player_element--;
    bot_element = rand() % 4;
    // give the player its cards
    // struct player_cards player_card_s[4];
    struct player_cards *ptr_to_player_cards = (struct player_cards *)calloc(8, sizeof(struct player_cards));
    struct player_spells *ptr_to_player_spells = (struct player_spells *)calloc(5, sizeof(struct player_spells));
    player_cards_assigning(ptr_to_all_cards, ptr_to_player_cards, elements, player_element, ptr_to_player_spells, ptr_to_all_spells);

    // give the bot its card
    // struct bot_cards bot_card_s[4];
    struct bot_cards *ptr_to_bot_cards = (struct bot_cards *)calloc(8, sizeof(struct bot_cards));
    struct bot_spells *ptr_to_bot_spells = (struct bot_spells *)calloc(5, sizeof(struct bot_spells));
    bot_cards_assigning(ptr_to_all_cards, ptr_to_bot_cards, elements, bot_element, ptr_to_bot_spells, ptr_to_all_spells);

    // initialize the structs for player and the bot
    struct player p1;
    struct player *ptr_to_player = &p1;
    p1.mana = 100;
    p1.health = 100;
    struct bot b1;
    struct bot *ptr_to_bot = &b1;
    b1.mana = 100;
    b1.health = 100;
    i = 0;
    int points;
    level1();
    while (i == 0)
    {
        i = player_turn(ptr_to_player, ptr_to_bot, ptr_to_player_cards, ptr_to_bot_cards, player_element, ptr_to_player_spells, Level);
        if (i == 1)
        {
            Level++;
            int points = 10; // Assuming you want to start with 100 points
            ptr_to_bot->health = 100;
            upgrade_attributes(ptr_to_player, ptr_to_player_cards, points);
            printf("level %d Starts", Level);
            reallocating_player_cards(ptr_to_all_spells, ptr_to_all_cards, ptr_to_player_cards, ptr_to_player_spells, Level);
            level2();
            i = 0;
            while (i == 0) // set the winning conditions here
            {
                i = player_turn(ptr_to_player, ptr_to_bot, ptr_to_player_cards, ptr_to_bot_cards, player_element, ptr_to_player_spells, Level);
                if (i == 1)
                {
                    Level++;
                    int points = 10; // Assuming you want to start with 100 points
                    ptr_to_bot->health = 100;
                    upgrade_attributes(ptr_to_player, ptr_to_player_cards, points);
                    printf("level %d Starts", Level);
                    reallocating_player_cards(ptr_to_all_spells, ptr_to_all_cards, ptr_to_player_cards, ptr_to_player_spells, Level);
                    level3();
                    i = 0;
                    while (i == 0) // set the winning conditions here
                    {
                        i = player_turn(ptr_to_player, ptr_to_bot, ptr_to_player_cards, ptr_to_bot_cards, player_element, ptr_to_player_spells, Level);
                        if (i == 1)
                        {
                            finalLevel();
                            break;
                        }
                        i = bot_turn(ptr_to_player, ptr_to_bot, ptr_to_player_cards, ptr_to_bot_cards, bot_element, ptr_to_bot_spells, Level, ptr_to_player_spells);
                        if (i == 1)
                        {
                            break;
                        }
                    }
                    break;
                }
                i = bot_turn(ptr_to_player, ptr_to_bot, ptr_to_player_cards, ptr_to_bot_cards, bot_element, ptr_to_bot_spells, Level, ptr_to_player_spells);
                if (i == 1)
                {
                    break;
                }
            }
            break;
        }
        i = bot_turn(ptr_to_player, ptr_to_bot, ptr_to_player_cards, ptr_to_bot_cards, bot_element, ptr_to_bot_spells, Level, ptr_to_player_spells);
        if (i == 1 || i == 2)
        {
            break;
        }
    }
    // free(ptr_to_bot);
    free(ptr_to_all_spells);
    // free(ptr_to_player);
    free(ptr_to_all_cards);
    free(ptr_to_player_cards);
    free(ptr_to_bot_cards);
    free(ptr_to_player_spells);
    free(ptr_to_bot_spells);
}
