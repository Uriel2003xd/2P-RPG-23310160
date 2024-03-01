#include <iostream>
#include "Player/Player.h"
#include "Enemy/Enemy.h"
#include "Combat/Combat.h"

int main() {
    Player *player = new Player("Carmencita", 80, 10, 1, 6);
    Enemy *enemy = new Enemy("Chente Titanic", 15, 8, 3, 3, 15);
    Enemy *enemy2 = new Enemy("Abejita Furiosa", 10, 8, 3, 3, 15);

    vector<Character*> participants;

    participants.push_back(player);
    participants.push_back(enemy);
    participants.push_back(enemy2);

    Combat *combat = new Combat(participants);
    combat->doCombat();

    delete player;
    delete enemy;
    delete combat;
    return 0;
}
