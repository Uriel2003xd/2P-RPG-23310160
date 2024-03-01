//
// Created by Victor Navarro on 19/02/24.
//

#include "Combat.h"
#include <string>
#include <iostream>
#include <utility>
#include <algorithm>

using namespace std;
bool compareSpeed(Character *a, Character *b) {
    return a->getSpeed() > b->getSpeed();
}

Combat::Combat(vector<Character *> _participants) {
    participants = std::move(_participants);
    for(auto participant : participants) {
        if (participant->getIsPlayer()) {
            partyMembers.push_back((Player *) participant);
        } else {
            enemies.push_back((Enemy *) participant);
        }
    }
}

Combat::Combat(vector<Player*> _partyMembers, vector<Enemy*> _enemies) {
    partyMembers = std::move(_partyMembers);
    enemies = std::move(_enemies);
    participants = vector<Character*>();
    participants.insert(participants.end(), partyMembers.begin(), partyMembers.end());
    participants.insert(participants.end(), enemies.begin(), enemies.end());
}

Combat::Combat() {
    participants = vector<Character*>();
}

void Combat::addParticipant(Character *participant) {
    participants.push_back(participant);
    if(participant->getIsPlayer()){
        partyMembers.push_back((Player*) participant);
    } else {
        enemies.push_back((Enemy*) participant);
    }
}

void Combat::combatPrep() {
    // Sort participants by speed
    sort(participants.begin(), participants.end(), compareSpeed);
}

string Combat::toString() {
    string result = "";
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        result += (*it)->toString() + "\n";
    }
    cout<<"===================="<<endl;
    return result;
}

Character* Combat::getTarget(Character* attacker) {
    vector<Character*>::iterator it;
    for(it = participants.begin(); it != participants.end(); it++){
        if((*it)->getIsPlayer() != attacker->getIsPlayer()){
            return *it;
        }
    }
    //TODO: Handle this exception
    return nullptr;
}


int playerDefenseTurn = 0;
int enemyDefenseTurn = 0;
int ronda = 1;

void Combat::doCombat() {
    cout<< "Se ha iniciado el combate" << endl;
    combatPrep();
    while(participants.size() > 1){

        vector<Character*>::iterator it = participants.begin();
        cout<<"Ronda " << ronda << endl;
        while(it != participants.end()) {
            Character *target = nullptr;
            if ((*it)->getIsPlayer()) {
                if (playerDefenseTurn == 1) {
                    (*it)->resetDefense();
                    playerDefenseTurn = 0;
                }
                cout << "Sigue el turno de: " << (*it)->getName() << endl;
                int action;
                cout << "Que vas a decidir?" << endl;
                cout << "1. Atacar" << endl;
                cout << "2. Defender" << endl;
                cin >> action;
                switch (action) {
                    case 1:
                        target = ((Player *) *it)->selectTarget(enemies);
                        (*it)->doAttack(target);
                        break;
                    case 2:
                        target = ((Player *) *it)->selectTarget(enemies);
                        (*it)->defend();
                        playerDefenseTurn = 1;
                        break;
                    default:
                        cout << "Opcion 1 o 2" << endl;
                        break;
                }
            } else {
                if (enemyDefenseTurn == 1) {
                    (*it)->resetDefense();
                    enemyDefenseTurn = 0;
                }
                cout << "Sigue el turno del enemigo: " << (*it)->getName() << "!" << endl;
                target = ((Enemy *) *it)->selectTarget(partyMembers);
                if ((*it)->getHealth() < ((*it)->getOriginalHealth() * 0.60) && rand() % 100 < 40) {
                    (*it)->defend();
                    cout << "El Enemigo: " << (*it)->getName() << " Esquivo" << endl;
                    enemyDefenseTurn = 1;
                } else {
                    (*it)->doAttack(target);
                }
            }
            if(target->getHealth() <= 0){
                it = participants.erase(remove(participants.begin(), participants.end(), target), participants.end());
                if(target->getIsPlayer()){
                    partyMembers.erase(remove(partyMembers.begin(), partyMembers.end(), target), partyMembers.end());
                    if(partyMembers.size() == 0){
                        cout << "Game Over" << endl;
                        return;
                    }
                } else {
                    cout << "Eliminaste a: " << target->getName() << endl;
                    enemies.erase(remove(enemies.begin(), enemies.end(), target), enemies.end());
                    if(enemies.size() == 0){
                        cout << "Felicidades Has Ganado " << endl;
                        return;
                    }
                }
            } else {
                it++;
            }
        }
        ronda++;
    }
}
